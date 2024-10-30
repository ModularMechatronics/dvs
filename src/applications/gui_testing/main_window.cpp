#include "main_window.h"

#include <unistd.h>
#include <wx/wfstream.h>
#include <wx/wxprec.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "duoplot/constants.h"
#include "duoplot/duoplot.h"
#include "duoplot/math/math.h"

using namespace duoplot;

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, "", wxPoint(300, 600), wxSize(500, 500))
{
    gui_pane_ = new GuiPane(this);
    Bind(wxEVT_SIZE, &MainWindow::OnSize, this);
    Bind(wxEVT_DPI_CHANGED, &MainWindow::DpiChanged, this);
}

MainWindow::~MainWindow() {}

void MainWindow::timerFunction(wxTimerEvent&) {}

void MainWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    const wxSize new_size = event.GetSize();

    // gui_pane_->UpdateSizeFromParent(new_size);
}

void MainWindow::DpiChanged(wxDPIChangedEvent& event)
{
    std::cout << "DPI changed!" << std::endl;
    const wxSize old_dpi = event.GetOldDPI();
    const wxSize new_dpi = event.GetNewDPI();

    std::cout << "Old DPI: " << old_dpi.GetWidth() << " " << old_dpi.GetHeight() << std::endl;
    std::cout << "New DPI: " << new_dpi.GetWidth() << " " << new_dpi.GetHeight() << std::endl;
}
