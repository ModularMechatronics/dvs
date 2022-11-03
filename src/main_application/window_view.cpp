#include "window_view.h"

#include "events.h"
#include "globals.h"
#include "plot_pane.h"

WindowView::WindowView(wxFrame* parent,
                       const WindowSettings& window_settings,
                       const int callback_id,
                       const std::function<void(const char key)>& notify_main_window_key_pressed,
                       const std::function<void(const char key)>& notify_main_window_key_released)
    : wxFrame(parent, wxID_ANY, "Figure 1"),
      tab_buttons_{this,
                   window_settings,
                   [this](const std::string name) { tabChanged(name); },
                   [this](const wxPoint pos) { mouseRightPressed(pos, ClickSource::TAB_BUTTON); }},
      notify_main_window_key_pressed_{notify_main_window_key_pressed},
      notify_main_window_key_released_{notify_main_window_key_released}

{
    grid_size_ = 1.0f;
    const RGBTripletf color_vec{axes_settings_.window_background_};
    SetBackgroundColour(wxColour(color_vec.red * 255.0f, color_vec.green * 255.0f, color_vec.blue * 255.0f));
    callback_id_ = callback_id;

    this->SetLabel(window_settings.getName());

    for (size_t k = 0; k < window_settings.tabs.size(); k++)
    {
        const std::string tab_name = window_settings.tabs[k].name;
        tabs_[tab_name] = new GuiTab(this,
                                     window_settings.tabs[k],
                                     notify_main_window_key_pressed,
                                     notify_main_window_key_released,
                                     [this](const wxPoint pos) { mouseRightPressed(pos, ClickSource::GUI_ELEMENT); });
    }

    /*const std::vector<ElementSettings> elements = settings_->getElementSettingsList();

    for (const auto& elem : elements)
    {
        GuiElement* const ge = new PlotPane(dynamic_cast<wxFrame*>(this),
                                            elem,
                                            grid_size_,
                                            notify_main_window_key_pressed_,
                                            notify_main_window_key_released_);

        ge->updateSizeFromParent(this->GetSize());
        gui_elements_[elem.name] = ge;
    }

    if (gui_elements_.size() > 0)
    {
        gui_elements_.begin()->second->setIsSelected();
    }*/

    for (const auto& tab : tabs_)
    {
        if (tab.second->getName() == window_settings.tabs[0].name)
        {
            tab.second->showAllElements();
        }
        else
        {
            tab.second->hideAllElements();
        }
    }
    this->SetPosition(wxPoint(20, 30));

    popup_menu_window_ = new wxMenu(wxT(""));
    popup_menu_element_ = new wxMenu(wxT(""));
    popup_menu_tab_ = new wxMenu(wxT(""));

    popup_menu_window_->AppendSeparator();
    popup_menu_window_->Append(wxID_ADD, wxT("&Edit window name"));
    popup_menu_window_->Append(wxID_ADD, wxT("&Delete window"));
    popup_menu_window_->AppendSeparator();
    popup_menu_window_->Append(wxID_ADD, wxT("&New window"));
    popup_menu_window_->Append(wxID_ADD, wxT("&New tab"));
    popup_menu_window_->Append(wxID_ADD, wxT("&New element"));

    popup_menu_element_->Append(wxID_ADD, wxT("&Edit element name"));
    popup_menu_element_->Append(wxID_ADD, wxT("&Delete element"));
    popup_menu_element_->AppendSeparator();
    popup_menu_element_->Append(wxID_ADD, wxT("&Edit window name"));
    popup_menu_element_->Append(wxID_ADD, wxT("&Delete window"));
    popup_menu_element_->AppendSeparator();
    popup_menu_element_->Append(wxID_ADD, wxT("&New window"));
    popup_menu_element_->Append(wxID_ADD, wxT("&New tab"));
    popup_menu_element_->Append(wxID_ADD, wxT("&New element"));

    popup_menu_tab_->Append(wxID_ADD, wxT("&Edit tab name"));
    popup_menu_tab_->Append(wxID_ADD, wxT("&Delete tab"));
    popup_menu_tab_->AppendSeparator();
    popup_menu_tab_->Append(wxID_ADD, wxT("&Edit window name"));
    popup_menu_tab_->Append(wxID_ADD, wxT("&Delete window"));
    popup_menu_tab_->AppendSeparator();
    popup_menu_tab_->Append(wxID_ADD, wxT("&New window"));
    popup_menu_tab_->Append(wxID_ADD, wxT("&New tab"));
    popup_menu_tab_->Append(wxID_ADD, wxT("&New element"));

    // Bind(wxEVT_MENU, &MainWindow::newProjectCallback, this, wxID_NEW);
    // Bind(wxEVT_CLOSE_WINDOW, &WindowView::OnClose, this);
    Bind(wxEVT_SIZE, &WindowView::OnSize, this);
    Bind(wxEVT_RIGHT_DOWN, &WindowView::mouseRightPressedCallback, this);

    tab_buttons_.windowWasResized(this->GetSize());
    show();
}

void WindowView::mouseRightPressed(const wxPoint pos, const ClickSource source)
{
    wxMenu menu(wxT(""));

    if (source == ClickSource::GUI_ELEMENT)
    {
        PopupMenu(popup_menu_element_, pos);
    }
    else if (source == ClickSource::TAB_BUTTON)
    {
        PopupMenu(popup_menu_tab_, pos);
    }
    else
    {
        PopupMenu(popup_menu_window_, pos);
    }
}

void WindowView::mouseRightPressedCallback(wxMouseEvent& event)
{
    mouseRightPressed(event.GetPosition(), ClickSource::THIS);
}

void WindowView::resetSelectionForAllChildren()
{
    /*for (const auto& tab : tabs_)
    {
        tab.second->resetSelection();
    }*/
}

void WindowView::childModified(wxCommandEvent& WXUNUSED(event))
{
    // wxCommandEvent parent_event(GUI_ELEMENT_CHANGED_EVENT);
    // wxPostEvent(this->GetParent(), parent_event);
}

std::map<std::string, GuiElement*> WindowView::getGuiElements() const
{
    std::map<std::string, GuiElement*> gui_elements;

    for (const auto& tab : tabs_)
    {
        const auto ges = tab.second->getGuiElements();

        gui_elements.insert(ges.begin(), ges.end());
    }

    return gui_elements;
}

void WindowView::tabChanged(const std::string name)
{
    for (const auto& tab : tabs_)
    {
        if (tab.first == name)
        {
            tab.second->showAllElements();
        }
        else
        {
            tab.second->hideAllElements();
        }
    }
}

std::vector<ElementSettings> WindowView::getElementSettingsList() const
{
    std::vector<ElementSettings> elements;

    /*for (auto it : gui_elements_)
    {
        elements.push_back(it.second->getElementSettings());
    }*/

    return elements;
}

void WindowView::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    const wxSize new_size = event.GetSize();

    tab_buttons_.windowWasResized(new_size);

    for (const auto& tab : tabs_)
    {
        tab.second->updateSizeFromParent(new_size);
    }
}

int WindowView::getCallbackId() const
{
    return callback_id_;
}

void WindowView::setName(const std::string& new_name)
{
    // this->SetLabel(new_name);
    // name_ = new_name;
}

void WindowView::onActivate(wxActivateEvent& event)
{
    if (event.GetActive())
    {
        wxCommandEvent parent_event(CHILD_WINDOW_IN_FOCUS_EVENT);
        parent_event.SetId(callback_id_);
        wxPostEvent(GetParent(), parent_event);
    }
}

void WindowView::hide()
{
    Unbind(wxEVT_ACTIVATE, &WindowView::onActivate, this);
    this->Hide();
}

void WindowView::show()
{
    this->Show();
    Bind(wxEVT_ACTIVATE, &WindowView::onActivate, this);
}

void WindowView::OnClose(wxCloseEvent& WXUNUSED(event))
{
    // wxCommandEvent parent_event(CHILD_WINDOW_CLOSED_EVENT);
    // wxPostEvent(GetParent(), parent_event);
    // hide();
}

void WindowView::newElement(const std::string& element_name)
{
    /*ElementSettings elem;
    elem.x = 0;
    elem.y = 0;
    elem.width = 0.3;
    elem.height = 0.3;
    elem.name = element_name;

    GuiElement* const ge = new PlotPane(dynamic_cast<wxNotebookPage*>(this),
                                        elem,
                                        grid_size_,
                                        notify_main_window_key_pressed_,
                                        notify_main_window_key_released_);

    ge->updateSizeFromParent(this->GetSize());
    if (is_editing_)
    {
        ge->setIsEditing(true);
    }
    gui_elements_[elem.name] = ge;*/
}

void WindowView::startEdit()
{
    is_editing_ = true;
    for (const auto& tab : tabs_)
    {
        tab.second->setIsEditing(true);
    }
}

void WindowView::stopEdit()
{
    is_editing_ = false;
    for (const auto& tab : tabs_)
    {
        tab.second->setIsEditing(false);
    }
}

void WindowView::newElement()
{
    /*ElementSettings elem;
    elem.x = 0;
    elem.y = 0;
    elem.width = 0.3;
    elem.height = 0.3;
    elem.name = "new-element-" + std::to_string(current_unused_element_idx++);

    GuiElement* const ge = new PlotPane(dynamic_cast<wxNotebookPage*>(this),
                                        elem,
                                        grid_size_,
                                        notify_main_window_key_pressed_,
                                        notify_main_window_key_released_);

    ge->updateSizeFromParent(this->GetSize());
    if (is_editing_)
    {
        ge->setIsEditing(true);
    }
    gui_elements_[elem.name] = ge;*/
}

std::string WindowView::getSelectedElementName()
{
    /*for (auto it : gui_elements_)
    {
        if (it.second->isSelected())
        {
            name_of_selected_element_ = it.second->getName();
        }
    }
    return name_of_selected_element_;*/
}

WindowSettings WindowView::getWindowSettings() const
{
    WindowSettings ws;
    const wxPoint pos = GetPosition();
    const wxSize size = GetSize();

    ws.x = pos.x;
    ws.y = pos.y;
    ws.width = size.GetWidth();
    ws.height = size.GetHeight();

    for (const auto& t : tabs_)
    {
        ws.tabs.push_back(t.second->getTabSettings());
    }

    return ws;
}

std::string WindowView::getName() const
{
    return name_;
}

void WindowView::deleteSelectedElement()
{
    /*std::string key_to_delete = "";
    for (auto it : gui_elements_)
    {
        if (it.second->isSelected())
        {
            key_to_delete = it.first;
        }
    }

    if (key_to_delete != "")
    {
        gui_elements_[key_to_delete]->destroy();
        gui_elements_.erase(key_to_delete);
    }*/
}

void WindowView::setSelectedElementName(const std::string& new_name)
{
    /*name_of_selected_element_ = new_name;
    for (auto it : gui_elements_)
    {
        if (it.second->isSelected())
        {
            it.second->setName(new_name);
        }
    }*/
}

void WindowView::mouseLeftPressed(wxMouseEvent& WXUNUSED(event))
{
    /*// Since this event will only be caught if the mouse is pressed outside of an
    // element, we know we can deselect all elements.
    // resetSelectionForAllChildren();
    name_of_selected_element_ = "";
    wxCommandEvent parent_event(NO_ELEMENT_SELECTED);
    wxPostEvent(this->GetParent(), parent_event);*/
}

void WindowView::setFirstElementSelected()
{
    /*if (gui_elements_.size() > 0)
    {
        gui_elements_.begin()->second->setSelection();
        gui_elements_.begin()->second->refresh();
    }*/
}
