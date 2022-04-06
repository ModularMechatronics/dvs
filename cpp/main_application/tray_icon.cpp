#include "tray_icon.h"



wxBEGIN_EVENT_TABLE(CustomTaskBarIcon, wxTaskBarIcon)
    EVT_MENU(PU_RESTORE, CustomTaskBarIcon::OnMenuRestore)
    EVT_MENU(PU_EXIT,    CustomTaskBarIcon::OnMenuExit)
    EVT_MENU(PU_CHECKMARK,CustomTaskBarIcon::OnMenuCheckmark)
    EVT_UPDATE_UI(PU_CHECKMARK,CustomTaskBarIcon::OnMenuUICheckmark)
    EVT_TASKBAR_LEFT_DCLICK  (CustomTaskBarIcon::OnLeftButtonDClick)
    EVT_MENU(PU_SUB1, CustomTaskBarIcon::OnMenuSub)
    EVT_MENU(PU_SUB2, CustomTaskBarIcon::OnMenuSub)
    // EVT_MENU_RANGE(PU_SUB1, PU_SUB2, CustomTaskBarIcon::OnMenuSub)
wxEND_EVENT_TABLE()

void CustomTaskBarIcon::OnMenuRestore(wxCommandEvent& )
{
    // gs_dialog->Show(true);
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

void CustomTaskBarIcon::OnMenuSetNewIcon(wxCommandEvent&)
{
    wxIcon* icon = new wxIcon();
    icon->LoadFile("../resources/images/apple.ico", wxBITMAP_TYPE_ICO, 32, 32);

    if (!SetIcon(*icon,
                                 "wxTaskBarIcon Sample\n"
                                 "With a very, very, very, very\n"
                                 "long tooltip whose length is\n"
                                 "greater than 64 characters.") )
    {
        wxLogError("Could not set icon.");
    }
}

void CustomTaskBarIcon::OnMenuSub(wxCommandEvent& evt)
{
    // wxStringClientData* cd = (wxStringClientData*)evt.GetClientObject();
    // std::cout << "WIndows Submenu: " << cd->GetData() << std::endl;
    std::cout << "GetInt: " << evt.GetInt() << std::endl;
    std::cout << "GetExtraLong: " << evt.GetExtraLong() << std::endl;
    std::cout << "GetSelection: " << evt.GetSelection() << std::endl;
    std::cout << "GetId: " << evt.GetId() << std::endl;
    
}

// Overridables
wxMenu *CustomTaskBarIcon::CreatePopupMenu()
{
    wxMenu *menu = new wxMenu;

    wxMenu *file_submenu = new wxMenu;
    file_submenu->Append(PU_FILE_NEW, wxT("New"));
    file_submenu->Append(PU_FILE_OPEN, wxT("Open..."));
    file_submenu->Append(PU_FILE_SAVE, wxT("Save"));
    file_submenu->Append(PU_FILE_SAVE_AS, wxT("Save As..."));
    
    menu->Append(PU_RESTORE, wxT("&File"), file_submenu);
    menu->AppendSeparator();
    menu->Append(PU_RESTORE, wxT("&Show main window"));
    menu->AppendSeparator();

    menu->Append(PU_NEW_ICON, wxT("&Edit layout"));
    menu->AppendSeparator();
    wxMenu *windows_submenu = new wxMenu;
    windows_submenu->Append(PU_SUB1, wxT("Main window"));
    windows_submenu->AppendSeparator();
    // wxMenuItem* window0 = new wxMenuItem(NULL, PU_SUB1);
    // window0->SetItemLabel(wxT("window-0"));
    // window0->SetText(wxT("window-0-text"));
    windows_submenu->Append(PU_SUB2, wxT("window-0"));
    windows_submenu->Append(PU_SUB2, wxT("window-0"));
    // windows_submenu->Append(window0);
    menu->Append(PU_SUBMAIN, wxT("Windows"), windows_submenu);
    /* OSX has built-in quit menu for the dock menu, but not for the status item */
#ifdef __WXOSX__ 
    if ( OSXIsStatusItem() )
#endif
    {
        menu->AppendSeparator();
        menu->Append(PU_EXIT,    wxT("E&xit"));
    }
    return menu;
}

void CustomTaskBarIcon::OnLeftButtonDClick(wxTaskBarIconEvent&)
{
    // gs_dialog->Show(true);
}