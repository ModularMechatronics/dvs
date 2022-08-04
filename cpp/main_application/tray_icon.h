#ifndef MAIN_APPLICATION_TRAY_ICON_H_
#define MAIN_APPLICATION_TRAY_ICON_H_

#include <wx/taskbar.h>
#include <wx/wx.h>

#include <iostream>
#include <map>
#include <functional>

#include "constants.h"

enum
{
    PU_EXIT = 10001,
    PU_SHOW_MAIN_WINDOW,
    PU_SHOW_SUB_WINDOW,
    PU_EDIT_LAYOUT,
    PU_PREFERENCES,
    PU_FILE,
    PU_FILE_NEW,
    PU_FILE_OPEN,
    PU_FILE_SAVE,
    PU_FILE_SAVE_AS,
    PU_FIRST_WINDOW_ID,
    PU_LAST_WINDOW_ID = PU_FIRST_WINDOW_ID + kMaxNumWindows - 1,
    PU_SUBMAIN
};

class MyMenu : public wxMenu
{
private:
    std::function<void()> menu_teardown_function_;
public:
    MyMenu() = delete;
    MyMenu(std::function<void()>&& menu_teardown_function) : 
        wxMenu{}, 
        menu_teardown_function_{std::move(menu_teardown_function)}
    {

    }

    ~MyMenu()
    {
        menu_teardown_function_();
    }
};


class CustomTaskBarIcon : public wxTaskBarIcon
{
public:
#if defined(__WXOSX__) && wxOSX_USE_COCOA
    CustomTaskBarIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE);
#else
    CustomTaskBarIcon();
#endif

    int getNextFreeId()
    {
        assert(free_ids_.size() != 0);
        const int val = free_ids_.back();
        free_ids_.pop_back();
        taken_ids_.push_back(val);
        return val;
    }

    void freeId(int id_to_free)
    {
        assert(std::find(taken_ids_.begin(), taken_ids_.end(), id_to_free) != taken_ids_.end());
        taken_ids_.erase(std::find(taken_ids_.begin(), taken_ids_.end(), id_to_free));
        free_ids_.push_back(id_to_free);
    }

    void setOnMenuExitCallback(std::function<void()>&& exit_function);
    void setOnMenuFileNew(std::function<void()>&& file_new_function);
    void setOnMenuFileOpen(std::function<void()>&& file_open_function);
    void setOnMenuFileSave(std::function<void()>&& file_save_function);
    void setOnMenuFileSaveAs(std::function<void()>&& file_save_as_function);
    void setOnMenuEdit(std::function<void()>&& edit_function);
    void setOnMenuSubWindow(std::function<void(const std::string&)>&& submenu_function);
    void setOnMenuShowMainWindow(std::function<void()>&& main_menu_function);
    void setOnMenuPreferences(std::function<void()>&& preferences_function);    

    void onMenuExit(wxCommandEvent&);
    void onMenuMainWindow(wxCommandEvent& evt);
    
    void onMenuFileNew(wxCommandEvent&);
    void onMenuFileOpen(wxCommandEvent&);
    void onMenuFileSave(wxCommandEvent&);
    void onMenuFileSaveAs(wxCommandEvent&);
    void onMenuEdit(wxCommandEvent&);
    void onMenuPreferences(wxCommandEvent&);

    void setEditLabel(const std::string edit_label);

    void onMenuSubWindow(wxCommandEvent& evt);
    void addNewWindow(const std::string& window_name);
    void removeWindow(const std::string& window_name);
    virtual wxMenu *CreatePopupMenu();

    wxDECLARE_EVENT_TABLE();

private:
    std::function<void()> exit_function_;
    std::function<void()> edit_function_;
    std::function<void()> preferences_function_;
    std::function<void()> file_new_function_;
    std::function<void()> file_open_function_;
    std::function<void()> file_save_function_;
    std::function<void()> file_save_as_function_;
    std::function<void()> main_menu_function_;
    std::function<void(const std::string&)> submenu_function_;

    std::vector<int> free_ids_;
    std::vector<int> taken_ids_;
    std::string edit_label_;
    wxMenu *windows_submenu_;
    // wxMenu *menu_;
    wxMenu *file_submenu_;

    std::map<std::string, wxEventTypeTag<wxCommandEvent>> window_events_;
    std::map<std::string, int> window_event_ids_;

};


#endif // MAIN_APPLICATION_TRAY_ICON_H_
