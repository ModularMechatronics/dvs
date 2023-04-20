#include <wx/wx.h>

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "dvs/dvs.h"
#include "main_window.h"
#include "module_api.h"

using namespace dvs;

/*
 -- Gui Element Output Types:
A button is clicked
A slider is dragged and gets a new value
A checkbox is checked or unchecked
An editable text field is edited
A drop down menu is changed
A list box is changed
A radio button is changed

 -- Gui Element Input Types:
A slider has its range set
A text field has its text set
A drop down menu has its items set
A list box has its items set


All elements can be enabled or disabled
*/

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
    main_window = new MainWindow();
    main_window->Show();

    SetTopWindow(main_window);

    return true;
}

int MainApp::OnExit()
{
    return true;
}
