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

using namespace dvs::internal;

MainWindow::MainWindow(const std::vector<std::string>& cmdl_args)
    : wxFrame(NULL, wxID_ANY, "", wxPoint(30, 30), wxSize(50, 50))
{
    static_cast<void>(cmdl_args);
    udp_server_ = new UdpServer(dvs::internal::kUdpPortNum);
    udp_server_->start();
    current_gui_element_ = nullptr;
    current_gui_element_set_ = false;
    window_callback_id_ = dvs_ids::WINDOW_TOGGLE;

    configuration_agent_ = new ConfigurationAgent();
    main_window_last_in_focus_ = true;

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
    task_bar_->setOnMenuEdit([this]() -> void {});
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
    Bind(CHILD_WINDOW_IN_FOCUS_EVENT, &MainWindow::childWindowInFocus, this, wxID_ANY);

    current_tab_num_ = 0;

    setupGui();

    for (auto we : windows_)
    {
        // m_pWindowsMenu->Append(we->getCallbackId(), we->getName());
        Bind(wxEVT_MENU, &MainWindow::toggleWindowVisibilityCallback, this, we->getCallbackId());
    }

    timer_.Bind(wxEVT_TIMER, &MainWindow::OnTimer, this);
    timer_.Start(10);

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
                                             notification_from_gui_element_key_released_));
        window_callback_id_++;
        const std::map<std::string, GuiElement*> ges = windows_.back()->getGuiElements();
        gui_elements_.insert(ges.begin(), ges.end());
        task_bar_->addNewWindow(ws.getName());
    }
}

MainWindow::~MainWindow()
{
    delete configuration_agent_;
}

void MainWindow::saveProjectAsCallback(wxCommandEvent& WXUNUSED(event))
{
    // saveProjectAs();
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
    // newProject();
}

void MainWindow::newProject()
{
    // if (!save_manager_->isSaved())
    // {
    //     if (wxMessageBox(_("Current content has not been saved! Proceed?"),
    //                      _("Please confirm"),
    //                      wxICON_QUESTION | wxYES_NO,
    //                      this) == wxNO)
    //     {
    //         return;
    //     }
    // }
    // Unbind(wxEVT_ACTIVATE, &MainWindow::onActivate, this);

    // current_tab_name_ = "";
    // current_element_name_ = "";

    // save_manager_->reset();

    // /*tabs_ = std::vector<TabView*>();
    // tabs_view->DeleteAllPages();
    // tabs_view->Destroy();*/

    // for (auto we : windows_)
    // {
    //     we->Destroy();
    // }
    // windows_.clear();

    // // tabs_view = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(500, 500));
    // // tabs_view->Layout();

    // // tabs_sizer_v->Add(tabs_view, 1, wxEXPAND);

    // Bind(wxEVT_ACTIVATE, &MainWindow::onActivate, this);

    // setupWindows(save_manager_->getCurrentProjectSettings());

    // SendSizeEvent();
    // Refresh();
}

void MainWindow::saveProjectCallback(wxCommandEvent& WXUNUSED(event))
{
    // saveProject();
}

void MainWindow::addNewWindowCallback(wxCommandEvent& WXUNUSED(event))
{
    // const std::string window_name = "new-window-" + std::to_string(current_tab_num_);
    // current_tab_num_++;

    // addNewWindow(window_name);
    // task_bar_->addNewWindow(window_name);
}

void MainWindow::addNewWindow(const std::string& window_name)
{
    // WindowSettings window_settings;
    // window_settings.setName(window_name);
    // window_settings.x = 30;
    // window_settings.y = 30;
    // window_settings.width = 600;
    // window_settings.height = 628;
    // WindowView* window_element = new WindowView(this,
    //                                             window_settings,
    //                                             window_callback_id_,
    //                                             notification_from_gui_element_key_pressed_,
    //                                             notification_from_gui_element_key_released_);
    // window_callback_id_++;
    // windows_.push_back(window_element);

    // m_pWindowsMenu->Append(window_element->getCallbackId(), window_element->getName());
    // Bind(wxEVT_MENU, &MainWindow::toggleWindowVisibilityCallback, this, window_element->getCallbackId());

    // main_window_last_in_focus_ = false;
    // /*for (auto te : tabs_)
    // {
    //     te->resetSelectionForAllChildren();
    // }*/

    // for (auto we : windows_)
    // {
    //     we->resetSelectionForAllChildren();
    // }

    // current_tab_name_ = window_element->getName();

    // fileModified();
}

void MainWindow::deleteWindow()
{
    // int window_idx = 0;
    // if (!main_window_last_in_focus_)
    // {
    //     for (auto we : windows_)
    //     {
    //         if (we->getName() == current_tab_name_)
    //         {
    //             Unbind(wxEVT_MENU, &MainWindow::toggleWindowVisibilityCallback, this, we->getCallbackId());
    //             task_bar_->removeWindow(current_tab_name_);

    //             const int menu_id = m_pWindowsMenu->FindItem(we->getName());
    //             m_pWindowsMenu->Destroy(menu_id);

    //             /*const std::map<std::string, GuiElement*> window_gui_elements = we->getGuiElements();
    //             for (const auto& q : window_gui_elements)
    //             {
    //                 gui_elements_.erase(q.first);
    //             }
    //             we->Destroy();

    //             main_window_last_in_focus_ = true;
    //             break;*/
    //         }

    //         window_idx++;
    //     }
    //     windows_.erase(windows_.begin() + window_idx);
    // }
    // /*const int current_tab_idx = tabs_view->GetSelection();
    // if(current_tab_idx != wxNOT_FOUND)
    // {

    //     const std::map<std::string, GuiElement*> tab_gui_elements = tabs_.at(current_tab_idx)->getGuiElements();

    //     for(const auto& q : tab_gui_elements)
    //     {
    //         gui_elements_.erase(q.first);
    //     }

    //     tabs_view->DeletePage(current_tab_idx);
    //     tabs_.erase(tabs_.begin() + current_tab_idx);
    // }

    // fileModified();*/
}

void MainWindow::guiElementModified(wxCommandEvent& WXUNUSED(event))
{
    // fileModified();
}

void MainWindow::noElementSelected(wxCommandEvent& WXUNUSED(event)) {}

void MainWindow::childWindowClosed(wxCommandEvent& WXUNUSED(event)) {}

void MainWindow::childWindowInFocus(wxCommandEvent& event)
{
    // main_window_last_in_focus_ = false;
    // /*for (auto te : tabs_)
    // {
    //     te->resetSelectionForAllChildren();
    // }*/

    // for (auto we : windows_)
    // {
    //     we->resetSelectionForAllChildren();
    //     if (we->getCallbackId() == event.GetId())
    //     {
    //         current_tab_name_ = we->getName();
    //     }
    // }
}

void MainWindow::preferencesCallback(wxCommandEvent& WXUNUSED(event))
{
    // preferences();
}

void MainWindow::preferences()
{
    std::cout << "Preferences!" << std::endl;
}

wxMenuBar* MainWindow::createMainMenuBar()
{
    wxMenuBar* menu_bar_tmp = new wxMenuBar();

    // // File Menu
    wxMenu* m_pFileMenu = new wxMenu();
    m_pFileMenu->Append(wxID_NEW, _T("&New"));
    m_pFileMenu->Append(wxID_OPEN, _T("&Open..."));
    m_pFileMenu->Append(wxID_SAVE, _T("&Save"));
    m_pFileMenu->Append(wxID_SAVEAS, _T("&Save As..."));
    m_pFileMenu->AppendSeparator();
    m_pFileMenu->Append(wxID_EXIT, _T("&Quit"));
    menu_bar_tmp->Append(m_pFileMenu, _T("&File"));

    wxMenu* m_edit_menu = new wxMenu();

    edit_layout_menu_option_ = m_edit_menu->Append(dvs_ids::EDIT_LAYOUT, _T("Edit layout"));
    edit_layout_menu_option_ = m_edit_menu->Append(dvs_ids::PREFERENCES, _T("Preferences"));
    menu_bar_tmp->Append(m_edit_menu, _T("Edit"));

    m_pWindowsMenu = new wxMenu();
    m_pWindowsMenu->Append(dvs_ids::SHOW_MAIN_WINDOW, "Main window");
    m_pWindowsMenu->AppendSeparator();
    menu_bar_tmp->Append(m_pWindowsMenu, _T("&Windows"));

    wxMenu* m_pHelpMenu = new wxMenu();
    m_pHelpMenu->Append(wxID_ABOUT, _T("&About"));
    menu_bar_tmp->Append(m_pHelpMenu, _T("&Help"));

    return menu_bar_tmp;
}

void MainWindow::openExistingFileCallback(wxCommandEvent& WXUNUSED(event))
{
    // openExistingFile();
}

void MainWindow::openExistingFile()
{
    // if (!save_manager_->isSaved())
    // {
    //     if (wxMessageBox(_("Current content has not been saved! Proceed?"),
    //                      _("Please confirm"),
    //                      wxICON_QUESTION | wxYES_NO,
    //                      this) == wxNO)
    //     {
    //         return;
    //     }
    // }
    // Unbind(wxEVT_ACTIVATE, &MainWindow::onActivate, this);

    // wxFileDialog openFileDialog(
    //     this, _("Open dvs file"), "", "", "dvs files (*.dvs)|*.dvs", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    // if (openFileDialog.ShowModal() == wxID_CANCEL)
    // {
    //     return;
    // }

    // if (save_manager_->getCurrentFilePath() == std::string(openFileDialog.GetPath().mb_str()))
    // {
    //     return;
    // }

    // configuration_agent_->writeValue("last_opened_file", std::string(openFileDialog.GetPath().mb_str()));

    // save_manager_->openExistingFile(std::string(openFileDialog.GetPath().mb_str()));

    // /*tabs_ = std::vector<TabView*>();
    // tabs_view->DeleteAllPages();
    // tabs_view->Destroy();*/

    // for (auto we : windows_)
    // {
    //     we->Destroy();
    // }
    // windows_.clear();

    // // tabs_view = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(500, 500));
    // // tabs_view->Layout();

    // // tabs_sizer_v->Add(tabs_view, 1, wxEXPAND);

    // Bind(wxEVT_ACTIVATE, &MainWindow::onActivate, this);

    // setupWindows(save_manager_->getCurrentProjectSettings());

    // refresh_timer_.Start(10);

    // SendSizeEvent();
    // Refresh();
}

void MainWindow::OnRefreshTimer(wxTimerEvent&)
{
    refresh_timer_.Stop();
    // tabs_view->Refresh();

    SendSizeEvent();
    Refresh();
}

void MainWindow::OnTimer(wxTimerEvent&)
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
    std::map<std::string, GuiElement*>::iterator it;

    for (it = gui_elements_.begin(); it != gui_elements_.end(); it++)
    {
        it->second->keyPressed(key);
    }
}

void MainWindow::notifyChildrenOnKeyReleased(const char key)
{
    std::cout << "Key released: " << key << std::endl;
    // // std::cout << "Key released: " << key << std::endl;
    // if ((key == 'E') || (key == 'e'))
    // {
    //     return;
    // }
    std::map<std::string, GuiElement*>::iterator it;

    for (it = gui_elements_.begin(); it != gui_elements_.end(); it++)
    {
        it->second->keyReleased(key);
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
