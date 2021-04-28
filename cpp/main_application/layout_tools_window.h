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

public:

    LayoutToolsWindow() = default;
    LayoutToolsWindow(wxFrame* main_window, wxPoint pos, wxSize size);
    void setPosAndSize(wxPoint pos, wxSize size);
    void onButtonClickX(wxCommandEvent& event);
};

#endif
