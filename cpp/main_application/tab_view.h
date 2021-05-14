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

class TabView : public wxNotebookPage
{
private:
    std::string name_;
    project_file::TabSettings tab_;
    std::map<std::string, GuiElement*> gui_elements_;
    float grid_size_;
    int current_unnamed_idx_;
    bool is_editing_;
    std::string name_of_selected_element_;

public:
    TabView() = default;
    TabView(wxNotebook* parent, const project_file::TabSettings& tab);
    void setSize(const wxSize& new_size);
    void newElement();
    void setSelectedElementName(const std::string& new_name);
    void setFirstElementSelected();
    std::string getSelectedElementName();

    std::string getName() const
    {
        return name_;
    }

    void setName(const std::string& new_name)
    {
        name_ = new_name;
    }

    void startEdit();
    void stopEdit();

    void resetSelectionForAllChildren();

    void deleteSelectedElement();

    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);

    std::vector<ElementSettings> getElementSettingsList() const;

};

#endif
