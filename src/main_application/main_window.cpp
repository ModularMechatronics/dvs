#include "main_window.h"

#include <unistd.h>
#include <wx/wfstream.h>
#include <wx/wxprec.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "dvs/constants.h"
#include "dvs/math/math.h"
#include "events.h"
#include "filesystem.h"
#include "window_view.h"

using namespace dvs::internal;

MainWindow::MainWindow(const std::vector<std::string>& cmdl_args)
    : wxFrame(NULL, wxID_ANY, "", wxPoint(30, 30), wxSize(50, 50))
{
    static_cast<void>(cmdl_args);
    window_initialization_in_progress_ = true;
    data_receiver_ = new DataReceiver(dvs::internal::kUdpPortNum);
    open_project_file_queued_ = false;

    window_callback_id_ = dvs_ids::WINDOW_TOGGLE;

    configuration_agent_ = new ConfigurationAgent();

    task_bar_ = new CustomTaskBarIcon();
    wxIcon* icon = new wxIcon();
    icon->LoadFile("../resources/images/apple.ico", wxBITMAP_TYPE_ICO, 32, 32);

    if (!task_bar_->SetIcon(*icon, "wxTaskBarIcon Sample\n"))
    {
        wxLogError("Could not set icon.");
    }
    this->SetPosition(wxPoint(0, 0));

    menu_bar_ = createMainMenuBar();

    SetMenuBar(menu_bar_);
    wxMenuBar::MacSetCommonMenuBar(menu_bar_);

    Bind(wxEVT_MENU, &MainWindow::newProjectCallback, this, wxID_NEW);
    Bind(wxEVT_MENU, &MainWindow::saveProjectCallback, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MainWindow::openExistingFileCallback, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainWindow::saveProjectAsCallback, this, wxID_SAVEAS);
    Bind(wxEVT_MENU, &MainWindow::exitApplication, this, wxID_HELP_INDEX);
    Bind(wxEVT_MENU, &MainWindow::newWindowCallback, this, wxID_HELP_CONTENTS);

    task_bar_->setOnMenuExitCallback([this]() -> void { this->Destroy(); });
    task_bar_->setOnMenuFileNew([this]() -> void { newProject(); });
    task_bar_->setOnMenuFileOpen([this]() -> void { openExistingFile(); });
    task_bar_->setOnMenuFileSave([this]() -> void { saveProject(); });
    task_bar_->setOnMenuFileSaveAs([this]() -> void { saveProjectAs(); });
    task_bar_->setOnMenuSubWindow(
        [this](const std::string& window_name) -> void { toggleWindowVisibility(window_name); });
    task_bar_->setOnMenuPreferences([this]() -> void { preferences(); });
    task_bar_->setOnMenuFileNewWindow([this]() -> void { this->newWindow(); });

    notification_from_gui_element_key_pressed_ = [this](const char key) { notifyChildrenOnKeyPressed(key); };
    notification_from_gui_element_key_released_ = [this](const char key) { notifyChildrenOnKeyReleased(key); };
    get_all_element_names_ = [this]() -> std::vector<std::string> { return getAllElementNames(); };
    notify_main_window_element_deleted_ = [this](const std::string& element_name) -> void {
        elementDeleted(element_name);
    };
    notify_main_window_element_name_changed_ = [this](const std::string& old_name,
                                                      const std::string& new_name) -> void {
        elementNameChanged(old_name, new_name);
    };
    notify_main_window_about_modification_ = [this]() -> void { fileModified(); };

#ifdef PLATFORM_LINUX_M
    int argc = 1;
    char* argv[1] = {"noop"};
    glutInit(&argc, argv);
#endif

    if (configuration_agent_->hasKey("last_opened_file") &&
        dvs::filesystem::exists(configuration_agent_->readValue<std::string>("last_opened_file")))
    {
        save_manager_ = new SaveManager(configuration_agent_->readValue<std::string>("last_opened_file"));
    }
    else
    {
        save_manager_ = new SaveManager();
    }

    int visualization_period_ms = 10;
    if (configuration_agent_->hasKey("visualization_period_ms"))
    {
        visualization_period_ms = configuration_agent_->readValue<int>("visualization_period_ms");
        visualization_period_ms = std::max(1, std::min(100, visualization_period_ms));
    }

    current_window_num_ = 0;

    setupWindows(save_manager_->getCurrentProjectSettings());

    for (auto we : windows_)
    {
        windows_menu_->Append(we->getCallbackId(), we->getName());
        Bind(wxEVT_MENU, &MainWindow::toggleWindowVisibilityCallback, this, we->getCallbackId());
    }

    receive_thread_ = new std::thread(&MainWindow::receiveThreadFunction, this);

    receive_timer_.Bind(wxEVT_TIMER, &MainWindow::OnReceiveTimer, this);
    receive_timer_.Start(visualization_period_ms);

    // refresh_timer_.Bind(wxEVT_TIMER, &MainWindow::OnRefreshTimer, this);  // TODO: Remove?
    window_initialization_in_progress_ = false;
}

void MainWindow::exitApplication(wxCommandEvent& WXUNUSED(event))
{
    this->Destroy();
}

void MainWindow::elementNameChanged(const std::string& old_name, const std::string& new_name)
{
    for (auto& local_ge : gui_elements_)
    {
        if (local_ge.first == old_name)
        {
            GuiElement* ge = local_ge.second;
            gui_elements_.erase(local_ge.first);
            gui_elements_[new_name] = ge;
            break;
        }
    }
}

void MainWindow::elementDeleted(const std::string& element_name)
{
    if (gui_elements_.count(element_name) > 0)
    {
        gui_elements_.erase(element_name);
    }
}

wxMenuBar* MainWindow::createMainMenuBar()
{
    wxMenuBar* menu_bar_tmp = new wxMenuBar();

    // File Menu
    wxMenu* file_menu_ = new wxMenu();
    file_menu_->Append(wxID_NEW, _T("&New"));
    file_menu_->AppendSeparator();
    file_menu_->Append(wxID_OPEN, _T("&Open..."));
    file_menu_->AppendSeparator();
    file_menu_->Append(wxID_SAVE, _T("&Save"));
    file_menu_->Append(wxID_SAVEAS, _T("&Save As..."));

    file_menu_->AppendSeparator();
    file_menu_->Append(wxID_HELP_INDEX, _T("Quit"));

    menu_bar_tmp->Append(file_menu_, _T("&File"));

    windows_menu_ = new wxMenu();
    windows_menu_->Append(wxID_HELP_CONTENTS, "New window");
    windows_menu_->AppendSeparator();

    menu_bar_tmp->Append(windows_menu_, _T("&Windows"));

    return menu_bar_tmp;
}

bool MainWindow::hasWindowWithName(const std::string& window_name)
{
    auto q = std::find_if(windows_.begin(), windows_.end(), [&window_name](const WindowView* const w) -> bool {
        return window_name == w->getName();
    });
    return q != windows_.end();
}

void MainWindow::toggleWindowVisibilityCallback(wxCommandEvent& event)
{
    for (auto we : windows_)
    {
        if (we->getCallbackId() == event.GetId())
        {
            if (!we->IsShown())
            {
                we->Show();
                fileModified();
            }
            we->Raise();

            break;
        }
    }
}

void MainWindow::toggleWindowVisibility(const std::string& window_name)
{
    for (auto we : windows_)
    {
        if (we->getName() == window_name)
        {
            if (!(we->IsShown()))
            {
                we->Show();
            }

            we->Raise();
        }
    }
}

void MainWindow::setupWindows(const ProjectSettings& project_settings)
{
    for (const WindowSettings& ws : project_settings.getWindows())
    {
        windows_.emplace_back(new WindowView(this,
                                             ws,
                                             save_manager_->getCurrentFileName(),
                                             window_callback_id_,
                                             save_manager_->isSaved(),
                                             notification_from_gui_element_key_pressed_,
                                             notification_from_gui_element_key_released_,
                                             get_all_element_names_,
                                             notify_main_window_element_deleted_,
                                             notify_main_window_element_name_changed_,
                                             notify_main_window_about_modification_));
        window_callback_id_ += 1;
        current_window_num_++;
        task_bar_->addNewWindow(ws.name);
    }

    for (auto we : windows_)
    {
        std::vector<GuiElement*> ges = we->getGuiElements();
        for (const auto& ge : ges)
        {
            gui_elements_[ge->getName()] = ge;
        }
    }
}

std::vector<std::string> MainWindow::getAllElementNames() const
{
    std::vector<std::string> element_names;

    for (auto we : windows_)
    {
        const std::vector<std::string> window_element_names = we->getElementNames();

        element_names.insert(element_names.end(), window_element_names.begin(), window_element_names.end());
    }

    return element_names;
}

void MainWindow::newWindowCallback(wxCommandEvent& WXUNUSED(event))
{
    newWindow();
}

void MainWindow::newWindow()
{
    window_initialization_in_progress_ = true;
    newWindowWithoutFileModification();

    window_initialization_in_progress_ = false;

    fileModified();
}

void MainWindow::newWindowWithoutFileModification()
{
    WindowSettings window_settings;
    window_settings.name = "Window " + std::to_string(current_window_num_);
    window_settings.x = 30;
    window_settings.y = 30;
    window_settings.width = 600;
    window_settings.height = 628;

    WindowView* window_element = new WindowView(this,
                                                window_settings,
                                                save_manager_->getCurrentFileName(),
                                                window_callback_id_,
                                                save_manager_->isSaved(),
                                                notification_from_gui_element_key_pressed_,
                                                notification_from_gui_element_key_released_,
                                                get_all_element_names_,
                                                notify_main_window_element_deleted_,
                                                notify_main_window_element_name_changed_,
                                                notify_main_window_about_modification_);

    windows_menu_->Append(window_element->getCallbackId(), window_element->getName());
    Bind(wxEVT_MENU, &MainWindow::toggleWindowVisibilityCallback, this, window_element->getCallbackId());

    task_bar_->addNewWindow(window_settings.name);
    current_window_num_++;
    window_callback_id_ += 1;

    windows_.push_back(window_element);
}

MainWindow::~MainWindow()
{
    delete configuration_agent_;
}

void MainWindow::setIsFileSavedForAllWindows(const bool file_saved)
{
    for (auto we : windows_)
    {
        we->setIsFileSavedForLabel(file_saved);
    }
}

void MainWindow::fileModified()
{
    if (window_initialization_in_progress_)
    {
        return;
    }
    save_manager_->setIsModified();

    setIsFileSavedForAllWindows(false);
}

void MainWindow::saveProjectAsCallback(wxCommandEvent& WXUNUSED(event))
{
    saveProjectAs();
}

void MainWindow::saveProjectAs()
{
    wxFileDialog open_file_dialog(this, _("Choose file to save to"), "", "", "dvs files (*.dvs)|*.dvs", wxFD_SAVE);
    if (open_file_dialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    const std::string new_save_path = std::string(open_file_dialog.GetPath().mb_str());

    configuration_agent_->writeValue("last_opened_file", new_save_path);

    if (new_save_path == save_manager_->getCurrentFilePath())
    {
        saveProject();
        return;
    }

    ProjectSettings ps;

    for (const WindowView* we : windows_)
    {
        ps.pushBackWindowSettings(we->getWindowSettings());
    }

    save_manager_->saveToNewFile(new_save_path, ps);

    for (auto we : windows_)
    {
        we->setProjectName(save_manager_->getCurrentFileName());
    }
    setIsFileSavedForAllWindows(true);
}

void MainWindow::saveProject()
{
    if (!save_manager_->savePathIsSet())
    {
        saveProjectAs();
    }
    else
    {
        if (save_manager_->isSaved())
        {
            return;
        }
        ProjectSettings ps;

        for (const WindowView* we : windows_)
        {
            ps.pushBackWindowSettings(we->getWindowSettings());
        }

        configuration_agent_->writeValue("last_opened_file", save_manager_->getCurrentFilePath());

        save_manager_->save(ps);
        setIsFileSavedForAllWindows(true);
    }
}

void MainWindow::newProjectCallback(wxCommandEvent& WXUNUSED(event))
{
    newProject();
}

void MainWindow::newProject()
{
    window_initialization_in_progress_ = true;
    if (!save_manager_->isSaved())
    {
        if (wxMessageBox(_("Current content has not been saved! Proceed?"),
                         _("Please confirm"),
                         wxICON_QUESTION | wxYES_NO,
                         this) == wxNO)
        {
            return;
        }
    }

    removeAllWindows();

    save_manager_->reset();

    current_window_num_ = 0;
    newWindowWithoutFileModification();

    window_initialization_in_progress_ = false;
}

void MainWindow::removeAllWindows()
{
    gui_elements_.clear();

    for (auto we : windows_)
    {
        Unbind(wxEVT_MENU, &MainWindow::toggleWindowVisibilityCallback, this, we->getCallbackId());
        const int menu_id = windows_menu_->FindItem(we->getName());
        windows_menu_->Destroy(menu_id);

        we->deleteAllTabs();
        we->Destroy();
    }

    windows_.clear();
}

void MainWindow::saveProjectCallback(wxCommandEvent& WXUNUSED(event))
{
    saveProject();
}

void MainWindow::preferencesCallback(wxCommandEvent& WXUNUSED(event))
{
    preferences();
}

void MainWindow::preferences()
{
    std::cout << "Preferences!" << std::endl;
}

void MainWindow::openExistingFileCallback(wxCommandEvent& WXUNUSED(event))
{
    openExistingFile();
}

void MainWindow::openExistingFile(const std::string& file_path)
{
    window_initialization_in_progress_ = true;
    if (save_manager_->getCurrentFilePath() == file_path)
    {
        return;
    }

    removeAllWindows();

    configuration_agent_->writeValue("last_opened_file", file_path);

    save_manager_->openExistingFile(file_path);

    setupWindows(save_manager_->getCurrentProjectSettings());

    for (auto we : windows_)
    {
        windows_menu_->Append(we->getCallbackId(), we->getName());
        Bind(wxEVT_MENU, &MainWindow::toggleWindowVisibilityCallback, this, we->getCallbackId());
    }

    window_initialization_in_progress_ = false;
}

void MainWindow::openExistingFile()
{
    if (!save_manager_->isSaved())
    {
        if (wxMessageBox(_("Current content has not been saved! Proceed?"),
                         _("Please confirm"),
                         wxICON_QUESTION | wxYES_NO,
                         this) == wxNO)
        {
            return;
        }
    }

    wxFileDialog open_file_dialog(
        this, _("Open dvs file"), "", "", "dvs files (*.dvs)|*.dvs", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (open_file_dialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    openExistingFile(std::string(open_file_dialog.GetPath().mb_str()));
}

void MainWindow::OnRefreshTimer(wxTimerEvent&)
{
    refresh_timer_.Stop();

    SendSizeEvent();
    Refresh();
}

void MainWindow::notifyChildrenOnKeyPressed(const char key)
{
    for (auto& we : windows_)
    {
        we->notifyChildrenOnKeyPressed(key);
    }

    if ((wxGetKeyState(WXK_COMMAND) || wxGetKeyState(WXK_CONTROL)) &&
        (wxGetKeyState(static_cast<wxKeyCode>('s')) || wxGetKeyState(static_cast<wxKeyCode>('S'))))
    {
        saveProject();
    }
    else if ((wxGetKeyState(WXK_COMMAND) || wxGetKeyState(WXK_CONTROL)) &&
             (wxGetKeyState(static_cast<wxKeyCode>('q')) || wxGetKeyState(static_cast<wxKeyCode>('Q'))))
    {
        this->Destroy();
    }
}

void MainWindow::notifyChildrenOnKeyReleased(const char key)
{
    for (auto& we : windows_)
    {
        we->notifyChildrenOnKeyReleased(key);
    }
}

void MainWindow::deleteWindow(wxCommandEvent& event)
{
    auto q = std::find_if(windows_.begin(), windows_.end(), [&event](const WindowView* const w) -> bool {
        return w->getCallbackId() == (event.GetId() - 1);
    });

    if (q != windows_.end())
    {
        const std::vector<GuiElement*> ges = (*q)->getGuiElements();
        for (const auto& ge : ges)
        {
            gui_elements_.erase(ge->getName());
        }

        Unbind(wxEVT_MENU, &MainWindow::toggleWindowVisibilityCallback, this, (*q)->getCallbackId());
        const int menu_id = windows_menu_->FindItem((*q)->getName());
        windows_menu_->Destroy(menu_id);

        task_bar_->removeWindow((*q)->getName());
        delete (*q);
        windows_.erase(q);

        fileModified();
    }
}
