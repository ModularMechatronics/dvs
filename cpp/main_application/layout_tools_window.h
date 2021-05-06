#ifndef LAYOUT_TOOLS_WINDOW_H_
#define LAYOUT_TOOLS_WINDOW_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/wx.h>

#include <mutex>
#include <string>
#include <map>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>
#include <csignal>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

class LayoutToolsWindow : public wxFrame
{
private:
    wxStaticBox* shapes_box_;
    wxStaticBox* inspector_box_;
    wxTextCtrl* tab_name_ctrl_;
    wxFrame* main_window_;

    void onTextEnter(wxCommandEvent& event);

public:

    LayoutToolsWindow() = default;
    LayoutToolsWindow(wxFrame* main_window, wxPoint pos, wxSize size);
    void setPosAndSize(wxPoint pos, wxSize size);
    void setupInspector();
    void setupShapes();
};

#endif
