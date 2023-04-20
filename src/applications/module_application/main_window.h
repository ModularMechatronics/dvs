#ifndef MODULE_APPLICATION_MAIN_WINDOW_H_
#define MODULE_APPLICATION_MAIN_WINDOW_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

class MainWindow : public wxFrame
{
private:
    void OnSize(wxSizeEvent& event);

    void guiElementCallbackHandler(wxCommandEvent& event);
    void sliderHandler(wxCommandEvent& event);

public:
    MainWindow();
    MainWindow(const std::vector<std::string>& cmdl_args);
    ~MainWindow();
};

#endif  // MODULE_APPLICATION_MAIN_WINDOW_H_
