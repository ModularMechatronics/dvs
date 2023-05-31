#ifndef REAL_TIME_GUI_MAIN_WINDOW_H_
#define REAL_TIME_GUI_MAIN_WINDOW_H_

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

class ImuVisualizer;

class RtMainWindow : public wxFrame
{
private:
    wxTimer update_timer_;
    wxSlider* hori_slider;
    wxSlider* vert_slider;

    ImuVisualizer* visualizer_;

    void timerFunction(wxTimerEvent&);

public:
    RtMainWindow();
    ~RtMainWindow();
};

#endif  // REAL_TIME_GUI_MAIN_WINDOW_H_
