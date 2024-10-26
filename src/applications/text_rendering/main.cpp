#include <wx/wx.h>

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
    wxImage::AddHandler(new wxPNGHandler);
    wxImage::AddHandler(new wxICOHandler);

    main_window = new MainWindow();
    main_window->Show();

    return true;
}

int MainApp::OnExit()
{
    return true;
}
