#include "main_window.h"

#ifdef PLATFORM_LINUX_M
#include <libgen.h>
#include <linux/limits.h>
#include <unistd.h>

#endif

#ifdef PLATFORM_APPLE_M

#include <mach-o/dyld.h>

#endif

#include <unistd.h>
#include <wx/wfstream.h>
#include <wx/wxprec.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "gl_canvas.h"

using namespace dvs::internal;

MainWindow::MainWindow(const std::vector<std::string>& cmdl_args)
    : wxFrame(NULL, wxID_ANY, "", wxPoint(0, 30), wxSize(700, 700))
{
    static_cast<void>(cmdl_args);

#ifdef PLATFORM_LINUX_M
    int argc = 1;
    char* argv[1] = {"noop"};
    glutInit(&argc, argv);
#endif

    wxNotebookPage* tab_view_;

    tab_container_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(700, 700));
    tabs_view_ = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(700, 700));

    wxNotebookPage* tab_element = new wxNotebookPage(tabs_view_, -1);

    tabs_view_->AddPage(tab_element, "Main tab");

    GlCanvas* gl_pane = new GlCanvas(tabs_view_);
    gl_pane->show();

}

MainWindow::~MainWindow()
{
}
