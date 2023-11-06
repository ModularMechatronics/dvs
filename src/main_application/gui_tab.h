#ifndef MAIN_APPLICATION_GUI_TAB_H_
#define MAIN_APPLICATION_GUI_TAB_H_

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
#include "help_pane.h"
#include "plot_pane.h"
#include "project_state/project_settings.h"
#include "tab_buttons.h"

class ZOrderQueue
{
private:
    std::vector<std::string> elements_;
    // [element{0}, element{1}, ..., element{n-1}]
    // First element in vector (element{n-1}) is the
    // element with the highest Z order (in front)

    bool elementExistsInQueue(const std::string& element_handle_string) const;

public:
    ZOrderQueue() = default;

    int getOrderOfElement(const std::string& element_handle_string) const;
    void raise(const std::string& element_handle_string);
    void lower(const std::string& element_handle_string);
    void eraseElement(const std::string& element_handle_string);
};

class WindowTab
{
private:
    std::string name_;
    std::vector<ApplicationGuiElement*> gui_elements_;
    wxFrame* parent_window_;
    std::function<void(const char key)> notify_main_window_key_pressed_;
    std::function<void(const char key)> notify_main_window_key_released_;
    std::function<void(const wxPoint pos, const std::string& elem_name)> notify_parent_window_right_mouse_pressed_;
    std::function<void(const std::string&)> notify_main_window_element_deleted_;
    std::function<void()> notify_main_window_about_modification_;
    int current_element_idx_;
    RGBTripletf background_color_;
    RGBTripletf button_normal_color_;
    RGBTripletf button_clicked_color_;
    RGBTripletf button_selected_color_;
    RGBTripletf button_text_color_;
    ZOrderQueue z_order_queue_;
    int element_x_offset_;

public:
    WindowTab(wxFrame* parent_window,
              const TabSettings& tab_settings,
              const int element_x_offset,
              const std::function<void(const char key)>& notify_main_window_key_pressed,
              const std::function<void(const char key)>& notify_main_window_key_released,
              const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                  notify_parent_window_right_mouse_pressed,
              const std::function<void(const std::string&)>& notify_main_window_element_deleted,
              const std::function<void()>& notify_main_window_about_modification);
    void initializeZOrder(const TabSettings& tab_settings);
    ~WindowTab();
    std::vector<ApplicationGuiElement*> getGuiElements() const;
    void updateAllElements();
    void setMinXPos(const int min_x_pos);
    void newElement();
    void newElement(const std::string& element_handle_string);
    void newElement(const ElementSettings& element_settings);
    void show();
    void hide();
    void updateSizeFromParent(const wxSize new_size) const;
    RGBTripletf getBackgroundColor() const;
    std::string getName() const;
    TabSettings getTabSettings() const;
    ApplicationGuiElement* getGuiElement(const std::string& element_handle_string) const;
    void notifyChildrenOnKeyPressed(const char key);
    void notifyChildrenOnKeyReleased(const char key);
    void setMouseInteractionType(const MouseInteractionType mit);
    bool deleteElementIfItExists(const std::string& element_handle_string);
    void toggleProjectionMode(const std::string& element_handle_string);
    bool elementWithNameExists(const std::string& element_handle_string);
    bool changeNameOfElementIfElementExists(const std::string& element_handle_string, const std::string& new_name);
    bool raiseElement(const std::string& element_handle_string);
    bool lowerElement(const std::string& element_handle_string);
    std::vector<std::string> getElementNames() const;
    void setName(const std::string& new_name);
};

#endif  // MAIN_APPLICATION_GUI_TAB_H_
