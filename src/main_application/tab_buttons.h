#ifndef MAIN_APPLICATION_TAB_CONTAINER_H_
#define MAIN_APPLICATION_TAB_CONTAINER_H_

#include <unistd.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <functional>
#include <string>
#include <vector>

#include "project_state/project_settings.h"
#include "tab_button.h"

class TabButtons
{
private:
    std::function<void(std::string)> tab_changed_callback_;
    std::vector<TabButton*> tab_buttons_;
    wxSize window_size_;
    int button_height_, button_width_;
    WindowSettings window_settings_;
    int tab_button_id_counter_;
    wxFrame* parent_;
    std::function<void(const wxPoint pos, const std::string& item_name)> notify_parent_window_right_mouse_pressed_;

    void layoutButtons();

public:
    TabButtons(wxFrame* parent,
               const WindowSettings& window_settings,
               const std::function<void(std::string)> tab_changed_callback,
               const std::function<void(const wxPoint pos, const std::string& item_name)>&
                   notify_parent_window_right_mouse_pressed);

    void setSelection(const std::string id);
    void windowWasResized(const wxSize new_size);
    void addNewTab(const std::string name);
    std::string getNameOfSelectedTab() const;
    void deleteTabButton(const std::string& button_name);
    void changeButtonName(const std::string& old_label, const std::string& new_label);
};

#endif  // MAIN_APPLICATION_TAB_CONTAINER_H_
