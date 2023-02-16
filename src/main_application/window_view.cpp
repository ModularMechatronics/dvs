#include "window_view.h"

#include "events.h"
#include "globals.h"
#include "main_window.h"
#include "plot_pane.h"

WindowView::WindowView(
    wxFrame* main_window,
    const WindowSettings& window_settings,
    const std::string& project_name,
    const int callback_id,
    const bool project_is_saved,
    const std::function<void(const char key)>& notify_main_window_key_pressed,
    const std::function<void(const char key)>& notify_main_window_key_released,
    const std::function<std::vector<std::string>(void)>& get_all_element_names,
    const std::function<void(const std::string&)>& notify_main_window_element_deleted,
    const std::function<void(const std::string&, const std::string&)>& notify_main_window_element_name_changed,
    const std::function<void()>& notify_main_window_about_modification)
    : wxFrame(main_window, wxID_ANY, "Figure 1"),
      tab_buttons_{this,
                   window_settings,
                   [this](const std::string name) { tabChanged(name); },
                   [this](const wxPoint pos, const std::string& item_name) {
                       mouseRightPressed(pos, ClickSource::TAB_BUTTON, item_name);
                   }},
      notify_main_window_key_pressed_{notify_main_window_key_pressed},
      notify_main_window_key_released_{notify_main_window_key_released},
      notify_main_window_element_deleted_{notify_main_window_element_deleted},
      get_all_element_names_{get_all_element_names},
      notify_main_window_element_name_changed_{notify_main_window_element_name_changed},
      notify_main_window_about_modification_{notify_main_window_about_modification},
      help_pane_{this, wxPoint(150, 150), wxSize(100, 100), this->GetSize()}
{
    project_name_ = project_name;
    main_window_ = main_window;
    current_tab_num_ = 0;
    callback_id_ = callback_id;
    dialog_color_ = RGBTripletf(0.5, 0.0, 0.0);
    project_is_saved_ = project_is_saved;

    notify_parent_window_right_mouse_pressed_ = [this](const wxPoint pos, const std::string& item_name) {
        mouseRightPressed(pos, ClickSource::GUI_ELEMENT, item_name);
    };

    name_ = window_settings.name;
    updateLabel();

    if (window_settings.tabs.size() == 0)
    {
        element_x_offset_ = 0;
        TabSettings tab_settings;
        tab_settings.name = "Tab " + std::to_string(current_tab_num_);
        tabs_.push_back(new WindowTab(this,
                                      tab_settings,
                                      element_x_offset_,
                                      notify_main_window_key_pressed,
                                      notify_main_window_key_released,
                                      notify_parent_window_right_mouse_pressed_,
                                      notify_main_window_element_deleted_,
                                      notify_main_window_about_modification_));
        tab_buttons_.addNewTab(tab_settings.name);
        tab_buttons_.setSelection(tab_settings.name);
        SetBackgroundColour(wxColour(tab_settings.background_color.red * 255.0f,
                                     tab_settings.background_color.green * 255.0f,
                                     tab_settings.background_color.blue * 255.0f));
        current_tab_num_++;
    }
    else
    {
        if (window_settings.tabs.size() == 1)
        {
            element_x_offset_ = 0;
        }
        else
        {
            element_x_offset_ = 70;
        }

        for (size_t k = 0; k < window_settings.tabs.size(); k++)
        {
            tabs_.push_back(new WindowTab(this,
                                          window_settings.tabs[k],
                                          element_x_offset_,
                                          notify_main_window_key_pressed,
                                          notify_main_window_key_released,
                                          notify_parent_window_right_mouse_pressed_,
                                          notify_main_window_element_deleted_,
                                          notify_main_window_about_modification_));
            current_tab_num_++;
        }
    }

    if (window_settings.tabs.size() > 0)
    {
        for (const auto& tab : tabs_)
        {
            if (tab->getName() == window_settings.tabs[0].name)
            {
                tab->show();
                const auto c = tab->getBackgroundColor();
                SetBackgroundColour(wxColour(c.red * 255.0f, c.green * 255.0f, c.blue * 255.0f));
            }
            else
            {
                tab->hide();
            }
            tab->setMinXPos(element_x_offset_);
        }
    }

    popup_menu_window_ = new wxMenu(wxT(""));
    popup_menu_element_ = new wxMenu(wxT(""));
    popup_menu_tab_ = new wxMenu(wxT(""));

    popup_menu_window_->AppendSeparator();
    popup_menu_window_->Append(dvs_ids::EDIT_WINDOW_NAME, wxT("Edit window name"));
    popup_menu_window_->Append(callback_id_ + 1, wxT("Delete window"));
    popup_menu_window_->AppendSeparator();
    popup_menu_window_->Append(callback_id_ + 2, wxT("New window"));
    popup_menu_window_->Append(dvs_ids::NEW_TAB, wxT("New tab"));
    popup_menu_window_->Append(dvs_ids::NEW_ELEMENT, wxT("New element"));

    popup_menu_element_->Append(dvs_ids::EDIT_ELEMENT_NAME, wxT("Edit element name"));
    popup_menu_element_->Append(dvs_ids::DELETE_ELEMENT, wxT("Delete element"));
    popup_menu_element_->Append(dvs_ids::TOGGLE_PROJECTION_TYPE, wxT("Toggle projection type"));
    popup_menu_element_->Append(dvs_ids::RAISE_ELEMENT, wxT("Raise"));
    popup_menu_element_->Append(dvs_ids::LOWER_ELEMENT, wxT("Lower"));
    popup_menu_element_->AppendSeparator();
    popup_menu_element_->Append(dvs_ids::EDIT_WINDOW_NAME, wxT("Edit window name"));
    popup_menu_element_->Append(callback_id_ + 1, wxT("Delete window"));
    popup_menu_element_->AppendSeparator();
    popup_menu_element_->Append(callback_id_ + 2, wxT("New window"));
    popup_menu_element_->Append(dvs_ids::NEW_TAB, wxT("New tab"));
    popup_menu_element_->Append(dvs_ids::NEW_ELEMENT, wxT("New element"));

    popup_menu_tab_->Append(dvs_ids::EDIT_TAB_NAME, wxT("Edit tab name"));
    popup_menu_tab_->Append(dvs_ids::DELETE_TAB, wxT("Delete tab"));
    popup_menu_tab_->AppendSeparator();
    popup_menu_tab_->Append(dvs_ids::EDIT_WINDOW_NAME, wxT("Edit window name"));
    popup_menu_tab_->Append(callback_id_ + 1, wxT("Delete window"));
    popup_menu_tab_->AppendSeparator();
    popup_menu_tab_->Append(callback_id_ + 2, wxT("New window"));
    popup_menu_tab_->Append(dvs_ids::NEW_TAB, wxT("New tab"));
    popup_menu_tab_->Append(dvs_ids::NEW_ELEMENT, wxT("New element"));

    Bind(wxEVT_MENU, &WindowView::editWindowName, this, dvs_ids::EDIT_WINDOW_NAME);
    Bind(wxEVT_MENU, &MainWindow::deleteWindow, static_cast<MainWindow*>(main_window_), callback_id_ + 1);
    Bind(wxEVT_MENU, &MainWindow::newWindowCallback, static_cast<MainWindow*>(main_window_), callback_id_ + 2);
    Bind(wxEVT_MENU, &WindowView::newTab, this, dvs_ids::NEW_TAB);
    Bind(wxEVT_MENU, &WindowView::newElement, this, dvs_ids::NEW_ELEMENT);
    Bind(wxEVT_MENU, &WindowView::editElementName, this, dvs_ids::EDIT_ELEMENT_NAME);
    Bind(wxEVT_MENU, &WindowView::deleteElement, this, dvs_ids::DELETE_ELEMENT);

    Bind(wxEVT_MENU, &WindowView::toggleProjectionType, this, dvs_ids::TOGGLE_PROJECTION_TYPE);
    Bind(wxEVT_MENU, &WindowView::raiseElement, this, dvs_ids::RAISE_ELEMENT);
    Bind(wxEVT_MENU, &WindowView::lowerElement, this, dvs_ids::LOWER_ELEMENT);
    Bind(wxEVT_MENU, &WindowView::editTabName, this, dvs_ids::EDIT_TAB_NAME);
    Bind(wxEVT_MENU, &WindowView::deleteTab, this, dvs_ids::DELETE_TAB);
    Bind(wxEVT_KEY_DOWN, &WindowView::keyPressedCallback, this);
    Bind(wxEVT_KEY_UP, &WindowView::keyReleasedCallback, this);

    Bind(wxEVT_CLOSE_WINDOW, &WindowView::OnClose, this);
    Bind(wxEVT_SIZE, &WindowView::OnSize, this);
    Bind(wxEVT_MOVE, &WindowView::OnMove, this);

    Bind(wxEVT_RIGHT_DOWN, &WindowView::mouseRightPressedCallback, this);

    tab_buttons_.windowWasResized(this->GetSize());

    Show();

    this->SetPosition(wxPoint(window_settings.x, window_settings.y));
    this->SetSize(wxSize(window_settings.width, window_settings.height));
}

WindowView::~WindowView()
{
    for (const auto& tab : tabs_)
    {
        delete tab;
    }
}

void WindowView::deleteAllTabs()
{
    for (const auto& tab : tabs_)
    {
        delete tab;
    }

    tabs_.clear();
}

void WindowView::setProjectName(const std::string& project_name)
{
    project_name_ = project_name;
}

std::vector<GuiElement*> WindowView::getGuiElements() const
{
    std::vector<GuiElement*> gui_elements;

    for (const auto& tab : tabs_)
    {
        std::vector<GuiElement*> tab_gui_elements = tab->getGuiElements();
        gui_elements.insert(gui_elements.end(), tab_gui_elements.begin(), tab_gui_elements.end());
    }

    return gui_elements;
}

GuiElement* WindowView::getGuiElement(const std::string& element_name) const
{
    GuiElement* ge;
    for (const auto& tab : tabs_)
    {
        ge = tab->getGuiElement(element_name);
        if (ge != nullptr)
        {
            return ge;
        }
    }

    return nullptr;
}

void WindowView::keyPressedCallback(wxKeyEvent& evt)
{
    // TODO: This callback doesn't work
    const int key = evt.GetUnicodeKey();
    if (key == 'H')
    {
        help_pane_.show();
    }
    std::cout << "Key pressed" << std::endl;
}

void WindowView::keyReleasedCallback(wxKeyEvent& evt)
{
    // TODO: This callback doesn't work
    const int key = evt.GetUnicodeKey();
    if (key == 'H')
    {
        help_pane_.hide();
    }
}

void WindowView::notifyChildrenOnKeyPressed(const char key)
{
    if (key == 'H')
    {
        help_pane_.show();
    }

    for (const auto& tab : tabs_)
    {
        tab->notifyChildrenOnKeyPressed(key);
    }
}

void WindowView::notifyChildrenOnKeyReleased(const char key)
{
    if (key == 'H')
    {
        help_pane_.hide();
    }

    for (const auto& tab : tabs_)
    {
        tab->notifyChildrenOnKeyReleased(key);
    }
}

void WindowView::mouseRightPressed(const wxPoint pos, const ClickSource source, const std::string& item_name)
{
    last_clicked_item_ = item_name;

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
    mouseRightPressed(event.GetPosition(), ClickSource::THIS, "");
}

void WindowView::tabChanged(const std::string name)
{
    for (const auto& tab : tabs_)
    {
        if (tab->getName() == name)
        {
            tab->show();
            const auto c = tab->getBackgroundColor();
            SetBackgroundColour(wxColour(c.red * 255.0f, c.green * 255.0f, c.blue * 255.0f));
            Refresh();
        }
        else
        {
            tab->hide();
        }
    }
}

void WindowView::OnMove(wxMoveEvent& event)
{
    notify_main_window_about_modification_();
}

void WindowView::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    const wxSize new_size = event.GetSize();

    tab_buttons_.windowWasResized(new_size);

    for (const auto& tab : tabs_)
    {
        tab->updateSizeFromParent(new_size);
    }

    help_pane_.updateParentSize(new_size);

    notify_main_window_about_modification_();
}

int WindowView::getCallbackId() const
{
    return callback_id_;
}

void WindowView::updateLabel()
{
    std::string save_string = "";
    if (!project_is_saved_)
    {
        save_string = "*";
    }
    this->SetLabel(name_ + " [" + project_name_ + save_string + "]");
}

void WindowView::setName(const std::string& new_name)
{
    name_ = new_name;
    updateLabel();
}

void WindowView::OnClose(wxCloseEvent& WXUNUSED(event))
{
    notify_main_window_about_modification_();
    Hide();
}

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
        ws.tabs.push_back(t->getTabSettings());
    }

    return ws;
}

std::string WindowView::getName() const
{
    return name_;
}

void WindowView::editWindowName(wxCommandEvent& WXUNUSED(event))
{
    wxTextEntryDialog name_dialog(this, "Enter the name for the window", "Enter window name", name_);

    name_dialog.SetBackgroundColour(
        wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));

    if (name_dialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    const std::string window_name{name_dialog.GetValue().mb_str()};

    if (window_name.length() > 0)
    {
        setName(window_name);
        notify_main_window_about_modification_();
    }
}

void WindowView::setIsFileSavedForLabel(const bool is_saved)
{
    project_is_saved_ = is_saved;
    updateLabel();
}

void WindowView::newTab(wxCommandEvent& WXUNUSED(event))
{
    TabSettings tab_settings;
    tab_settings.name = "Tab " + std::to_string(current_tab_num_);
    current_tab_num_++;
    element_x_offset_ = 70;

    tabs_.push_back(new WindowTab(this,
                                  tab_settings,
                                  element_x_offset_,
                                  notify_main_window_key_pressed_,
                                  notify_main_window_key_released_,
                                  notify_parent_window_right_mouse_pressed_,
                                  notify_main_window_element_deleted_,
                                  notify_main_window_about_modification_));
    tab_buttons_.addNewTab(tab_settings.name);

    for (const auto& t : tabs_)
    {
        t->setMinXPos(element_x_offset_);
    }
    notify_main_window_about_modification_();
}

void WindowView::newElement(wxCommandEvent& WXUNUSED(event))
{
    const std::string selected_tab = tab_buttons_.getNameOfSelectedTab();

    auto q = std::find_if(tabs_.begin(), tabs_.end(), [&selected_tab](const WindowTab* const tb) -> bool {
        return selected_tab == tb->getName();
    });

    if (q != tabs_.end())
    {
        wxTextEntryDialog name_dialog(
            this, "Enter the name for the new element", "Enter element name", "<element-name>");

        name_dialog.SetBackgroundColour(
            wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));
        std::string element_name;

        while (true)
        {
            if (name_dialog.ShowModal() == wxID_CANCEL)
            {
                return;
            }

            element_name = name_dialog.GetValue().mb_str();

            if (element_name.length() == 0)
            {
                wxMessageDialog dlg(&name_dialog, "Can't have an empty element name!", "Invalid name!");
                dlg.SetBackgroundColour(
                    wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));
                dlg.ShowModal();
                continue;
            }

            const std::vector<std::string> all_element_names = get_all_element_names_();

            auto q = std::find(all_element_names.begin(), all_element_names.end(), element_name);

            const bool element_exists = q != all_element_names.end();

            if (element_exists)
            {
                wxMessageDialog dlg(
                    &name_dialog, "Choose a unique name", "Element name \"" + element_name + "\" exists!");
                dlg.SetBackgroundColour(
                    wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));
                dlg.ShowModal();
            }
            else
            {
                break;
            }
        }

        (*q)->newElement(element_name);
        notify_main_window_about_modification_();
    }
}

std::vector<std::string> WindowView::getElementNames() const
{
    std::vector<std::string> element_names;
    for (const auto& t : tabs_)
    {
        const std::vector<std::string> tab_element_names = t->getElementNames();

        element_names.insert(element_names.end(), tab_element_names.begin(), tab_element_names.end());
    }

    return element_names;
}

void WindowView::editElementName(wxCommandEvent& WXUNUSED(event))
{
    wxTextEntryDialog name_dialog(this, "Enter the new name for the element", "Edit element name", last_clicked_item_);

    name_dialog.SetBackgroundColour(
        wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));

    std::string new_name;

    while (true)
    {
        if (name_dialog.ShowModal() == wxID_CANCEL)
        {
            return;
        }

        new_name = name_dialog.GetValue().mb_str();

        if (new_name == last_clicked_item_)
        {
            return;
        }
        else
        {
            new_name = name_dialog.GetValue().mb_str();

            if (new_name.length() == 0)
            {
                wxMessageDialog dlg(&name_dialog, "Can't have an empty element name!", "Invalid name!");
                dlg.SetBackgroundColour(
                    wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));
                dlg.ShowModal();
                continue;
            }

            const std::vector<std::string> all_element_names = get_all_element_names_();

            auto q = std::find(all_element_names.begin(), all_element_names.end(), new_name);

            const bool element_exists = q != all_element_names.end();

            if (element_exists)
            {
                wxMessageDialog dlg(&name_dialog, "Choose a unique name", "Element name \"" + new_name + "\" exists!");
                dlg.SetBackgroundColour(
                    wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));
                dlg.ShowModal();
            }
            else
            {
                break;
            }
        }
    }

    bool name_changed = false;
    for (const auto& t : tabs_)
    {
        name_changed = name_changed || t->changeNameOfElementIfElementExists(last_clicked_item_, new_name);
    }

    if (name_changed)
    {
        notify_main_window_element_name_changed_(last_clicked_item_, new_name);
    }
}

void WindowView::updateAllElements()
{
    for (auto& t : tabs_)
    {
        t->updateAllElements();
    }
}

void WindowView::deleteElement(wxCommandEvent& WXUNUSED(event))
{
    bool element_deleted = false;

    for (const auto& t : tabs_)
    {
        element_deleted = element_deleted || t->deleteElementIfItExists(last_clicked_item_);
    }

    if (element_deleted)
    {
        notify_main_window_about_modification_();
    }
}

void WindowView::toggleProjectionType(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->toggleProjectionType(last_clicked_item_);
    }

    notify_main_window_about_modification_();
}

void WindowView::raiseElement(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->raiseElement(last_clicked_item_);
    }

    notify_main_window_about_modification_();
}

void WindowView::lowerElement(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->lowerElement(last_clicked_item_);
    }

    notify_main_window_about_modification_();
}

void WindowView::editTabName(wxCommandEvent& WXUNUSED(event))
{
    wxTextEntryDialog name_dialog(this, "Enter the new name for tab", "Enter tab name", last_clicked_item_);

    name_dialog.SetBackgroundColour(
        wxColour(dialog_color_.red * 255.0f, dialog_color_.green * 255.0f, dialog_color_.blue * 255.0f));

    if (name_dialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }

    const std::string tab_name{name_dialog.GetValue().mb_str()};

    if (tab_name.length() == 0)
    {
        return;
    }

    auto q = std::find_if(tabs_.begin(), tabs_.end(), [this](const WindowTab* const tb) -> bool {
        return last_clicked_item_ == tb->getName();
    });

    if (q != tabs_.end())
    {
        (*q)->setName(tab_name);
        notify_main_window_about_modification_();
    }
    tab_buttons_.changeButtonName(last_clicked_item_, tab_name);
}

void WindowView::deleteTab(wxCommandEvent& WXUNUSED(event))
{
    auto q = std::find_if(tabs_.begin(), tabs_.end(), [this](const WindowTab* const tb) -> bool {
        return last_clicked_item_ == tb->getName();
    });

    if (q != tabs_.end())
    {
        delete (*q);
        tabs_.erase(q);
    }

    tab_buttons_.deleteTabButton(last_clicked_item_);

    if (tabs_.size() > 1)
    {
        const std::string selected_button = tab_buttons_.getNameOfSelectedTab();
        for (const auto& tab : tabs_)
        {
            if (tab->getName() == selected_button)
            {
                tab->show();
            }
            else
            {
                tab->hide();
            }
        }
    }
    else
    {
        element_x_offset_ = 0;
        tabs_[0]->setMinXPos(element_x_offset_);
        tabs_[0]->show();
    }

    notify_main_window_about_modification_();
}
