#include <wx/wx.h>

#include <iostream>

#include "dvs/logging.h"
#include "rt_main_window.h"

class MainApp : public wxApp
{
private:
public:
    virtual bool OnInit();
    virtual int OnExit();
};

RtMainWindow* main_window;

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxICOHandler);

    main_window = new RtMainWindow();
    main_window->Show();

    return true;
}

int MainApp::OnExit()
{
    DVS_LOG_INFO() << "Exit from MainApp::OnExit!";
    return true;
}
