#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

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

#include "udp_server.h"
#include "received_data.h"
#include "gui_element.h"
#include "plot_window_gl_pane.h"
#include "layout_tools_window.h"
#include "project_file.h"
#include "prototype_view.h"

wxDEFINE_EVENT(EVENT_TYPE_HANDLE_NEW_DATA, wxCommandEvent);

class ResizablePanel : wxPanel
{
private:

public:

    ResizablePanel() = default;
    ResizablePanel(wxPanel* parent, wxPoint pos, wxSize size) : wxPanel(parent, wxID_ANY, pos, size)
    {

    }

    void setBackgroundColour(wxColor col)
    {
        this->SetBackgroundColour(col);
    }

    void setSize(wxPoint pos, wxSize size)
    {
        this->SetPosition(pos);
        this->SetSize(size);
    }
};

class MainWindow : public wxFrame
{
private:

    UdpServer* udp_server_;
    wxTimer timer_;
    project_file::ProjectFile project_file_;
    std::map<std::string, GuiElement*> gui_elements_;

    GuiElement* current_gui_element_;
    bool current_gui_element_set_;

    wxNotebook* tabs_view;
    wxPanel* tab_container;
    LayoutToolsWindow* layout_tools_window_;
    PrototypeView* prototype_view_;

    int initial_width_;
    int initial_height_;

    void OnTimer(wxTimerEvent&);
    void setupGui();
    void setCurrentElement(const internal::FunctionHeader& hdr);
    void receiveData();

    void addNewTab(wxCommandEvent & event);

public:
    MainWindow();
    MainWindow(const wxString& title);
    ~MainWindow();

    virtual void OnClose(wxCloseEvent& event);
    virtual void OnSize(wxSizeEvent& event);
    void OnChildDestroy(wxCloseEvent& event);

    void numCellsXInc(wxCommandEvent& event);
    void numCellsXDec(wxCommandEvent& event);
    void numCellsYInc(wxCommandEvent& event);
    void numCellsYDec(wxCommandEvent& event);
    void marginXInc(wxCommandEvent& event);
    void marginXDec(wxCommandEvent& event);
    void marginYInc(wxCommandEvent& event);
    void marginYDec(wxCommandEvent& event);
};

#endif
