#ifndef MAIN_APPLICATION_MAIN_WINDOW_H_
#define MAIN_APPLICATION_MAIN_WINDOW_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/window.h>
#include <wx/wx.h>

#include <atomic>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "close_button.h"
#include "communication/data_receiver.h"
#include "communication/received_data.h"
// #include "dvs/gui_api.h"
#include "events.h"
#include "gui_element.h"
#include "input_data.h"
#include "plot_pane.h"
#include "project_state/configuration_agent.h"
#include "project_state/project_settings.h"
#include "project_state/save_manager.h"
#include "tab_button.h"
#include "tray_icon.h"
#include "window_button.h"

class GuiWindow;

class MainWindow : public wxFrame
{
private:
    DataReceiver data_receiver_;
    SaveManager* save_manager_;
    ConfigurationAgent* configuration_agent_;
    std::mutex receive_mtx_;

    std::thread* tcp_receive_thread_;
    std::map<std::string, ApplicationGuiElement*> plot_panes_;
    std::map<std::string, ApplicationGuiElement*> gui_elements_;

    std::map<std::string, std::queue<std::unique_ptr<InputData>>> queued_data_;

    CloseButton* close_button_;
    CloseButton* minimize_button_;

    bool shutdown_in_progress_;

    std::atomic<bool> open_project_file_queued_;
    std::string queued_project_file_name_;

    std::string current_element_name_;

    wxTimer receive_timer_;
    wxTimer refresh_timer_;

    std::vector<GuiWindow*> windows_;
    int current_window_num_;
    CustomTaskBarIcon* task_bar_;
    int window_callback_id_;

    bool window_initialization_in_progress_;

    std::function<void(const char key)> notification_from_gui_element_key_pressed_;
    std::function<void(const char key)> notification_from_gui_element_key_released_;
    std::function<std::vector<std::string>(void)> get_all_element_names_;
    std::function<void(const std::string&)> notify_main_window_element_deleted_;
    std::function<void(const std::string&, const std::string&)> notify_main_window_element_name_changed_;
    std::function<void(const std::string&, const std::string&)> notify_main_window_name_changed_;
    std::function<void()> notify_main_window_about_modification_;

    void OnReceiveTimer(wxTimerEvent&);
    void OnRefreshTimer(wxTimerEvent&);
    void setCurrentElement(const ReceivedData& received_data);
    void createNewElement(const internal::CommunicationHeader& hdr);
    void receiveData();

    void mouseLeftPressed(wxMouseEvent& event);

    void mouseLeftReleased(wxMouseEvent& event);
    void mouseMoved(wxMouseEvent& event);

    bool mouse_left_pressed_;
    wxPoint mouse_pos_at_press_;
    int first_window_button_offset_;

    std::vector<WindowButton*> window_buttons_;
    WindowButton* new_window_button_;

    bool hasWindowWithName(const std::string& window_name);
    void preferencesCallback(wxCommandEvent& event);
    void preferences();

    void windowNameChanged(const std::string& old_name, const std::string& new_name);

    void notifyChildrenOnKeyPressed(const char key);
    void notifyChildrenOnKeyReleased(const char key);

    void destroy();

    void saveProject();
    void saveProjectCallback(wxCommandEvent& event);
    void newProjectCallback(wxCommandEvent& event);
    void newProject();
    void saveProjectAs();
    void saveProjectAsCallback(wxCommandEvent& event);
    void openExistingFile(const std::string& file_path);

    void updateClientApplicationAboutGuiState() const;

    void performScreenshot(const std::string& screenshot_base_path);

    void removeAllWindows();
    void setupWindows(const ProjectSettings& project_settings);
    void fileModified();
    bool currentGuiElementSet() const;
    void tcpReceiveThreadFunction();
    void manageReceivedData(ReceivedData& received_data);
    void transmitBackGuiData(ReceivedData& received_data);

    void mainWindowFlushMultipleElements(const ReceivedData& received_data);
    void addActionToQueue(ReceivedData& received_data);
    void setIsFileSavedForAllWindows(const bool file_saved);
    wxMenuBar* createMainMenuBar();

    wxMenuBar* menu_bar_;
    wxMenu* windows_menu_;

    void toggleWindowVisibilityCallback(wxCommandEvent& event);
    void exitApplication(wxCommandEvent& event);

public:
    MainWindow();
    MainWindow(const std::vector<std::string>& cmdl_args);
    ~MainWindow();

    void OnChildDestroy(wxCloseEvent& event);

    std::vector<std::string> getAllElementNames() const;

    void deleteWindow(wxCommandEvent& event);
    void toggleWindowVisibility(const std::string& window_name);
    void openExistingFile(wxCommandEvent& event);
    void openExistingFileCallback(wxCommandEvent& WXUNUSED(event));
    void openExistingFile();
    void newWindowCallback(wxCommandEvent& WXUNUSED(event));
    void newWindow();
    void newWindowWithoutFileModification();

    void elementDeleted(const std::string& element_handle_string);
    void elementNameChanged(const std::string& old_name, const std::string& new_name);

    void disableEditing();
};

#endif  // MAIN_APPLICATION_MAIN_WINDOW_H_
