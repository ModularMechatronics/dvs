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

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, "", wxPoint(30, 30), wxSize(500, 500))
{
    gui_pane_ = new GuiPane(this);
    update_timer_.Bind(wxEVT_TIMER, &MainWindow::timerFunction, this);
    update_timer_.Start(20);
}

MainWindow::~MainWindow() {}

void MainWindow::timerFunction(wxTimerEvent&)
{

}
