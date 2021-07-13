#ifndef DVS_WINDOW_VIEW_H_
#define DVS_WINDOW_VIEW_H_

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

#include "gui_element.h"
#include "project_state/project_settings.h"
#include "view_base.h"

class WindowView : public ViewBase<wxFrame>
{
private:
    int callback_id_;

public:
    WindowView() = delete;
    WindowView(wxFrame* parent, const WindowSettings& window_settings, const int callback_id);

    void newElement(const std::string& element_name);
    void newElement() override;
    int getCallbackId() const;
    void OnSize(wxSizeEvent& event);

    void onActivate(wxActivateEvent& event);
    void hide();
    void show();
    void setName(const std::string& new_name) override;

    virtual void OnClose(wxCloseEvent& event);
};

#endif
