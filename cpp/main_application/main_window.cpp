#include "main_window.h"

#include "math/math.h"

#include <unistd.h>

#include <csignal>
#include <iostream>

MainWindow::MainWindow(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxPoint(30, 30), wxSize(700, 700))
{
    udp_server_ = new UdpServer(9752);
    udp_server_->start();
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
