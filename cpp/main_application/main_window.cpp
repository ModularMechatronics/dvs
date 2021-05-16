#include "main_window.h"

#include <unistd.h>
#include <wx/wxprec.h>
#include <wx/wfstream.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "layout_tools_window.h"
#include "events.h"
#include "math/math.h"

using namespace dvs::internal;

std::string getProjectFilePath()
{
    return "/Users/annotelldaniel/work/repos/dvs/project_files/exp0.dvs.json";
}

MainWindow::MainWindow(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxPoint(0, 30), wxSize(1500, 700))
{
    udp_server_ = new UdpServer(9752);
    udp_server_->start();
    // current_gui_element_ = nullptr;
    current_gui_element_set_ = false;
    is_editing_ = false;

    save_manager_ = new SaveManager(getProjectFilePath());

    SetLabel(save_manager_->getCurrentFileName());
    Bind(GUI_ELEMENT_CHANGED_EVENT, &MainWindow::guiElementModified, this, wxID_ANY);

    wxMenuBar* m_pMenuBar = new wxMenuBar();
    // File Menu
    wxMenu* m_pFileMenu = new wxMenu();
    m_pFileMenu->Append(wxID_NEW, _T("&New"));
    m_pFileMenu->Append(wxID_OPEN, _T("&Open..."));
    m_pFileMenu->Append(wxID_SAVE, _T("&Save"));
    m_pFileMenu->Append(wxID_SAVEAS, _T("&Save As..."));
    m_pFileMenu->AppendSeparator();
    m_pFileMenu->Append(wxID_EXIT, _T("&Quit"));
    m_pMenuBar->Append(m_pFileMenu, _T("&File"));

    wxMenu* m_edit_menu = new wxMenu();

    edit_layout_menu_option_ = m_edit_menu->Append(dvs_ids::EDIT_LAYOUT, _T("Edit layout"));
    m_pMenuBar->Append(m_edit_menu, _T("Edit"));
    // About menu
    wxMenu* m_pHelpMenu = new wxMenu();
    m_pHelpMenu->Append(wxID_ABOUT, _T("&About"));
    m_pMenuBar->Append(m_pHelpMenu, _T("&Help"));

    Bind(wxEVT_MENU, &MainWindow::newProjectCallback, this, wxID_NEW);
    Bind(wxEVT_MENU, &MainWindow::saveProjectCallback, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MainWindow::toggleEditLayout, this, dvs_ids::EDIT_LAYOUT);
    Bind(wxEVT_MENU, &MainWindow::openExistingFile, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainWindow::saveProjectAsCallback, this, wxID_SAVEAS);

    SetMenuBar(m_pMenuBar);
    // wxMenuBar::MacSetCommonMenuBar(m_pMenuBar);

    current_tab_num_ = 0;

    wxImage::AddHandler(new wxPNGHandler);

    initial_width_ = 1500;
    initial_height_ = 700;

    layout_tools_window_ = new LayoutToolsWindow(this, wxPoint(1500, 30), wxSize(300, 300));
    layout_tools_window_->Hide();

    Bind(MY_EVENT, &MainWindow::currentElementSelectionChanged, this);

    setupGui();

    Bind(wxEVT_SIZE, &MainWindow::OnSize, this);

    timer_.Bind(wxEVT_TIMER, &MainWindow::OnTimer, this);
    timer_.Start(10);
}

MainWindow::~MainWindow() {}

void MainWindow::editingFinished(wxCommandEvent& event)
{
    disableEditing();
    layout_tools_window_->Hide();
}

void MainWindow::saveProjectAsCallback(wxCommandEvent& event)
{
    saveProjectAs();
}

void MainWindow::saveProjectAs()
{
    wxFileDialog openFileDialog(this, _("Choose file to save to"), "", "",
                       "dvs.json files (*.json)|*.json", wxFD_SAVE);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    const std::string new_save_path = std::string(openFileDialog.GetPath().mb_str());

    if(new_save_path == save_manager_->getCurrentFilePath())
    {
        saveProject();
        return;
    }

    ProjectFile pf;
    for(const TabView* te : tab_elements_)
    {
        pf.pushBackTabSettings(te->getTabSettings());
    }

    save_manager_->saveToNewFile(new_save_path, pf);
    SetLabel(save_manager_->getCurrentFileName());
}

void MainWindow::saveProject()
{
    if(!save_manager_->pathIsSet())
    {
        saveProjectAs();
    }
    else
    {
        ProjectFile pf;
        for(const TabView* te : tab_elements_)
        {
            pf.pushBackTabSettings(te->getTabSettings());
        }

        SetLabel(save_manager_->getCurrentFileName());

        save_manager_->save(pf);
    }
}

void MainWindow::newProjectCallback(wxCommandEvent& event)
{
    if (!save_manager_->isSaved())
    {
        if (wxMessageBox(_("Current content has not been saved! Proceed?"), _("Please confirm"),
                         wxICON_QUESTION | wxYES_NO, this) == wxNO )
        {
            return;
        }
    }

    save_manager_->reset();

    if(is_editing_)
    {
        disableEditing();
        layout_tools_window_->Hide();
    }

    SetLabel(save_manager_->getCurrentFileName());

    tabs_view->DeleteAllPages();
    tabs_view->Destroy();

    tabs_view = new wxNotebook(tab_container, wxID_ANY, wxDefaultPosition, wxSize(500, 500));
    tabs_view->Layout();

    tab_elements_ = std::vector<TabView*>();
    tabs_sizer_v->Add(tabs_view, 1, wxEXPAND);

    setupTabs(save_manager_->getCurrentProjectFile());

    SendSizeEvent();
    Refresh();
}

void MainWindow::saveProjectCallback(wxCommandEvent& event)
{
    saveProject();
}

void MainWindow::addNewTab(wxCommandEvent& event)
{
    const std::string tab_name = "New tab " + std::to_string(current_tab_num_);
    TabSettings tab;
    tab.setName(tab_name);
    TabView* tab_element = new TabView(tabs_view, tab);
    tab_elements_.push_back(tab_element);

    tabs_view->AddPage(dynamic_cast<wxNotebookPage*>(tab_element), tab_name);

    if(is_editing_)
    {
        tab_element->startEdit();
    }

    current_tab_num_++;
    fileModified();
}

void MainWindow::disableEditing()
{
    edit_layout_menu_option_->SetName("Edit layout");
    is_editing_ = false;
    for(auto te : tab_elements_)
    {
        te->stopEdit();
    }
}

void MainWindow::guiElementModified(wxCommandEvent& event)
{
    fileModified();
}

void MainWindow::deleteTab(wxCommandEvent& event)
{
    const int current_tab_idx = tabs_view->GetSelection();
    if(current_tab_idx != wxNOT_FOUND)
    {
        tabs_view->DeletePage(current_tab_idx);
        tab_elements_.erase(tab_elements_.begin() + current_tab_idx);
    }

    fileModified();
}

void MainWindow::toggleEditLayout(wxCommandEvent& event)
{
    if(is_editing_)
    {
        edit_layout_menu_option_->SetName("Edit layout");
        layout_tools_window_->Hide();
        for(auto te : tab_elements_)
        {
            te->stopEdit();
        }
    }
    else
    {
        edit_layout_menu_option_->SetName("Stop editing");
        layout_tools_window_->Show();
        for(auto te : tab_elements_)
        {
            te->startEdit();
        }
    }
    is_editing_ = !is_editing_;
}

void MainWindow::openExistingFile(wxCommandEvent& event)
{
    if (!save_manager_->isSaved())
    {
        if (wxMessageBox(_("Current content has not been saved! Proceed?"), _("Please confirm"),
                         wxICON_QUESTION | wxYES_NO, this) == wxNO )
        {
            return;
        }
    }

    wxFileDialog openFileDialog(this, _("Open dvs.json file"), "", "",
                       "dvs.json files (*.json)|*.json", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    if(save_manager_->getCurrentFilePath() == std::string(openFileDialog.GetPath().mb_str()))
    {
        return;
    }

    if(is_editing_)
    {
        disableEditing();
        layout_tools_window_->Hide();
    }

    save_manager_->openExistingFile(std::string(openFileDialog.GetPath().mb_str()));
    SetLabel(save_manager_->getCurrentFileName());

    tabs_view->DeleteAllPages();
    tabs_view->Destroy();

    tabs_view = new wxNotebook(tab_container, wxID_ANY, wxDefaultPosition, wxSize(500, 500));
    tabs_view->Layout();

    tab_elements_ = std::vector<TabView*>();
    tabs_sizer_v->Add(tabs_view, 1, wxEXPAND);

    setupTabs(save_manager_->getCurrentProjectFile());

    SendSizeEvent();
    Refresh();
}

void MainWindow::OnTimer(wxTimerEvent&)
{
    try
    {
        receiveData();
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << "Got runtime_error when receiving: " << e.what() << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Got exception when receiving: " << e.what() << std::endl;
    }
}

void MainWindow::OnClose(wxCloseEvent& event)
{
    (void)event;
    if (wxMessageBox("Are you sure you want to exit?",
                     "Please confirm",
                     wxICON_QUESTION | wxYES_NO) != wxYES)
    {
        return;
    }

    std::cout << "Window close" << std::endl;
    Destroy();
}
