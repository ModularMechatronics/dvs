#ifndef MAIN_APPLICATION_MAIN_WINDOW_H_
#define MAIN_APPLICATION_MAIN_WINDOW_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
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

#include "communication/received_data.h"
#include "communication/udp_server.h"
#include "events.h"
#include "gui_element.h"
#include "plot_pane.h"
#include "project_state/configuration_agent.h"
#include "project_state/project_settings.h"
#include "project_state/save_manager.h"
#include "queueable_action.h"
#include "tray_icon.h"

class WindowView;

class MainWindow : public wxFrame
{
private:
    std::atomic<bool> is_rendering_;
    SaveManager* save_manager_;
    ConfigurationAgent* configuration_agent_;
    std::mutex udp_mtx_;
    std::mutex reveive_mtx_;

    std::thread* query_thread_;
    std::thread* receive_thread_;

    std::queue<std::unique_ptr<const ReceivedData>> received_data_buffer_;

    std::map<std::string, std::queue<std::unique_ptr<QueueableAction>>> queued_actions_;

    std::atomic<bool> open_project_file_queued_;
    properties::Name queued_project_file_name_;
    std::string current_element_name_;

    UdpServer* udp_server_;
    UdpServer* query_udp_server_;
    wxTimer receive_timer_;
    wxTimer refresh_timer_;

    std::vector<WindowView*> windows_;

    GuiElement* current_gui_element_;
    int current_window_num_;

    std::function<void(const char key)> notification_from_gui_element_key_pressed_;
    std::function<void(const char key)> notification_from_gui_element_key_released_;

    CustomTaskBarIcon* task_bar_;

    int window_callback_id_;

    void OnReceiveTimer(wxTimerEvent&);
    void OnRefreshTimer(wxTimerEvent&);
    void setCurrentElement(const ReceivedData& received_data);
    void createNewElement(const internal::CommunicationHeader& hdr);
    void setWaitForFlush();
    void receiveData();

    bool hasWindowWithName(const std::string& window_name);
    void preferencesCallback(wxCommandEvent& event);
    void preferences();

    void notifyChildrenOnKeyPressed(const char key);
    void notifyChildrenOnKeyReleased(const char key);

    void saveProject();
    void saveProjectCallback(wxCommandEvent& event);
    void newProjectCallback(wxCommandEvent& event);
    void newProject();
    void saveProjectAs();
    void saveProjectAsCallback(wxCommandEvent& event);
    void openExistingFile(const std::string& file_path);
    void openFileFromClient(const internal::CommunicationHeader& hdr);

    void setupWindows(const ProjectSettings& project_settings);
    void fileModified();
    bool currentGuiElementSet() const;
    void queryUdpThreadFunction();
    void receiveThreadFunction();
    void mainWindowFlushMultipleElements(const ReceivedData& received_data);
    void addActionToQueue(ReceivedData& received_data);

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
    void newWindow(wxCommandEvent& WXUNUSED(event));
    void elementWasDeleted(const GuiElement* const ge);

    void disableEditing();
};

#endif  // MAIN_APPLICATION_MAIN_WINDOW_H_
