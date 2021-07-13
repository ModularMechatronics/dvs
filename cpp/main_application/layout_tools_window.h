#ifndef LAYOUT_TOOLS_WINDOW_H_
#define LAYOUT_TOOLS_WINDOW_H_

#include <unistd.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <csignal>
#include <iostream>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

class LayoutToolsWindow : public wxFrame
{
private:
    wxStaticBox* shapes_box_;
    wxStaticBox* inspector_box_;
    wxTextCtrl* tab_name_ctrl_;
    wxTextCtrl* element_name_ctrl_;
    wxFrame* main_window_;

public:
    LayoutToolsWindow() = default;
    LayoutToolsWindow(wxFrame* main_window, wxPoint pos, wxSize size);
    void setPosAndSize(wxPoint pos, wxSize size);
    void setupInspector();
    void setupShapes();

    void setCurrentTabName(const std::string& tab_name);
    void setCurrentElementName(const std::string& name_of_selected);

    virtual void OnClose(wxCloseEvent& event);
};

#endif
