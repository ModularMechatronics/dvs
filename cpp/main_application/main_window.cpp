#include "main_window.h"

#include <unistd.h>
#include <wx/wxprec.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "layout_tools_window.h"
#include "events.h"
#include "math/math.h"

using namespace dvs::internal;

std::string getProjectFilePath()
{
    return "../../project_files/exp0.dvs.json";
}

MainWindow::~MainWindow() {}

void MainWindow::editingFinished(wxCommandEvent& event)
{
    disableEditing();
    layout_tools_window_->Hide();
}

void MainWindow::saveProject(wxCommandEvent& event)
{
    const std::string proj_file_path = getProjectFilePath();

    nlohmann::json j_to_save = project_file_.getJsonObject();
    j_to_save["tabs"] = nlohmann::json::array();

    for(auto te : tab_elements_)
    {
        nlohmann::json objects = nlohmann::json::array();

        const std::vector<ElementSettings> elems = te->getElementSettingsList();
        for(const auto elem : elems)
        {
            nlohmann::json j;
            j["x"] = elem.x;
            j["y"] = elem.y;
            j["width"] = elem.width;
            j["height"] = elem.height;
            j["name"] = elem.name;
            objects.push_back(j);
        }

        nlohmann::json tab_obj = nlohmann::json();
        tab_obj["name"] = te->getName();
        tab_obj["elements"] = objects;

        j_to_save["tabs"].push_back(tab_obj);
        
    }

    std::ofstream output_file(proj_file_path);
    output_file << std::setw(4) << j_to_save << std::endl;
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
}

void MainWindow::disableEditing()
{
    edit_layout_menu_option_->SetName("Edit layout");
    is_editing_ = false;
    for(auto te : tab_elements_)
    {
        te->stopEdit();
    }
    toolbar_->Hide();
}

void MainWindow::deleteTab(wxCommandEvent& event)
{
    const int current_tab_idx = tabs_view->GetSelection();
    if(current_tab_idx != wxNOT_FOUND)
    {
        tabs_view->DeletePage(current_tab_idx);
        tab_elements_.erase(tab_elements_.begin() + current_tab_idx);
    }
}

void MainWindow::toggleEditLayout(wxCommandEvent& event)
{
    if(is_editing_)
    {
        edit_layout_menu_option_->SetName("Edit layout");
        layout_tools_window_->Hide();
        toolbar_->Hide();
        for(auto te : tab_elements_)
        {
            te->stopEdit();
        }
    }
    else
    {
        edit_layout_menu_option_->SetName("Stop editing");
        layout_tools_window_->Show();
        toolbar_->Show();
        for(auto te : tab_elements_)
        {
            te->startEdit();
        }
    }
    is_editing_ = !is_editing_;
}

/*void MainWindow::onRightClickMenu(wxCommandEvent& event)
{
    std::cout << "Menu!" << std::endl;
    wxTextEntryDialog* wx_te_dialog = new wxTextEntryDialog(this, "Change name", "Enter a new tab name", "<old-name>", wxOK | wxCANCEL | wxCENTRE);
    
    if ( wx_te_dialog->ShowModal() == wxID_OK )
    {
        const wxString value = wx_te_dialog->GetValue();
        std::cout << std::string(value.mb_str()) << std::endl;
    }
}

void MainWindow::onShowContextMenu(wxContextMenuEvent& event)
{
    std::cout << event.GetString() << std::endl;
    wxMenu menu;

    menu.Append(MENU_ID_CONTEXT_1, "Context Menu command 1");
    menu.Append(MENU_ID_CONTEXT_2, "Context Menu command 2");
    menu.Append(MENU_ID_CONTEXT_3, "Context Menu command 3");
    
    PopupMenu(&menu);        
}*/

MainWindow::MainWindow(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxPoint(0, 30), wxSize(1500, 700)), project_file_(getProjectFilePath())
{
    udp_server_ = new UdpServer(9752);
    udp_server_->start();
    // current_gui_element_ = nullptr;
    current_gui_element_set_ = false;
    is_editing_ = false;

    // Bind(wxEVT_CONTEXT_MENU, &MainWindow::onShowContextMenu, this);
    // Bind(wxEVT_COMMAND_MENU_SELECTED, &MainWindow::onRightClickMenu, this, MENU_ID_CONTEXT_1, MENU_ID_CONTEXT_3);

    save_manager_ = new SaveManager(getProjectFilePath());

    wxMenuBar* m_pMenuBar = new wxMenuBar();
    // File Menu
    wxMenu* m_pFileMenu = new wxMenu();
    m_pFileMenu->Append(wxID_NEW, _T("&New"));
    m_pFileMenu->Append(wxID_OPEN, _T("&Open..."));
    m_pFileMenu->Append(wxID_SAVE, _T("&Save"));
    m_pFileMenu->Append(wxID_SAVE, _T("&Save As..."));
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

    Bind(wxEVT_MENU, &MainWindow::saveProject, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MainWindow::toggleEditLayout, this, dvs_ids::EDIT_LAYOUT);

    SetMenuBar(m_pMenuBar);
    // wxMenuBar::MacSetCommonMenuBar(m_pMenuBar);

    current_tab_num_ = 0;

    wxImage::AddHandler(new wxPNGHandler);

    tb_edit = wxBitmap(wxT("../icons/edit.png"), wxBITMAP_TYPE_PNG);
    tb_delete = wxBitmap(wxT("../icons/delete.png"), wxBITMAP_TYPE_PNG);
    tb_done = wxBitmap(wxT("../icons/done2.png"), wxBITMAP_TYPE_PNG);
    tb_add = wxBitmap(wxT("../icons/add.png"), wxBITMAP_TYPE_PNG);

    toolbar_ = CreateToolBar();

    toolbar_->AddTool(dvs_ids::EDITING_DONE, wxT("Done"), tb_done);
    toolbar_->AddTool(dvs_ids::DELETE_TAB, wxT("Delete current tab"), tb_delete);
    toolbar_->AddTool(dvs_ids::ADD_TAB, wxT("New tab"), tb_add);
    toolbar_->Realize();
    toolbar_->Hide();

    Connect(dvs_ids::EDITING_DONE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::editingFinished));
    Connect(dvs_ids::DELETE_TAB, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::deleteTab));
    Connect(dvs_ids::ADD_TAB, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::addNewTab));

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
