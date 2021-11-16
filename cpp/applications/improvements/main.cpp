#include <wx/splash.h>
#include <wx/wx.h>

#include <csignal>
#include <iostream>

#include "main_window.h"

class MainApp : public wxApp
{
private:
public:
    virtual bool OnInit();
    virtual int OnExit();
    void appInFocus(wxActivateEvent& event);
};

MainWindow* main_window;

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
    std::vector<std::string> cmd_args;
    for (int k = 0; k < wxAppConsole::argc; k++)
    {
        cmd_args.emplace_back(wxAppConsole::argv[k].mb_str());
    }
    main_window = new MainWindow(cmd_args);
    main_window->Show();

    SetTopWindow(main_window);

    return true;
}

int MainApp::OnExit()
{
    return true;
}
