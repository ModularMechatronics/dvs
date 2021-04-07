#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

wxDEFINE_EVENT(EVENT_TYPE_HANDLE_NEW_DATA, wxCommandEvent);

class MainWindow : public wxFrame
{
private:

public:
    MainWindow();
    MainWindow(const wxString& title);
    virtual void OnClose(wxCloseEvent& event);
    void OnChildDestroy(wxCloseEvent& event);
};

#endif
