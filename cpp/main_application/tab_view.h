#ifndef TAB_H_
#define TAB_H_

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
#include "project_settings.h"
#include "view_base.h"

class TabView : public ViewBase<wxNotebookPage>
{
private:
public:
    TabView() = delete;
    TabView(wxNotebookPage* parent, const TabSettings& tab_settings);

    void newElement(const std::string& element_name);
    void newElement() override;
};

#endif
