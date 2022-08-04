#ifndef APPLICATIONS_SHADER_APP_MAIN_WINDOW_H_
#define APPLICATIONS_SHADER_APP_MAIN_WINDOW_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "gui_element.h"
#include "plot_window_gl_pane.h"
#include "tab_view.h"
#include "window_view.h"
#include "gl_canvas.h"

class MainWindow : public wxFrame
{
private:

    void OnKeyDown(wxKeyEvent& event);
    PlotWindowGLPane* plot_window_;
    wxNotebookPage* tab_pages_;
    wxNotebook* tabs_view_;
    wxPanel* tab_container_;
    GlCanvas* gl_canvas_;

    void OnSize(wxSizeEvent& event);

public:
    MainWindow();
    MainWindow(const std::vector<std::string>& cmdl_args);
    ~MainWindow();

    

    // virtual void OnClose(wxCloseEvent& event);
    // virtual void OnSize(wxSizeEvent& event);
    // void OnChildDestroy(wxCloseEvent& event);

};

#endif // APPLICATIONS_SHADER_APP_MAIN_WINDOW_H_
