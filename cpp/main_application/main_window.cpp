#include "main_window.h"

#include <unistd.h>

#include <csignal>
#include <iostream>

#include "math/math.h"

MainWindow::MainWindow(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxPoint(30, 30), wxSize(700, 700))
{
    // udp_server_ = new UdpServer(9752);
    // udp_server_->start();

    wxPanel *panel_00 = new wxPanel(this, wxID_ANY);
    PlotWindowGLPane* gl_pane_00 = new PlotWindowGLPane(panel_00, wxPoint(0, 0), wxSize(150, 150));

    wxPanel *panel_10 = new wxPanel(this, wxID_ANY);
    PlotWindowGLPane* gl_pane_10 = new PlotWindowGLPane(panel_10, wxPoint(0, 0), wxSize(150, 150));

    wxPanel *panel_01 = new wxPanel(this, wxID_ANY);
    PlotWindowGLPane* gl_pane_01 = new PlotWindowGLPane(panel_01, wxPoint(0, 0), wxSize(150, 150));

    wxPanel *panel_11 = new wxPanel(this, wxID_ANY);
    PlotWindowGLPane* gl_pane_11 = new PlotWindowGLPane(panel_11, wxPoint(0, 0), wxSize(150, 150));

    wxPanel *panel_2 = new wxPanel(this, wxID_ANY);
    PlotWindowGLPane* gl_pane_2 = new PlotWindowGLPane(panel_2, wxPoint(0, 0), wxSize(300, 150));

    wxPanel *panel_3 = new wxPanel(this, wxID_ANY);
    PlotWindowGLPane* gl_pane_3 = new PlotWindowGLPane(panel_3, wxPoint(0, 0), wxSize(150, 450));

    wxBoxSizer* sizer0 = new wxBoxSizer(wxHORIZONTAL);
    sizer0->Add(panel_00, 0, 0, 0);
    sizer0->Add(panel_10, 0, 0, 0);

    wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);
    sizer1->Add(panel_01, 0, 0, 0);
    sizer1->Add(panel_11, 0, 0, 0);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer2 = new wxBoxSizer(wxHORIZONTAL);

    sizer->Add(sizer0, 0, 0, 0);
    sizer->Add(sizer1, 0, 0, 0);
    sizer->Add(panel_2, 0, 0, 0);

    sizer2->Add(sizer);
    sizer2->Add(panel_3);

    SetSizer(sizer2);

    timer_.Bind(wxEVT_TIMER, &MainWindow::OnTimer, this);
    // timer_.Start(100);
}

void MainWindow::OnTimer(wxTimerEvent&)
{
    std::unique_ptr<const ReceivedData> received_data = udp_server_->getReceivedData();
    if(received_data)
    {
        std::cout << "New data" << std::endl;
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
