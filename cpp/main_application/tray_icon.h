#ifndef MAIN_APPLICATION_TRAY_ICON_H_
#define MAIN_APPLICATION_TRAY_ICON_H_

#include <wx/taskbar.h>
#include <wx/wx.h>

#include <iostream>
#include <map>
#include <functional>

class CustomTaskBarIcon : public wxTaskBarIcon
{
public:
#if defined(__WXOSX__) && wxOSX_USE_COCOA
    CustomTaskBarIcon(wxTaskBarIconType iconType = wxTBI_DEFAULT_TYPE)
    :   wxTaskBarIcon(iconType)
#else
    CustomTaskBarIcon()
#endif
    {}

    void setOnExitCallback(std::function<void()> exit_function);

    void OnLeftButtonDClick(wxTaskBarIconEvent&);
    void OnMenuRestore(wxCommandEvent&);
    void OnMenuExit(wxCommandEvent&);
    void OnMenuSetNewIcon(wxCommandEvent&);
    void OnMenuCheckmark(wxCommandEvent&);
    void OnMenuUICheckmark(wxUpdateUIEvent&);
    void OnMenuSub(wxCommandEvent& evt);
    virtual wxMenu *CreatePopupMenu();

    wxDECLARE_EVENT_TABLE();

private:
    std::function<void()> exit_function_;

};

enum
{
    PU_RESTORE = 10001,
    PU_EXIT,
    PU_CHECKMARK,
    PU_SUB1,
    PU_SUB2,
    PU_FILE_NEW,
    PU_FILE_OPEN,
    PU_FILE_SAVE = PU_FILE_OPEN + 10,
    PU_FILE_SAVE_AS,
    PU_SUBMAIN
};


#endif
