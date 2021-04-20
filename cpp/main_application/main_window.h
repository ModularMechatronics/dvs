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
#include <map>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "udp_server.h"
#include "gui_element.h"
#include "plot_window_gl_pane.h"
#include "project_file.h"

wxDEFINE_EVENT(EVENT_TYPE_HANDLE_NEW_DATA, wxCommandEvent);

class MainWindow : public wxFrame
{
private:

    UdpServer* udp_server_;
    wxTimer timer_;
    project_file::ProjectFile project_file_;
    std::map<std::string, GuiElement*> gui_elements_;

    GuiElement* current_gui_element_;
    bool current_gui_element_set_;

    void OnTimer(wxTimerEvent&);
    void setupGui();
    void setCurrentElement(const internal::FunctionHeader& hdr);

public:
    MainWindow();
    MainWindow(const wxString& title);
    ~MainWindow();

    virtual void OnClose(wxCloseEvent& event);
    void OnChildDestroy(wxCloseEvent& event);
};

#endif
