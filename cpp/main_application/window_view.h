#ifndef DVS_WINDOW_VIEW_H_
#define DVS_WINDOW_VIEW_H_


#include <wx/button.h>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/wx.h>

#include <mutex>
#include <string>
#include <map>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "project_file.h"
#include "gui_element.h"
#include "view_base.h"

class WindowView : public ViewBase<wxFrame>
{
private:
    int callback_id_;
public:
    WindowView() = delete;
    WindowView(wxFrame* parent, const WindowSettings& window_settings, const int callback_id);

    void newElement() override;
    int getCallbackId() const;

    void onActivate(wxActivateEvent& event);
    void hide();
    void show();

    virtual void OnClose(wxCloseEvent& event);
};

#endif
