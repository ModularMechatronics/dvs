#include "tab_view.h"

#include "plot_window_gl_pane.h"

TabView::TabView(wxNotebook* parent, const project_file::Tab& tab) : wxNotebookPage(parent, -1), name_(tab.getName()), tab_(tab)
{
    const wxSize tab_size = this->GetSize();
    const float tab_dx = static_cast<float>(tab_size.GetWidth()) / static_cast<float>(tab.num_cells_x);
    const float tab_dy = static_cast<float>(tab_size.GetHeight()) / static_cast<float>(tab.num_cells_y);

    grid_size_ = 5.0f;

    const Vec2Df tab_cell_size(tab_dx, tab_dy);

    const std::vector<project_file::Element> elements = tab.getElements();
    const Vec2Df num_grid_cells(tab.num_cells_x, tab.num_cells_y);

    for(const auto elem : elements)
    {
        GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxNotebookPage*>(this), elem, grid_size_);

        ge->updateSize(this->GetSize());
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

void TabView::changeSize(const wxSize& new_size)
{
    const wxSize tab_size = this->GetSize();

    const float tab_dx = static_cast<float>(tab_size.GetWidth()) / static_cast<float>(tab_.num_cells_x);
    const float tab_dy = static_cast<float>(tab_size.GetHeight()) / static_cast<float>(tab_.num_cells_y);

    const Vec2Df tab_cell_size(tab_dx, tab_dy);

    for(auto it : gui_elements_)
    {
        it.second->updateSize(this->GetSize());
    }
}
