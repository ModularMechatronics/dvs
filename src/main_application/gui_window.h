#ifndef MAIN_APPLICATION_WINDOW_VIEW_H_
#define MAIN_APPLICATION_WINDOW_VIEW_H_

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

#include "axes/axes.h"
#include "gui_element.h"
#include "gui_elements.h"
#include "gui_tab.h"
#include "help_pane.h"
#include "plot_pane.h"
#include "project_state/project_settings.h"
#include "tab_buttons.h"

class GuiWindow : public wxFrame
{
private:
    enum ClickSource
    {
        TAB_BUTTON,
        GUI_ELEMENT,
        THIS
    };

    TabButtons tab_buttons_;
    std::vector<WindowTab*> tabs_;
    int element_x_offset_;
    int callback_id_;
    AxesSettings axes_settings_{};
    void tabChanged(const std::string name);
    std::function<void(const char key)> notify_main_window_key_pressed_;
    std::function<void(const char key)> notify_main_window_key_released_;
    std::function<void(const std::string&)> notify_main_window_element_deleted_;
    std::function<std::vector<std::string>(void)> get_all_element_names_;
    std::function<void(const std::string&, const std::string&)> notify_main_window_element_name_changed_;
    std::function<void(const std::string&, const std::string&)> notify_main_window_name_changed_;
    std::function<void()> notify_main_window_about_modification_;
    HelpPane help_pane_;

    std::function<void(const wxPoint pos, const std::string& elem_name)> notify_parent_window_right_mouse_pressed_;

    std::function<void()> notify_main_window_new_window_;
    std::string name_;
    wxFrame* main_window_;

    wxMenu* new_element_menu_window_;
    wxMenu* new_element_menu_element_;
    wxMenu* new_element_menu_tab_;

    wxMenu* popup_menu_window_;
    wxMenu* popup_menu_element_;
    wxMenu* popup_menu_tab_;

    wxMenuItem* getMenuItemFromString(const wxMenu* const menu, const std::string& menu_item_string) const;
    std::map<std::string, std::string> getValidNewElementHandleString(const std::map<std::string, std::pair<std::string, std::string>>& fields);

    int current_tab_num_;

    std::string last_clicked_item_;

    std::string project_name_;

    RGBTripletf dialog_color_;

    bool project_is_saved_;

public:
    GuiWindow() = delete;
    GuiWindow(
        wxFrame* main_window,
        const WindowSettings& window_settings,
        const std::string& project_name,
        const int callback_id,
        const bool project_is_saved,
        const std::function<void(const char key)>& notify_main_window_key_pressed,
        const std::function<void(const char key)>& notify_main_window_key_released,
        const std::function<std::vector<std::string>(void)>& get_all_element_names,
        const std::function<void(const std::string&)>& notify_main_window_element_deleted,
        const std::function<void(const std::string&, const std::string&)>& notify_main_window_element_name_changed,
        const std::function<void(const std::string&, const std::string&)>& notify_main_window_name_changed,
        const std::function<void()>& notify_main_window_about_modification);
    ~GuiWindow();
    int getCallbackId() const;
    void OnSize(wxSizeEvent& event);
    void OnMove(wxMoveEvent& event);
    void keyPressedCallback(wxKeyEvent& evt);
    void keyReleasedCallback(wxKeyEvent& evt);

    void setName(const std::string& new_name);
    void updateLabel();
    WindowSettings getWindowSettings() const;
    std::string getName() const;
    void setIsFileSavedForLabel(const bool is_saved);
    void setProjectName(const std::string& project_name);
    void deleteAllTabs();

    ApplicationGuiElement* getGuiElement(const std::string& element_handle_string) const;

    void mouseRightPressed(const wxPoint pos, const ClickSource source, const std::string& item_name);

    void mouseRightPressedCallback(wxMouseEvent& event);
    void notifyChildrenOnKeyPressed(const char key);
    void notifyChildrenOnKeyReleased(const char key);

    void editWindowName(wxCommandEvent& WXUNUSED(event));

    void newTab(wxCommandEvent& WXUNUSED(event));
    void createNewPlotPaneCallbackFunction(wxCommandEvent& WXUNUSED(event));
    void createNewSliderCallbackFunction(wxCommandEvent& WXUNUSED(event));
    void createNewButtonCallbackFunction(wxCommandEvent& WXUNUSED(event));
    void createNewCheckBoxCallbackFunction(wxCommandEvent& WXUNUSED(event));
    void createNewPlotPane();

    void editElementName(wxCommandEvent& WXUNUSED(event));
    void deleteElement(wxCommandEvent& WXUNUSED(event));

    void raiseElement(wxCommandEvent& WXUNUSED(event));
    void lowerElement(wxCommandEvent& WXUNUSED(event));

    void toggleProjectionMode(wxCommandEvent& WXUNUSED(event));

    void toggleToZoomCallback(wxCommandEvent& WXUNUSED(event));
    void toggleToPanCallback(wxCommandEvent& WXUNUSED(event));
    void toggleToRotateCallback(wxCommandEvent& WXUNUSED(event));
    void toggleToSelectCallback(wxCommandEvent& WXUNUSED(event));

    void editTabName(wxCommandEvent& WXUNUSED(event));
    void deleteTab(wxCommandEvent& WXUNUSED(event));

    virtual void OnClose(wxCloseEvent& event);

    void updateAllElements();
    std::vector<ApplicationGuiElement*> getGuiElements() const;
    std::vector<ApplicationGuiElement*> getPlotPanes() const;
    std::vector<ApplicationGuiElement*> getAllGuiElements() const;

    void screenshot(const std::string& base_path);

    std::vector<std::string> getElementNames() const;
};

#endif  // MAIN_APPLICATION_WINDOW_VIEW_H_
