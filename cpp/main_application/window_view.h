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
public:
    WindowView() = default;
    WindowView(wxFrame* parent, const WindowSettings& window_settings);

    void newElement() override;
};

#endif
