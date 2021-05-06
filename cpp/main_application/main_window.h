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
#include "tab_view.h"

wxDEFINE_EVENT(EVENT_TYPE_HANDLE_NEW_DATA, wxCommandEvent);

class MainWindow : public wxFrame
{
private:

    UdpServer* udp_server_;
    wxTimer timer_;
    project_file::ProjectFile project_file_;

    std::vector<TabView*> tab_elements_;

    // GuiElement* current_gui_element_;
    bool current_gui_element_set_;
    int current_tab_num_;

    bool is_editing_;

    wxNotebook* tabs_view;
    wxPanel* tab_container;
    LayoutToolsWindow* layout_tools_window_;

    int initial_width_;
    int initial_height_;

    void OnTimer(wxTimerEvent&);
    void setupGui();
    void setCurrentElement(const internal::FunctionHeader& hdr);
    void receiveData();

    void addNewTab(wxCommandEvent& event);
    void deleteTab(wxCommandEvent& event);
    void editLayout(wxCommandEvent& event);

    void saveProject() const;

    // void onRightClickMenu(wxCommandEvent& event);
    // void onShowContextMenu(wxContextMenuEvent& event);
    // enum MenuIDs { MENU_ID_CONTEXT_1 = wxID_HIGHEST + 4, MENU_ID_CONTEXT_2, MENU_ID_CONTEXT_3 };

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
    void changeCurrentTabName(wxCommandEvent& event);

    void newElement(wxCommandEvent& event);
    void deleteElement(wxCommandEvent& event);
};

#endif
