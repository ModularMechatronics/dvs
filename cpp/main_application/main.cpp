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
};

MainWindow* main_window;

IMPLEMENT_APP(MainApp)

bool MainApp::OnInit()
{
#if 0
    const std::string splash_img_path = "../splash_trans_small.png";
    wxInitAllImageHandlers();
    wxImage splash_img(splash_img_path, wxBITMAP_TYPE_PNG);
    wxSplashScreen *scrn = new wxSplashScreen(splash_img,
                                              wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
                                              2500,
                                              NULL,
                                              wxID_ANY,
                                              wxDefaultPosition,
                                              wxSize(500, 500),
                                              wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP | wxTRANSPARENT_WINDOW);
    // SplashScreen?
    // wxFrame *frame = new wxFrame(NULL, wxID_ANY, "Something", wxPoint(300, 300), wxSize(200, 200), wxFRAME_TOOL_WINDOW | wxNO_BORDER);
    // frame->Show(true);
#endif

    std::vector<std::string> cmdl_args;
    for (int k = 0; k < wxAppConsole::argc; k++)
    {
        cmdl_args.emplace_back(wxAppConsole::argv[k].mb_str());
    }
    main_window = new MainWindow(cmdl_args);
    main_window->Show();

    SetTopWindow(main_window);

    return true;
}

int MainApp::OnExit()
{
    std::cout << "Exit!" << std::endl;
    return true;
}
