#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/wx.h>

#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "udp_server.h"
#include "gui_element.h"
#include "plot_window_gl_pane.h"

wxDEFINE_EVENT(EVENT_TYPE_HANDLE_NEW_DATA, wxCommandEvent);

class MainWindow : public wxFrame
{
private:

    UdpServer* udp_server_;
    wxTimer timer_;
    // std::vector<std::shared_ptr<GuiElement>> gui_elements_;

    // std::vector<

    void OnTimer(wxTimerEvent&);

public:
    MainWindow();
    MainWindow(const wxString& title);
    virtual void OnClose(wxCloseEvent& event);
    void OnChildDestroy(wxCloseEvent& event);
};

#endif
