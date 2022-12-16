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
                       const std::function<std::vector<std::string>(void)>& get_all_element_names,
                       const std::function<void(const GuiElement* const)>& notify_main_window_element_deleted)
    : wxFrame(main_window, wxID_ANY, "Figure 1"),
      tab_buttons_{this,
                   window_settings,
                   [this](const std::string name) { tabChanged(name); },
                   [this](const wxPoint pos, const std::string& item_name) {
                       mouseRightPressed(pos, ClickSource::TAB_BUTTON, item_name);
                   }},
      notify_main_window_key_pressed_{notify_main_window_key_pressed},
      notify_main_window_key_released_{notify_main_window_key_released},
      get_all_element_names_{get_all_element_names},
      notify_main_window_element_deleted_{notify_main_window_element_deleted},
      help_pane_{this, wxPoint(150, 150), wxSize(100, 100), this->GetSize()}
{
    main_window_ = main_window;
    current_tab_num_ = 0;
    callback_id_ = callback_id;
    dialog_color_ = RGBTripletf(0.5, 0.0, 0.0);

    this->SetLabel(window_settings.name);
    name_ = window_settings.name;

    if (window_settings.tabs.size() == 0)
    {
        element_x_offset_ = 0;
        TabSettings tab_settings;
        tab_settings.name = "Tab " + std::to_string(current_tab_num_);
        tabs_.push_back(new WindowTab(
            this,
            tab_settings,
            notify_main_window_key_pressed,
            notify_main_window_key_released,
            [this](const wxPoint pos, const std::string& item_name) {
                mouseRightPressed(pos, ClickSource::GUI_ELEMENT, item_name);
            },
            notify_main_window_element_deleted_));
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
            tabs_.push_back(new WindowTab(
                this,
                window_settings.tabs[k],
                notify_main_window_key_pressed,
                notify_main_window_key_released,
                [this](const wxPoint pos, const std::string& item_name) {
                    mouseRightPressed(pos, ClickSource::GUI_ELEMENT, item_name);
                },
                notify_main_window_element_deleted_));
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
    Bind(wxEVT_MENU, &MainWindow::newWindow, static_cast<MainWindow*>(main_window_), callback_id_ + 2);
    Bind(wxEVT_MENU, &WindowView::newTab, this, dvs_ids::NEW_TAB);
    Bind(wxEVT_MENU, &WindowView::newElement, this, dvs_ids::NEW_ELEMENT);
    Bind(wxEVT_MENU, &WindowView::editElementName, this, dvs_ids::EDIT_ELEMENT_NAME);
    Bind(wxEVT_MENU, &WindowView::deleteElement, this, dvs_ids::DELETE_ELEMENT);
    Bind(wxEVT_MENU, &WindowView::raiseElement, this, dvs_ids::RAISE_ELEMENT);
    Bind(wxEVT_MENU, &WindowView::lowerElement, this, dvs_ids::LOWER_ELEMENT);
    Bind(wxEVT_MENU, &WindowView::editTabName, this, dvs_ids::EDIT_TAB_NAME);
    Bind(wxEVT_MENU, &WindowView::deleteTab, this, dvs_ids::DELETE_TAB);
    Bind(wxEVT_KEY_DOWN, &WindowView::keyPressedCallback, this);
    Bind(wxEVT_KEY_UP, &WindowView::keyReleasedCallback, this);

    Bind(wxEVT_CLOSE_WINDOW, &WindowView::OnClose, this);
    Bind(wxEVT_SIZE, &WindowView::OnSize, this);
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

void WindowView::childModified(wxCommandEvent& WXUNUSED(event))
{
    // wxCommandEvent parent_event(GUI_ELEMENT_CHANGED_EVENT);
    // wxPostEvent(this->GetParent(), parent_event);
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
}

int WindowView::getCallbackId() const
{
    return callback_id_;
}

void WindowView::setName(const std::string& new_name)
{
    this->SetLabel(new_name);
    name_ = new_name;
}

void WindowView::OnClose(wxCloseEvent& WXUNUSED(event))
{
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
        name_ = window_name;
        this->SetLabel(name_);
    }
}

void WindowView::newTab(wxCommandEvent& WXUNUSED(event))
{
    TabSettings tab_settings;
    tab_settings.name = "Tab " + std::to_string(current_tab_num_);
    current_tab_num_++;
    element_x_offset_ = 70;

    tabs_.push_back(new WindowTab(
        this,
        tab_settings,
        notify_main_window_key_pressed_,
        notify_main_window_key_released_,
        [this](const wxPoint pos, const std::string& item_name) {
            mouseRightPressed(pos, ClickSource::GUI_ELEMENT, item_name);
        },
        notify_main_window_element_deleted_));
    tab_buttons_.addNewTab(tab_settings.name);

    for (const auto& t : tabs_)
    {
        t->setMinXPos(element_x_offset_);
    }
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
    for (const auto& t : tabs_)
    {
        t->changeNameOfElementIfElementExists(last_clicked_item_, new_name);
    }
}

void WindowView::deleteElement(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->deleteElementIfItExists(last_clicked_item_);
    }
}

void WindowView::raiseElement(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->raiseElement(last_clicked_item_);
    }
}

void WindowView::lowerElement(wxCommandEvent& WXUNUSED(event))
{
    for (const auto& t : tabs_)
    {
        t->lowerElement(last_clicked_item_);
    }
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
        if (tabs_.size() == 1)
        {
            element_x_offset_ = 0;
            tabs_[0]->setMinXPos(element_x_offset_);
        }
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
}
