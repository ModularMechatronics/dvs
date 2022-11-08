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
    udp_server_ = new UdpServer(dvs::internal::kUdpPortNum);
    udp_server_->start();
    current_gui_element_ = nullptr;
    window_callback_id_ = dvs_ids::WINDOW_TOGGLE;

    configuration_agent_ = new ConfigurationAgent();

    task_bar_ = new CustomTaskBarIcon();
    wxIcon* icon = new wxIcon();
    icon->LoadFile("../resources/images/apple.ico", wxBITMAP_TYPE_ICO, 32, 32);

    if (!task_bar_->SetIcon(*icon, "wxTaskBarIcon Sample\n"))
    {
        wxLogError("Could not set icon.");
    }

    task_bar_->setOnMenuExitCallback([this]() -> void { this->Destroy(); });
    task_bar_->setOnMenuFileNew([this]() -> void { newProject(); });
    task_bar_->setOnMenuFileOpen([this]() -> void { openExistingFile(); });
    task_bar_->setOnMenuFileSave([this]() -> void { saveProject(); });
    task_bar_->setOnMenuFileSaveAs([this]() -> void { saveProjectAs(); });
    task_bar_->setOnMenuSubWindow(
        [this](const std::string& window_name) -> void { toggleWindowVisibility(window_name); });
    task_bar_->setOnMenuPreferences([this]() -> void { preferences(); });

    notification_from_gui_element_key_pressed_ = [this](const char key) { notifyChildrenOnKeyPressed(key); };
    notification_from_gui_element_key_released_ = [this](const char key) { notifyChildrenOnKeyReleased(key); };

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

    const RGBTripletf color_vec{AxesSettings().window_background_};
    SetBackgroundColour(wxColour(color_vec.red * 255.0f, color_vec.green * 255.0f, color_vec.blue * 255.0f));

    Bind(GUI_ELEMENT_CHANGED_EVENT, &MainWindow::guiElementModified, this, wxID_ANY);
    Bind(NO_ELEMENT_SELECTED, &MainWindow::noElementSelected, this, wxID_ANY);
    Bind(CHILD_WINDOW_CLOSED_EVENT, &MainWindow::childWindowClosed, this, wxID_ANY);
    Bind(wxEVT_CLOSE_WINDOW, &MainWindow::onCloseButton, this, wxID_ANY);

    current_window_num_ = 0;

    setupGui();

    for (auto we : windows_)
    {
        // m_pWindowsMenu->Append(we->getCallbackId(), we->getName());
        Bind(wxEVT_MENU, &MainWindow::toggleWindowVisibilityCallback, this, we->getCallbackId());
    }

    receive_timer_.Bind(wxEVT_TIMER, &MainWindow::OnReceiveTimer, this);
    receive_timer_.Start(10);

    refresh_timer_.Bind(wxEVT_TIMER, &MainWindow::OnRefreshTimer, this);
}

void MainWindow::toggleWindowVisibility(const std::string& window_name)
{
    // for (auto we : windows_)
    // {
    //     we->resetSelectionForAllChildren();
    //     if (we->getName() == window_name)
    //     {
    //         we->setFirstElementSelected();
    //         current_tab_name_ = we->getName();
    //         we->Hide();
    //         we->show();
    //     }
    // }
}

void MainWindow::toggleWindowVisibilityCallback(wxCommandEvent& event)
{
    /*for (auto te : tabs_)
    {
        te->resetSelectionForAllChildren();
    }*/

    // for (auto we : windows_)
    // {
    //     we->resetSelectionForAllChildren();
    //     if (we->getCallbackId() == event.GetId())
    //     {
    //         we->setFirstElementSelected();
    //         current_tab_name_ = we->getName();
    //         we->Hide();
    //         we->show();
    //     }
    // }
}

void MainWindow::setupWindows(const ProjectSettings& project_settings)
{
    for (const WindowSettings& ws : project_settings.getWindows())
    {
        windows_.emplace_back(new WindowView(this,
                                             ws,
                                             window_callback_id_,
                                             notification_from_gui_element_key_pressed_,
                                             notification_from_gui_element_key_released_,
                                             [this]() -> std::vector<std::string> { return getAllElementNames(); }));
        window_callback_id_ += 20;
        current_window_num_++;
        task_bar_->addNewWindow(ws.name);
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

void MainWindow::newWindow(wxCommandEvent& WXUNUSED(event))
{
    WindowSettings window_settings;
    window_settings.name = "Window " + std::to_string(current_window_num_);
    window_settings.x = 30;
    window_settings.y = 30;
    window_settings.width = 600;
    window_settings.height = 628;

    WindowView* window_element = new WindowView(this,
                                                window_settings,
                                                window_callback_id_,
                                                notification_from_gui_element_key_pressed_,
                                                notification_from_gui_element_key_released_,
                                                [this]() -> std::vector<std::string> { return getAllElementNames(); });

    current_window_num_++;
    window_callback_id_++;

    windows_.push_back(window_element);

    Bind(wxEVT_MENU, &MainWindow::toggleWindowVisibilityCallback, this, window_element->getCallbackId());

    fileModified();
}

MainWindow::~MainWindow()
{
    delete configuration_agent_;
}

void MainWindow::saveProjectAsCallback(wxCommandEvent& WXUNUSED(event))
{
    saveProjectAs();
}

void MainWindow::saveProjectAs()
{
    wxFileDialog openFileDialog(this, _("Choose file to save to"), "", "", "dvs files (*.dvs)|*.dvs", wxFD_SAVE);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    const std::string new_save_path = std::string(openFileDialog.GetPath().mb_str());

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
}

void MainWindow::saveProject()
{
    if (!save_manager_->pathIsSet())
    {
        saveProjectAs();
    }
    else
    {
        ProjectSettings ps;

        for (const WindowView* we : windows_)
        {
            ps.pushBackWindowSettings(we->getWindowSettings());
        }

        configuration_agent_->writeValue("last_opened_file", save_manager_->getCurrentFilePath());

        save_manager_->save(ps);
    }
}

void MainWindow::newProjectCallback(wxCommandEvent& WXUNUSED(event))
{
    newProject();
}

void MainWindow::newProject()
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

    // current_tab_name_ = "";

    save_manager_->reset();

    for (auto we : windows_)
    {
        we->Destroy();
    }
    windows_.clear();

    setupWindows(save_manager_->getCurrentProjectSettings());

    SendSizeEvent();
    Refresh();
}

void MainWindow::saveProjectCallback(wxCommandEvent& WXUNUSED(event))
{
    saveProject();
}

void MainWindow::guiElementModified(wxCommandEvent& WXUNUSED(event))
{
    // fileModified();
}

void MainWindow::noElementSelected(wxCommandEvent& WXUNUSED(event)) {}

void MainWindow::childWindowClosed(wxCommandEvent& WXUNUSED(event)) {}

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

    wxFileDialog openFileDialog(
        this, _("Open dvs file"), "", "", "dvs files (*.dvs)|*.dvs", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    if (save_manager_->getCurrentFilePath() == std::string(openFileDialog.GetPath().mb_str()))
    {
        return;
    }

    configuration_agent_->writeValue("last_opened_file", std::string(openFileDialog.GetPath().mb_str()));

    save_manager_->openExistingFile(std::string(openFileDialog.GetPath().mb_str()));

    for (auto we : windows_)
    {
        we->Destroy();
    }
    windows_.clear();

    setupWindows(save_manager_->getCurrentProjectSettings());

    // refresh_timer_.Start(10);

    SendSizeEvent();
    Refresh();
}

void MainWindow::OnRefreshTimer(wxTimerEvent&)
{
    refresh_timer_.Stop();

    SendSizeEvent();
    Refresh();
}

void MainWindow::OnReceiveTimer(wxTimerEvent&)
{
    try
    {
        receiveData();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Got runtime_error when receiving: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Got exception when receiving: " << e.what() << std::endl;
    }
}

void MainWindow::notifyChildrenOnKeyPressed(const char key)
{
    std::cout << "Key pressed: " << key << std::endl;

    for (auto& we : windows_)
    {
        we->notifyChildrenOnKeyPressed(key);
    }
}

void MainWindow::notifyChildrenOnKeyReleased(const char key)
{
    std::cout << "Key released: " << key << std::endl;

    for (auto& we : windows_)
    {
        we->notifyChildrenOnKeyReleased(key);
    }
}

void MainWindow::onCloseButton(wxCloseEvent& WXUNUSED(event))
{
    this->Hide();
}

void MainWindow::OnClose(wxCloseEvent& WXUNUSED(event))
{
    if (wxMessageBox("Are you sure you want to exit?", "Please confirm", wxICON_QUESTION | wxYES_NO) != wxYES)
    {
        return;
    }

    std::cout << "Window close" << std::endl;
    // Destroy();
}

void MainWindow::deleteWindow(wxCommandEvent& event)
{
    auto q = std::find_if(windows_.begin(), windows_.end(), [&event](const WindowView* const w) -> bool {
        return w->getCallbackId() == (event.GetId() - 1);
    });

    if (q != windows_.end())
    {
        std::cout << "Erasing element at " << std::distance(windows_.begin(), q) << std::endl;
        std::cout << "Name: " << (*q)->getName() << std::endl;
        delete (*q);
        windows_.erase(q);
    }
}
