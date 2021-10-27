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

#include "dvs/math/math.h"
#include "gui_element.h"
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

    ElementSettings elem;
    elem.x = 0;
    elem.y = 0;
    elem.width = 0.3;
    elem.height = 0.3;
    elem.name = "Base";

    wxNotebookPage* tab_view_;
    

    // GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxNotebookPage*>(this), elem, 10);

    // ge->updateSizeFromParent(this->GetSize());
    // tabs_view = new wxNotebook(tab_container, wxID_ANY, wxDefaultPosition, wxSize(500, 500));
    tab_container_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(700, 700));
    tabs_view_ = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(700, 700));


    // TabView* tab_element = new TabView(tabs_view, tab);
    // tabs_.push_back(tab_element);
    wxNotebookPage* tab_element = new wxNotebookPage(tabs_view_, -1);

    tabs_view_->AddPage(tab_element, "Main tab");

    GlCanvas* gl_pane = new GlCanvas(tabs_view_);
    gl_pane->show();

}

MainWindow::~MainWindow()
{
}
