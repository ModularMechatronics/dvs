#include "tab_buttons.h"

TabButtons::TabButtons(wxFrame* parent,
                       const WindowSettings& window_settings,
                       const std::function<void(std::string)> tab_changed_callback,
                       const std::function<void(const wxPoint pos, const std::string& item_name)>&
                           notify_parent_window_right_mouse_pressed)
{
    tab_changed_callback_ = tab_changed_callback;
    tab_button_id_counter_ = 0;
    parent_ = parent;
    notify_parent_window_right_mouse_pressed_ = notify_parent_window_right_mouse_pressed;

    button_height_ = 30;
    button_width_ = 70;

    for (size_t k = 0; k < window_settings.tabs.size(); k++)
    {
        tab_buttons_.push_back(new TabButton(
            parent,
            window_settings.tabs[k],
            wxPoint(0, k * 20),
            wxSize(button_width_, button_height_),
            tab_button_id_counter_,
            [this](std::string name) {
                setSelection(name);
                tab_changed_callback_(name);
            },
            notify_parent_window_right_mouse_pressed));

        tab_button_id_counter_++;
    }
    if (tab_buttons_.size() > 0)
    {
        tab_buttons_[0]->setSelected();
    }
}

void TabButtons::deleteTabButton(const std::string& button_name)
{
    auto q = std::find_if(tab_buttons_.begin(), tab_buttons_.end(), [&button_name](const TabButton* const tb) -> bool {
        return tb->getButtonLabel() == button_name;
    });

    if (q != tab_buttons_.end())
    {
        delete (*q);
        tab_buttons_.erase(q);

        bool selected_button_exists = false;
        for (size_t k = 0; k < tab_buttons_.size(); k++)
        {
            if (tab_buttons_[k]->isSelected())
            {
                selected_button_exists = true;
            }
        }

        if (!selected_button_exists)
        {
            tab_buttons_[0]->setSelected();
        }
        layoutButtons();
    }
    else
    {
        throw std::runtime_error("Could not find button to delete!");
    }
}

void TabButtons::changeButtonName(const std::string& old_label, const std::string& new_label)
{
    const auto q =
        std::find_if(tab_buttons_.begin(), tab_buttons_.end(), [&old_label](const TabButton* const tb) -> bool {
            return tb->getButtonLabel() == old_label;
        });
    if (q == tab_buttons_.end())
    {
        throw std::runtime_error("Could not find a button to change name of!");
    }

    (*q)->setButtonLabel(new_label);
}

void TabButtons::setSelection(const std::string name)
{
    bool button_found = false;
    for (size_t k = 0; k < tab_buttons_.size(); k++)
    {
        if (tab_buttons_[k]->getButtonLabel() == name)
        {
            tab_buttons_[k]->setSelected();
            button_found = true;
        }
        else
        {
            tab_buttons_[k]->setDeselected();
        }
    }

    if (!button_found)
    {
        throw std::runtime_error("Could not find a button to change name of!");
    }
}

std::string TabButtons::getNameOfSelectedTab() const
{
    const auto q = std::find_if(
        tab_buttons_.begin(), tab_buttons_.end(), [](const TabButton* const tb) -> bool { return tb->isSelected(); });
    if (q == tab_buttons_.end())
    {
        throw std::runtime_error("Could not find a selected button!");
    }

    return (*q)->getButtonLabel();
}

void TabButtons::addNewTab(const std::string tab_name)
{
    TabSettings tab_settings;

    tab_settings.name = tab_name;

    tab_buttons_.push_back(new TabButton(
        parent_,
        tab_settings,
        wxPoint(0, tab_buttons_.size() * 20),
        wxSize(button_width_, button_height_),
        tab_button_id_counter_,
        [this](std::string name) {
            setSelection(name);
            tab_changed_callback_(name);
        },
        notify_parent_window_right_mouse_pressed_));

    tab_button_id_counter_++;

    layoutButtons();
}

void TabButtons::layoutButtons()
{
    const int y_offset = 30;

    if (tab_buttons_.size() == 1)
    {
        tab_buttons_[0]->hide();
    }
    else
    {
        for (size_t k = 0; k < tab_buttons_.size(); k++)
        {
            tab_buttons_[k]->show();
            tab_buttons_[k]->SetPosition(wxPoint(0, y_offset + k * button_height_));
        }
    }
}

void TabButtons::windowWasResized(const wxSize new_size)
{
    layoutButtons();
}