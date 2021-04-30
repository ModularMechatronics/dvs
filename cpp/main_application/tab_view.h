#ifndef TAB_H_
#define TAB_H_

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
#include "prototype_view.h"

class TabView : public wxNotebookPage
{
private:
    std::string name_;
    project_file::Tab tab_;
    std::map<std::string, GuiElement*> gui_elements_;

public:
    TabView() = default;
    TabView(wxNotebook* parent, const project_file::Tab tab);
    void changeSize(const wxSize& new_size);

    void startEdit();
    void stopEdit();

};

#endif
