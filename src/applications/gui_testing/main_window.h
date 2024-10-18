#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <atomic>
#include <map>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "gui_pane.h"

class MainWindow : public wxFrame
{
private:
    wxTimer update_timer_;

    void timerFunction(wxTimerEvent&);
    GuiPane* gui_pane_;

public:
    MainWindow();
    ~MainWindow();
};

#endif  // MAIN_WINDOW_H
