#include "tray_icon.h"
#include "events.h"

wxBEGIN_EVENT_TABLE(CustomTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(PU_EXIT,    CustomTaskBarIcon::onMenuExit)
    EVT_MENU(PU_SHOW_MAIN_WINDOW, CustomTaskBarIcon::onMenuMainWindow)
    EVT_MENU(PU_FILE_NEW, CustomTaskBarIcon::onMenuFileNew)
    EVT_MENU(PU_FILE_OPEN, CustomTaskBarIcon::onMenuFileOpen)
    EVT_MENU(PU_FILE_SAVE, CustomTaskBarIcon::onMenuFileSave)
    EVT_MENU(PU_FILE_SAVE_AS, CustomTaskBarIcon::onMenuFileSaveAs)
    EVT_MENU(PU_EDIT_LAYOUT, CustomTaskBarIcon::onMenuEdit)
    EVT_MENU(PU_PREFERENCES, CustomTaskBarIcon::onMenuPreferences)
wxEND_EVENT_TABLE()

#if defined(__WXOSX__) && wxOSX_USE_COCOA
CustomTaskBarIcon::CustomTaskBarIcon(wxTaskBarIconType iconType) :
  wxTaskBarIcon(iconType),
#else
    CustomTaskBarIcon::CustomTaskBarIcon() : 
#endif
    // menu_{nullptr},
      file_submenu_{nullptr},
      windows_submenu_{nullptr},
      edit_label_{"Edit layout"}
{
    for(int k = PU_FIRST_WINDOW_ID; k <= PU_LAST_WINDOW_ID; k++)
    {
        free_ids_.push_back(k);
    }
    taken_ids_.clear();
}

void CustomTaskBarIcon::setOnMenuExitCallback(std::function<void()>&& function)
{
    exit_function_ = std::move(function);
}

void CustomTaskBarIcon::setOnMenuFileNew(std::function<void()>&& file_new_function)
{
    file_new_function_ = std::move(file_new_function);
}

void CustomTaskBarIcon::setOnMenuFileOpen(std::function<void()>&& file_open_function)
{
    file_open_function_ = std::move(file_open_function);
}

void CustomTaskBarIcon::setOnMenuFileSave(std::function<void()>&& file_save_function)
{
    file_save_function_ = std::move(file_save_function);
}

void CustomTaskBarIcon::setOnMenuFileSaveAs(std::function<void()>&& file_save_as_function)
{
    file_save_as_function_ = std::move(file_save_as_function);
}

void CustomTaskBarIcon::setOnMenuEdit(std::function<void()>&& edit_function)
{
    edit_function_ = std::move(edit_function);
}

void CustomTaskBarIcon::setOnMenuSubWindow(std::function<void(const std::string&)>&& submenu_function)
{
    submenu_function_ = std::move(submenu_function);
}

void CustomTaskBarIcon::setOnMenuShowMainWindow(std::function<void()>&& main_menu_function)
{
    main_menu_function_ = std::move(main_menu_function);
}

void CustomTaskBarIcon::onMenuExit(wxCommandEvent& )
{
    exit_function_();
}


void CustomTaskBarIcon::onMenuMainWindow(wxCommandEvent& evt)
{
    main_menu_function_();
}

void CustomTaskBarIcon::onMenuSubWindow(wxCommandEvent& evt)
{
    bool window_found = false;
    std::string window_name;
    for ( const auto &p : window_event_ids_)
    {
        if(p.second == evt.GetId())
        {
            window_found = true;
            window_name = p.first;
            break;
        }
    }
    assert(window_found);
    submenu_function_(window_name);
}

void CustomTaskBarIcon::onMenuFileNew(wxCommandEvent&)
{
    file_new_function_();
}

void CustomTaskBarIcon::onMenuFileOpen(wxCommandEvent&)
{
    file_open_function_();
}

void CustomTaskBarIcon::onMenuFileSave(wxCommandEvent&)
{
    file_save_function_();
}

void CustomTaskBarIcon::onMenuFileSaveAs(wxCommandEvent&)
{
    file_save_as_function_();
}

void CustomTaskBarIcon::onMenuEdit(wxCommandEvent&)
{
    edit_function_();
}

void CustomTaskBarIcon::onMenuPreferences(wxCommandEvent&)
{
    preferences_function_();
}

void CustomTaskBarIcon::addNewWindow(const std::string& window_name)
{
    const auto id = getNextFreeId();
    window_events_.emplace(window_name, wxEventTypeTag<wxCommandEvent>(id));
    window_event_ids_[window_name] = id;
}

void CustomTaskBarIcon::removeWindow(const std::string& window_name)
{
    const int id_to_free = window_event_ids_[window_name];
    freeId(id_to_free);
    window_events_.erase(window_events_.find(window_name));
    window_event_ids_.erase(window_event_ids_.find(window_name));
}

void CustomTaskBarIcon::setEditLabel(const std::string edit_label)
{
    edit_label_ = edit_label;
}

void CustomTaskBarIcon::setOnMenuPreferences(std::function<void()>&& preferences_function)
{
    preferences_function_ = preferences_function;
}
    

wxMenu *CustomTaskBarIcon::CreatePopupMenu()
{
    MyMenu* menu_ = new MyMenu{[this] () -> void {
        for ( const auto &p : window_events_ )
        {
            Unbind(wxEVT_MENU, &CustomTaskBarIcon::onMenuSubWindow, this, window_event_ids_[p.first]);
        }
    }};
    file_submenu_ = new wxMenu;
    windows_submenu_ = new wxMenu;

    file_submenu_->Append(PU_FILE_NEW, wxT("New"));
    file_submenu_->Append(PU_FILE_OPEN, wxT("Open..."));
    file_submenu_->Append(PU_FILE_SAVE, wxT("Save"));
    file_submenu_->Append(PU_FILE_SAVE_AS, wxT("Save As..."));
    
    menu_->Append(PU_FILE, wxT("&File"), file_submenu_);
    menu_->AppendSeparator();

    menu_->Append(PU_EDIT_LAYOUT, edit_label_);
    menu_->Append(PU_PREFERENCES, "Preferences");
    menu_->AppendSeparator();

    windows_submenu_->Append(PU_SHOW_MAIN_WINDOW, wxT("Main window"));
    windows_submenu_->AppendSeparator();

    for ( const auto &p : window_events_ )
    {
        windows_submenu_->Append(window_event_ids_[p.first], p.first);
        Bind(wxEVT_MENU, &CustomTaskBarIcon::onMenuSubWindow, this, p.second);
    }

    menu_->Append(PU_SUBMAIN, wxT("Windows"), windows_submenu_);

#ifdef __WXOSX__ 
    if ( OSXIsStatusItem() )
#endif
    {
        menu_->AppendSeparator();
        menu_->Append(PU_EXIT, wxT("E&xit"));
    }
    return menu_;
}
