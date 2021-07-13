#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

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

#include "communication/received_data.h"
#include "communication/udp_server.h"
#include "custom_button.h"
#include "events.h"
#include "gui_element.h"
#include "layout_tools_window.h"
#include "plot_window_gl_pane.h"
#include "project_state/cache_reader.h"
#include "project_state/project_settings.h"
#include "project_state/save_manager.h"
#include "tab_view.h"
#include "window_view.h"

class MainWindow : public wxFrame
{
private:
    SaveManager* save_manager_;
    CacheReader* cache_reader_;

    UdpServer* udp_server_;
    wxTimer timer_;
    wxTimer refresh_timer_;

    std::vector<TabView*> tabs_;
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
    int window_callback_id_;
    bool main_window_last_in_focus_;

    int initial_width_;
    int initial_height_;

    std::string current_tab_name_;
    std::string current_element_name_;

    bool elementNameExists(const std::string& element_name) const;

    void OnTimer(wxTimerEvent&);
    void OnRefreshTimer(wxTimerEvent&);
    void setupGui();
    void setCurrentElement(const internal::FunctionHeader& hdr);
    void createNewElement(const internal::FunctionHeader& hdr);
    void receiveData();

    bool hasTabWithName(const std::string& tab_name);
    TabView* getTabWithName(const std::string& tab_name);
    bool hasWindowWithName(const std::string& window_name);
    WindowView* getWindowWithName(const std::string& window_name);
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

    void setupTabs(const ProjectSettings& project_settings);
    void setupWindows(const ProjectSettings& project_settings);
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
    void addNewTabCallback(wxCommandEvent& event);
    void addNewTab(const std::string& tab_name);
    void deleteTab(wxCommandEvent& event);
    void addNewWindow(const std::string& window_name);
    void addNewWindowCallback(wxCommandEvent& event);
    void deleteWindow(wxCommandEvent& event);
    void noElementSelected(wxCommandEvent& event);

    void newNamedElement(const std::string& element_name);
    void newElement(wxCommandEvent& event);
    void deleteElement(wxCommandEvent& event);

    void disableEditing();
};

#endif
