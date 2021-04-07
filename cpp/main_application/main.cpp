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
};

MainWindow* main_window;

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
    main_window = new MainWindow("DVS v0.1");
    main_window->Show();

    SetTopWindow(main_window);

    return true;
}

int MainApp::OnExit()
{
    std::cout << "Exit!" << std::endl;
    return true;
}
