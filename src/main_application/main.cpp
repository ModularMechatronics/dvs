#include <wx/wx.h>

#include <iostream>

#include "dvs/logging.h"
#include "main_window.h"

class MainApp : public wxApp
{
private:
public:
    virtual bool OnInit();
    virtual int OnExit();
    void appInFocus(const wxActivateEvent& event);
};

MainWindow* main_window;

IMPLEMENT_APP(MainApp)

void MainApp::appInFocus(const wxActivateEvent& event)
{
    if (event.GetActive())
    {
        main_window->appActive();
    }
    else
    {
        main_window->appInactive();
    }
}

bool MainApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxICOHandler);

    std::vector<std::string> cmd_args;
    for (int k = 0; k < wxAppConsole::argc; k++)
    {
        cmd_args.emplace_back(wxAppConsole::argv[k].mb_str());
    }

    main_window = new MainWindow(cmd_args);
    main_window->Show();

    Bind(wxEVT_ACTIVATE_APP, &MainApp::appInFocus, this);

    SetTopWindow(main_window);

    return true;
}

int MainApp::OnExit()
{
    DVS_LOG_INFO() << "Exit from MainApp::OnExit!";
    return true;
}
