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

class WindowView : public wxFrame
{
private:
    std::string name_;
    project_file::WindowSettings window_;
    std::map<std::string, GuiElement*> gui_elements_;
    float grid_size_;
    int current_unnamed_idx_;
    bool is_editing_;
    std::string name_of_selected_element_;

public:
    WindowView() = default;
    WindowView(wxWindow* parent) : wxFrame(parent,
              wxID_ANY,
              "Figure 1",
              wxPoint(30, 30),
              wxSize(600, 628))
    {
        // , const project_file::WindowSettings& tab
        this->Show();
    }
    /*void setSize(const wxSize& new_size);
    void newElement();
    void setSelectedElementName(const std::string& new_name);
    void setFirstElementSelected();
    std::string getSelectedElementName();

    TabSettings getTabSettings() const;
    void childModified(wxCommandEvent& event);
    std::map<std::string, GuiElement*> getGuiElements() const
    {
        return gui_elements_;
    }

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

    std::vector<ElementSettings> getElementSettingsList() const;*/

};


#endif
