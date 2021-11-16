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
    Bind(wxEVT_SIZE, &MainWindow::OnSize, this);
    wxNotebookPage* tab_view_;

    tab_container_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(700, 700));
    tabs_view_ = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(700, 700));

    wxNotebookPage* tab_element = new wxNotebookPage(tabs_view_, -1);

    tabs_view_->AddPage(tab_element, "Main tab");

    gl_canvas_ = new GlCanvas(tabs_view_);
    gl_canvas_->show();

}

MainWindow::~MainWindow()
{
}

void MainWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);

    const wxSize new_size = this->GetSize();
    tab_container_->SetSize(new_size);
    tabs_view_->SetSize(new_size);
    const int offset = 70;
    const wxSize gl_canvas_size(new_size.GetWidth() - offset, new_size.GetHeight() - offset);
    gl_canvas_->setSize(gl_canvas_size);
}
