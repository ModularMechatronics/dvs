#include "tab_view.h"

#include "plot_window_gl_pane.h"

TabView::TabView(wxNotebook* parent, const project_file::Tab& tab) : wxNotebookPage(parent, -1), name_(tab.getName()), tab_(tab)
{
    grid_size_ = 5.0f;

    const std::vector<project_file::Element> elements = tab.getElements();
    this->SetBackgroundColour(wxColor(110, 2, 65));

    for(const auto elem : elements)
    {
        GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxNotebookPage*>(this), elem, grid_size_);

        ge->updateSizeFromParent(this->GetSize());
        gui_elements_[elem.name] = ge;
    }
}

void TabView::startEdit()
{
    for(auto it : gui_elements_)
    {
        it.second->setIsEditing(true);
    }
}

void TabView::stopEdit()
{
    for(auto it : gui_elements_)
    {
        it.second->setIsEditing(false);
    }
}

void TabView::setSize(const wxSize& new_size)
{
    for(auto it : gui_elements_)
    {
        it.second->updateSizeFromParent(new_size);
    }
}
