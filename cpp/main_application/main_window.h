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
#include "events.h"
#include "save_manager.h"
#include "cache_reader.h"
#include "window_view.h"

class MainWindow : public wxFrame
{
private:

    SaveManager* save_manager_;
    CacheReader* cache_reader_;

    UdpServer* udp_server_;
    wxTimer timer_;
    wxTimer refresh_timer_;

    std::vector<TabView*> tab_elements_;
    std::vector<WindowView*> windows_;
    std::map<std::string, GuiElement*> gui_elements_;

    GuiElement* current_gui_element_;
    bool current_gui_element_set_;
    int current_tab_num_;

    bool is_editing_;

    wxNotebook* tabs_view;
    wxPanel* tab_container;
    LayoutToolsWindow* layout_tools_window_;
    wxBoxSizer* tabs_sizer_v;

    wxMenuItem* edit_layout_menu_option_;
    wxMenu* m_pWindowsMenu;
    int window_id_offset;
    int window_callback_id_;

    int initial_width_;
    int initial_height_;

    std::string current_tab_name_;
    std::string current_element_name_;

    void OnTimer(wxTimerEvent&);
    void OnRefreshTimer(wxTimerEvent&);
    void setupGui();
    void setCurrentElement(const internal::FunctionHeader& hdr);
    void receiveData();

    void onActivate(wxActivateEvent& event);
    void tabChanged(wxCommandEvent& event);
    void toggleEditLayout(wxCommandEvent& event);
    void editingFinished(wxCommandEvent& event);
    void guiElementModified(wxCommandEvent& event);
    void childWindowClosed(wxCommandEvent& event);
    void childWindowInFocus(wxCommandEvent& event);

    void saveProject();
    void saveProjectCallback(wxCommandEvent& event);
    void newProjectCallback(wxCommandEvent& event);
    void saveProjectAs();
    void saveProjectAsCallback(wxCommandEvent& event);

    void setupTabs(const ProjectFile& project_file);
    void setupWindows(const ProjectFile& project_file);
    void fileModified();

public:
    MainWindow();
    MainWindow(const std::vector<std::string>& cmdl_args);
    ~MainWindow();

    virtual void OnClose(wxCloseEvent& event);
    virtual void OnSize(wxSizeEvent& event);
    void OnChildDestroy(wxCloseEvent& event);

    void toggleWindowVisibility(wxCommandEvent& event);
    void openExistingFile(wxCommandEvent& event);
    void changeCurrentTabName(wxCommandEvent& event);
    void changeCurrentElementName(wxCommandEvent& event);
    void currentElementSelectionChanged(wxCommandEvent& event);
    void addNewTab(wxCommandEvent& event);
    void deleteTab(wxCommandEvent& event);
    void addNewWindow(wxCommandEvent& event);
    void deleteWindow(wxCommandEvent& event);

    void newElement(wxCommandEvent& event);
    void deleteElement(wxCommandEvent& event);

    void disableEditing();
};

#endif
