#include "window_view.h"

#include "events.h"
#include "globals.h"
#include "main_window.h"
#include "plot_pane.h"

WindowView::WindowView(wxFrame* main_window,
                       const WindowSettings& window_settings,
                       const int callback_id,
                       const std::function<void(const char key)>& notify_main_window_key_pressed,
                       const std::function<void(const char key)>& notify_main_window_key_released,
                       const std::function<void()>& notify_main_window_new_window)
    : wxFrame(main_window, wxID_ANY, "Figure 1"),
      tab_buttons_{this,
                   window_settings,
                   [this](const std::string name) { tabChanged(name); },
                   [this](const wxPoint pos) { mouseRightPressed(pos, ClickSource::TAB_BUTTON); }},
      notify_main_window_key_pressed_{notify_main_window_key_pressed},
      notify_main_window_key_released_{notify_main_window_key_released},
      notify_main_window_new_window_{notify_main_window_new_window}
{
    main_window_ = main_window;
    current_tab_num_ = 0;
    grid_size_ = 1.0f;
    const RGBTripletf color_vec{axes_settings_.window_background_};
    SetBackgroundColour(wxColour(color_vec.red * 255.0f, color_vec.green * 255.0f, color_vec.blue * 255.0f));
    callback_id_ = callback_id;

    this->SetPosition(wxPoint(window_settings.x, window_settings.y));
    this->SetLabel(window_settings.name);
    this->SetSize(wxSize(window_settings.width, window_settings.height));
    name_ = window_settings.name;

    for (size_t k = 0; k < window_settings.tabs.size(); k++)
    {
        const std::string tab_name = window_settings.tabs[k].name;
        tabs_[tab_name] =
            new WindowTab(this,
                          window_settings.tabs[k],
                          notify_main_window_key_pressed,
                          notify_main_window_key_released,
                          [this](const wxPoint pos) { mouseRightPressed(pos, ClickSource::GUI_ELEMENT); });
    }

    for (const auto& tab : tabs_)
    {
        if (tab.second->getName() == window_settings.tabs[0].name)
        {
            tab.second->show();
        }
        else
        {
            tab.second->hide();
        }
    }

    popup_menu_window_ = new wxMenu(wxT(""));
    popup_menu_element_ = new wxMenu(wxT(""));
    popup_menu_tab_ = new wxMenu(wxT(""));

    popup_menu_window_->AppendSeparator();
    popup_menu_window_->Append(dvs_ids::EDIT_WINDOW_NAME, wxT("Edit window name"));
    popup_menu_window_->Append(callback_id_ + 1, wxT("Delete window"));
    popup_menu_window_->AppendSeparator();
    popup_menu_window_->Append(dvs_ids::NEW_WINDOW, wxT("New window"));
    popup_menu_window_->Append(dvs_ids::NEW_TAB, wxT("New tab"));
    popup_menu_window_->Append(dvs_ids::NEW_ELEMENT, wxT("New element"));

    popup_menu_element_->Append(dvs_ids::EDIT_ELEMENT_NAME, wxT("Edit element name"));
    popup_menu_element_->Append(dvs_ids::DELETE_ELEMENT, wxT("Delete element"));
    popup_menu_element_->AppendSeparator();
    popup_menu_element_->Append(dvs_ids::EDIT_WINDOW_NAME, wxT("Edit window name"));
    popup_menu_element_->Append(callback_id_ + 1, wxT("Delete window"));
    popup_menu_element_->AppendSeparator();
    popup_menu_element_->Append(dvs_ids::NEW_WINDOW, wxT("New window"));
    popup_menu_element_->Append(dvs_ids::NEW_TAB, wxT("New tab"));
    popup_menu_element_->Append(dvs_ids::NEW_ELEMENT, wxT("New element"));

    popup_menu_tab_->Append(dvs_ids::EDIT_TAB_NAME, wxT("Edit tab name"));
    popup_menu_tab_->Append(dvs_ids::DELETE_TAB, wxT("Delete tab"));
    popup_menu_tab_->AppendSeparator();
    popup_menu_tab_->Append(dvs_ids::EDIT_WINDOW_NAME, wxT("Edit window name"));
    popup_menu_tab_->Append(callback_id_ + 1, wxT("Delete window"));
    popup_menu_tab_->AppendSeparator();
    popup_menu_tab_->Append(dvs_ids::NEW_WINDOW, wxT("New window"));
    popup_menu_tab_->Append(dvs_ids::NEW_TAB, wxT("New tab"));
    popup_menu_tab_->Append(dvs_ids::NEW_ELEMENT, wxT("New element"));

    // Bind(wxEVT_MENU, &WindowView::editWindowName, this, dvs_ids::EDIT_WINDOW_NAME);
    Bind(wxEVT_MENU, &MainWindow::deleteWindow, static_cast<MainWindow*>(main_window_), callback_id_ + 1);
    Bind(wxEVT_MENU, &WindowView::newWindow, this, dvs_ids::NEW_WINDOW);
    // Bind(wxEVT_MENU, &WindowView::newTab, this, dvs_ids::NEW_TAB);
    // Bind(wxEVT_MENU, &WindowView::newElement, this, dvs_ids::NEW_ELEMENT);
    // Bind(wxEVT_MENU, &WindowView::editElementName, this, dvs_ids::EDIT_ELEMENT_NAME);
    // Bind(wxEVT_MENU, &WindowView::deleteElement, this, dvs_ids::DELETE_ELEMENT);
    // Bind(wxEVT_MENU, &WindowView::editTabName, this, dvs_ids::EDIT_TAB_NAME);
    // Bind(wxEVT_MENU, &WindowView::deleteTab, this, dvs_ids::DELETE_TAB);

    // Bind(wxEVT_MENU, &MainWindow::newProjectCallback, this, wxID_NEW);
    // Bind(wxEVT_CLOSE_WINDOW, &WindowView::OnClose, this);
    Bind(wxEVT_SIZE, &WindowView::OnSize, this);
    Bind(wxEVT_RIGHT_DOWN, &WindowView::mouseRightPressedCallback, this);

    tab_buttons_.windowWasResized(this->GetSize());
    show();
}

WindowView::~WindowView()
{
    for (const auto& tab : tabs_)
    {
        delete tab.second;
    }
}

GuiElement* WindowView::getGuiElement(const std::string& element_name) const
{
    GuiElement* ge;
    for (const auto& tab : tabs_)
    {
        ge = tab.second->getGuiElement(element_name);
        if (ge != nullptr)
        {
            return ge;
        }
    }

    return nullptr;
}

void WindowView::notifyChildrenOnKeyPressed(const char key)
{
    for (const auto& tab : tabs_)
    {
        tab.second->notifyChildrenOnKeyPressed(key);
    }
}

void WindowView::notifyChildrenOnKeyReleased(const char key)
{
    for (const auto& tab : tabs_)
    {
        tab.second->notifyChildrenOnKeyReleased(key);
    }
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
            tab.second->show();
        }
        else
        {
            tab.second->hide();
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

/*void WindowView::newElement(const std::string& element_name)
{
    ElementSettings elem;
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
    gui_elements_[elem.name] = ge;
}*/

/*void WindowView::newElement()
{
    ElementSettings elem;
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
    gui_elements_[elem.name] = ge;
}*/

WindowSettings WindowView::getWindowSettings() const
{
    WindowSettings ws;
    const wxPoint pos = GetPosition();
    const wxSize size = GetSize();
    ws.name = name_;

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

void WindowView::editWindowName(wxCommandEvent& WXUNUSED(event))
{
    std::cout << "Event from editWindowName" << std::endl;
}

void WindowView::newWindow(wxCommandEvent& WXUNUSED(event))
{
    std::cout << "Event from newWindow" << std::endl;
    notify_main_window_new_window_();
}

void WindowView::newTab(wxCommandEvent& WXUNUSED(event))
{
    std::cout << "Event from newTab" << std::endl;

    TabSettings tab_settings;
    tab_settings.name = "Tab " + std::to_string(current_tab_num_);
    current_tab_num_++;

    tabs_[tab_settings.name] =
        new WindowTab(this,
                      tab_settings,
                      notify_main_window_key_pressed_,
                      notify_main_window_key_released_,
                      [this](const wxPoint pos) { mouseRightPressed(pos, ClickSource::GUI_ELEMENT); });
    tab_buttons_.addNewTab(tab_settings.name);
}

void WindowView::newElement(wxCommandEvent& WXUNUSED(event))
{
    std::cout << "Event from newElement" << std::endl;
}

void WindowView::editElementName(wxCommandEvent& WXUNUSED(event))
{
    std::cout << "Event from editElementName" << std::endl;
}

void WindowView::deleteElement(wxCommandEvent& WXUNUSED(event))
{
    std::cout << "Event from deleteElement" << std::endl;
}

void WindowView::editTabName(wxCommandEvent& WXUNUSED(event))
{
    std::cout << "Event from editTabName" << std::endl;
}

void WindowView::deleteTab(wxCommandEvent& WXUNUSED(event))
{
    std::cout << "Event from deleteTab" << std::endl;
}
