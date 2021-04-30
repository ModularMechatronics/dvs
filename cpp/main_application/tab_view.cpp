#include "tab_view.h"

TabView::TabView(wxNotebook* parent, const project_file::Tab tab) : wxNotebookPage(parent, -1), name_(tab.getName()), tab_(tab)
{
    const wxSize tab_size = this->GetSize();
    const float tab_dx = static_cast<float>(tab_size.GetWidth()) / static_cast<float>(tab.num_cells_x);
    const float tab_dy = static_cast<float>(tab_size.GetHeight()) / static_cast<float>(tab.num_cells_y);

    const Vec2Df tab_cell_size(tab_dx, tab_dy);

    const std::vector<project_file::Element> elements = tab.getElements();
    const Vec2Df num_grid_cells(tab.num_cells_x, tab.num_cells_y);

    // prototype_view_ = new PrototypeView(dynamic_cast<wxNotebookPage*>(this), wxPoint(0, 0), tab_size, num_grid_cells, elements);
    // prototype_view_->hide();
    
    for(const auto e : elements)
    {
        float xpos = e.cell_idx_x * tab_dx;
        float ypos = e.cell_idx_y * tab_dy;

        wxSize gui_element_size(e.width * tab_dx, e.height * tab_dy);
        GuiElement* ge = new GuiElement(dynamic_cast<wxNotebookPage*>(this), e);
        ge->changeSize(tab_cell_size);
        gui_elements_[e.name] = ge;
    }
}

void TabView::startEdit()
{
    /*for(auto it : gui_elements_)
    {
        it.second->hide();
    }
    prototype_view_->show();*/
}

void TabView::stopEdit()
{
    /*prototype_view_->hide();
    for(auto it : gui_elements_)
    {
        it.second->show();
    }*/
}

void TabView::changeSize(const wxSize& new_size)
{
    const wxSize tab_size = this->GetSize();
    // prototype_view_->setSize(tab_size);

    const float tab_dx = static_cast<float>(tab_size.GetWidth()) / static_cast<float>(tab_.num_cells_x);
    const float tab_dy = static_cast<float>(tab_size.GetHeight()) / static_cast<float>(tab_.num_cells_y);

    const Vec2Df tab_cell_size(tab_dx, tab_dy);

    for(auto it : gui_elements_)
    {
        it.second->changeSize(tab_cell_size);
    }
}
