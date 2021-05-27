#include "tab_view.h"

#include "plot_window_gl_pane.h"
#include "events.h"

TabView::TabView(wxNotebookPage* parent, const TabSettings& tab_settings) : ViewBase<wxNotebookPage>(parent, tab_settings)
{
    const std::vector<project_file::ElementSettings> elements = settings_->getElementSettingsList();

    for(const auto elem : elements)
    {
        GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxWindow*>(this), elem, grid_size_);

        ge->updateSizeFromParent(this->GetSize());
        gui_elements_[elem.name] = ge;
    }

    if(gui_elements_.size() > 0)
    {
        gui_elements_.begin()->second->setIsSelected();
    }
    this->Show();
}

void TabView::newElement(const std::string& element_name)
{
    current_unnamed_idx_++;
    ElementSettings elem;
    elem.x = 0;
    elem.y = 0;
    elem.width = 0.3;
    elem.height = 0.3;
    elem.name = element_name;

    GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxNotebookPage*>(this), elem, grid_size_);

    ge->updateSizeFromParent(this->GetSize());
    if(is_editing_)
    {
        ge->setIsEditing(true);
    }
    gui_elements_[elem.name] = ge;
}

void TabView::newElement()
{
    current_unnamed_idx_++;
    ElementSettings elem;
    elem.x = 0;
    elem.y = 0;
    elem.width = 0.3;
    elem.height = 0.3;
    elem.name = "no-name-" + std::to_string(current_unnamed_idx_);

    GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxNotebookPage*>(this), elem, grid_size_);

    ge->updateSizeFromParent(this->GetSize());
    if(is_editing_)
    {
        ge->setIsEditing(true);
    }
    gui_elements_[elem.name] = ge;
}
