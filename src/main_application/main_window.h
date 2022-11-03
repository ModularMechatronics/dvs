#ifndef MAIN_APPLICATION_MAIN_WINDOW_H_
#define MAIN_APPLICATION_MAIN_WINDOW_H_

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
#include "events.h"
#include "gui_element.h"
#include "plot_pane.h"
#include "project_state/configuration_agent.h"
#include "project_state/project_settings.h"
#include "project_state/save_manager.h"
#include "tray_icon.h"
#include "window_view.h"

class MainWindow : public wxFrame
{
private:
    SaveManager* save_manager_;
    ConfigurationAgent* configuration_agent_;

    UdpServer* udp_server_;
    wxTimer timer_;
    wxTimer refresh_timer_;

    std::vector<WindowView*> windows_;
    std::map<std::string, GuiElement*> gui_elements_;

    GuiElement* current_gui_element_;
    bool current_gui_element_set_;
    int current_tab_num_;

    std::function<void(const char key)> notification_from_gui_element_key_pressed_;
    std::function<void(const char key)> notification_from_gui_element_key_released_;

    CustomTaskBarIcon* task_bar_;

    wxMenuItem* edit_layout_menu_option_;
    wxMenu* m_pWindowsMenu;
    int window_callback_id_;

    bool elementNameExists(const std::string& element_name) const;

    void OnTimer(wxTimerEvent&);
    void OnRefreshTimer(wxTimerEvent&);
    void OnKeyboardTimer(wxTimerEvent&);
    void setupGui();
    void setCurrentElement(const internal::CommunicationHeader& hdr);
    void createNewElement(const internal::CommunicationHeader& hdr);
    void setWaitForFlush();
    void receiveData();

    bool hasWindowWithName(const std::string& window_name);
    WindowView* getWindowWithName(const std::string& window_name);
    void onActivate(wxActivateEvent& event);
    void tabChanged(wxCommandEvent& event);
    void preferencesCallback(wxCommandEvent& event);
    void preferences();
    wxMenuBar* createMainMenuBar();
    void guiElementModified(wxCommandEvent& event);
    void childWindowClosed(wxCommandEvent& event);
    void childWindowInFocus(wxCommandEvent& event);
    void onCloseButton(wxCloseEvent& event);

    void notifyChildrenOnKeyPressed(const char key);
    void notifyChildrenOnKeyReleased(const char key);

    void saveProject();
    void saveProjectCallback(wxCommandEvent& event);
    void newProjectCallback(wxCommandEvent& event);
    void newProject();
    void saveProjectAs();
    void saveProjectAsCallback(wxCommandEvent& event);

    void setupWindows(const ProjectSettings& project_settings);
    void fileModified();

public:
    MainWindow();
    MainWindow(const std::vector<std::string>& cmdl_args);
    ~MainWindow();

    virtual void OnClose(wxCloseEvent& event);
    void OnChildDestroy(wxCloseEvent& event);

    void toggleWindowVisibilityCallback(wxCommandEvent& event);
    void toggleWindowVisibility(const std::string& window_name);
    void openExistingFile(wxCommandEvent& event);
    void openExistingFileCallback(wxCommandEvent& WXUNUSED(event));
    void openExistingFile();
    void changeCurrentElementName(const std::string new_element_name);
    void addNewWindow(const std::string& window_name);
    void addNewWindowCallback(wxCommandEvent& event);
    void deleteWindow();
    void noElementSelected(wxCommandEvent& event);

    void newNamedElement(const std::string& element_name);
    void newElement();
    void deleteSelectedElement();

    void disableEditing();
};

#endif  // MAIN_APPLICATION_MAIN_WINDOW_H_
