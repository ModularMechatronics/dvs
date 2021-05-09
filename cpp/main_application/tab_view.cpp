#include "tab_view.h"

#include "plot_window_gl_pane.h"

TabView::TabView(wxNotebook* parent, const project_file::Tab& tab) : wxNotebookPage(parent, -1), name_(tab.getName()), tab_(tab)
{
    grid_size_ = 5.0f;

    const std::vector<project_file::Element> elements = tab.getElements();
    this->SetBackgroundColour(wxColor(110, 2, 65));

    current_unnamed_idx_ = 0;
    is_editing_ = false;

    Bind(wxEVT_LEFT_DOWN, &TabView::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &TabView::mouseLeftReleased, this);

    for(const auto elem : elements)
    {
        GuiElement* const ge = new PlotWindowGLPane(dynamic_cast<wxNotebookPage*>(this), elem, grid_size_);

        ge->updateSizeFromParent(this->GetSize());
        gui_elements_[elem.name] = ge;
    }
}

std::vector<Element> TabView::getElements() const
{
    std::vector<Element> elements;

    for(auto it : gui_elements_)
    {
        elements.push_back(it.second->getElementSettings());
    }

    return elements;
}

void TabView::deleteSelectedElement()
{
    std::string key_to_delete = "";
    for(auto it : gui_elements_)
    {
        if(it.second->isSelected())
        {
            key_to_delete = it.first;
        }
    }

    if(key_to_delete != "")
    {
        gui_elements_[key_to_delete]->destroy();
        gui_elements_.erase(key_to_delete);
    }
}

void TabView::resetSelectionForAllChildren()
{
    for(auto it : gui_elements_)
    {
        it.second->resetSelection();
        it.second->refresh();
    }
}

void TabView::setSelectedElementName(const std::string& new_name)
{
    name_of_selected_element_ = new_name;
}

void TabView::mouseLeftPressed(wxMouseEvent& event)
{
    // Since this event will only be caught if the mouse is pressed outside of an
    // element, we know we can deselect all elements.
    resetSelectionForAllChildren();
    name_of_selected_element_ = "";
}

void TabView::mouseLeftReleased(wxMouseEvent& event)
{

}

std::string TabView::getSelectedElementName() const
{
    return name_of_selected_element_;
}

void TabView::newElement()
{
    current_unnamed_idx_++;
    Element elem;
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

void TabView::startEdit()
{
    is_editing_ = true;
    for(auto it : gui_elements_)
    {
        it.second->setIsEditing(true);
    }
}

void TabView::stopEdit()
{
    is_editing_ = false;
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
