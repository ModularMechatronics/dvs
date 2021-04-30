#include "main_window.h"

#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "math/math.h"

using namespace dvs::internal;

std::string getProjectFilePath()
{
    return "../../project_files/exp0.dvs.json";
}

MainWindow::~MainWindow()
{
    std::map<std::string, GuiElement*>::iterator it;

    for (it = gui_elements_.begin(); it != gui_elements_.end(); it++)
    {
        delete it->second;
    }
}

void MainWindow::addNewTab(wxCommandEvent& event)
{
    // tabs_view->AddPage( new wxNotebookPage(tabs_view, -1), L"TEST 4");
    std::cout << "New tab!" << std::endl;
}

void MainWindow::deleteTab(wxCommandEvent& event)
{
    std::cout << "Delete tab!" << std::endl;
}

void MainWindow::editLayout(wxCommandEvent& event)
{
    std::cout << "Edit tab!" << std::endl;
}

MainWindow::MainWindow(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxPoint(0, 30), wxSize(1500, 700)), project_file_(getProjectFilePath())
{
    udp_server_ = new UdpServer(9752);
    udp_server_->start();
    current_gui_element_ = nullptr;
    current_gui_element_set_ = false;

    wxImage::AddHandler(new wxPNGHandler);

    wxBitmap tb_edit(wxT("../icons/edit.png"), wxBITMAP_TYPE_PNG);
    wxBitmap tb_delete(wxT("../icons/delete.png"), wxBITMAP_TYPE_PNG);
    wxBitmap tb_done(wxT("../icons/done.png"), wxBITMAP_TYPE_PNG);
    wxBitmap tb_add(wxT("../icons/add.png"), wxBITMAP_TYPE_PNG);

    wxToolBar *toolbar = CreateToolBar();
    toolbar->AddTool(wxID_HIGHEST + 1, wxT("Edit layout"), tb_edit);
    toolbar->AddTool(wxID_HIGHEST + 2, wxT("Delete current tab"), tb_delete);
    toolbar->AddTool(wxID_HIGHEST + 3, wxT("New tab"), tb_add);
    toolbar->Realize();

    Connect(wxID_HIGHEST + 1, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::editLayout));
    Connect(wxID_HIGHEST + 2, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::deleteTab));
    Connect(wxID_HIGHEST + 3, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(MainWindow::addNewTab));

    initial_width_ = 1500;
    initial_height_ = 700;

    layout_tools_window_ = new LayoutToolsWindow(this, wxPoint(30, 500), wxSize(300, 300));
    // layout_tools_window_->Show();

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
