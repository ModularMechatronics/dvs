#include "tray_icon.h"
#include "events.h"

wxBEGIN_EVENT_TABLE(CustomTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(PU_EXIT,    CustomTaskBarIcon::OnMenuExit)
    EVT_MENU(PU_SHOW_MAIN_WINDOW, CustomTaskBarIcon::OnMainWindow)
wxEND_EVENT_TABLE()

#if defined(__WXOSX__) && wxOSX_USE_COCOA
CustomTaskBarIcon::CustomTaskBarIcon(wxTaskBarIconType iconType) :
  wxTaskBarIcon(iconType)
#else
    CustomTaskBarIcon::CustomTaskBarIcon()
#endif
{
    menu_ = new wxMenu;

    file_submenu_ = new wxMenu;
    windows_submenu_ = new wxMenu;

    for(int k = PU_FIRST_WINDOW_ID; k <= PU_LAST_WINDOW_ID; k++)
    {
        free_ids_.push_back(k);
    }
    std::cout << "Size: " << free_ids_.size() << std::endl;
    taken_ids_.clear();
}

void CustomTaskBarIcon::setOnExitCallback(std::function<void()> function)
{
    exit_function_ = std::move(function);
}

void CustomTaskBarIcon::OnMenuExit(wxCommandEvent& )
{
    std::cout << "Exit!" << std::endl;
    exit_function_();
}


int i = 0;

void CustomTaskBarIcon::OnMainWindow(wxCommandEvent& evt)
{
    // wxStringClientData* cd = (wxStringClientData*)evt.GetClientObject();
    // std::cout << "WIndows Submenu: " << cd->GetData() << std::endl;
    std::cout << "Show main window" << std::endl;
    addNewWindow("window" + std::to_string(i));
    i++;
}

void CustomTaskBarIcon::OnSubWindow(wxCommandEvent& evt)
{
    std::cout << "Subwindow: " << evt.GetId() << std::endl;
}

void CustomTaskBarIcon::addNewWindow(const std::string& window_name)
{
    std::cout << "addNedWindow" << std::endl;
    // const auto id = wxNewEventType();
    const auto id = getNextFreeId();
    window_events_.emplace(window_name, wxEventTypeTag<wxCommandEvent>(id));
    window_event_ids_[window_name] = id;
    std::cout << "id: " << id << std::endl;

}

void CustomTaskBarIcon::removeWindow(const std::string& window_name)
{
    // wxEventTypeTag<wxCommandEvent> name;
}

wxMenu *CustomTaskBarIcon::CreatePopupMenu()
{
    file_submenu_->Append(PU_FILE_NEW, wxT("New"));
    file_submenu_->Append(PU_FILE_OPEN, wxT("Open..."));
    file_submenu_->Append(PU_FILE_SAVE, wxT("Save"));
    file_submenu_->Append(PU_FILE_SAVE_AS, wxT("Save As..."));
    
    menu_->Append(PU_FILE, wxT("&File"), file_submenu);
    menu_->AppendSeparator();
    menu_->Append(PU_SHOW_MAIN_WINDOW, wxT("&Show main window"));
    menu_->AppendSeparator();

    menu_->Append(PU_EDIT_LAYOUT, wxT("&Edit layout"));
    menu_->AppendSeparator();

    windows_submenu_->Append(PU_SHOW_MAIN_WINDOW, wxT("Main window"));
    windows_submenu_->AppendSeparator();

    for ( const auto &p : window_events_ )
    {
        windows_submenu_->Append(window_event_ids_[p.first], p.first);
        Bind(wxEVT_MENU, &CustomTaskBarIcon::OnSubWindow, this, p.second);
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
