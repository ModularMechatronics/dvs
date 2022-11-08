#include "tab_buttons.h"

TabButtons::TabButtons(wxFrame* parent,
                       const WindowSettings& window_settings,
                       const std::function<void(std::string)> tab_changed_callback,
                       const std::function<void(const wxPoint pos, const std::string& item_name)>&
                           notify_parent_window_right_mouse_pressed)
{
    tab_changed_callback_ = tab_changed_callback;
    window_settings_ = window_settings;
    tab_button_id_counter_ = 0;
    parent_ = parent;
    notify_parent_window_right_mouse_pressed_ = notify_parent_window_right_mouse_pressed;

    button_height_ = 30;
    button_width_ = 70;

    for (size_t k = 0; k < window_settings_.tabs.size(); k++)
    {
        tab_buttons_.push_back(new TabButton(
            parent,
            window_settings_.tabs[k].name,
            [this](std::string name) {
                setSelection(name);
                tab_changed_callback_(name);
            },
            tab_button_id_counter_,
            wxPoint(0, k * 20),
            wxSize(button_width_, button_height_),
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

        if (tab_buttons_.size() > 0)
        {
            tab_buttons_[0]->setSelected();
        }
        layoutButtons();
    }
}

void TabButtons::changeButtonName(const std::string& old_label, const std::string& new_label)
{
    for (size_t k = 0; k < tab_buttons_.size(); k++)
    {
        if (tab_buttons_[k]->getButtonLabel() == old_label)
        {
            tab_buttons_[k]->setButtonLabel(new_label);
            break;
        }
    }
}

void TabButtons::setSelection(const std::string name)
{
    for (size_t k = 0; k < tab_buttons_.size(); k++)
    {
        if (tab_buttons_[k]->getButtonLabel() == name)
        {
            tab_buttons_[k]->setSelected();
        }
        else
        {
            tab_buttons_[k]->setDeselected();
        }
    }
}

std::string TabButtons::getNameOfSelectedTab() const
{
    auto q = std::find_if(
        tab_buttons_.begin(), tab_buttons_.end(), [](const TabButton* const tb) -> bool { return tb->isSelected(); });
    if (q == tab_buttons_.end())
    {
        throw std::runtime_error("Could not find a selected button!");
    }

    return (*q)->getButtonLabel();
}

void TabButtons::addNewTab(const std::string tab_name)
{
    tab_buttons_.push_back(new TabButton(
        parent_,
        tab_name,
        [this](std::string name) {
            setSelection(name);
            tab_changed_callback_(name);
        },
        tab_button_id_counter_,
        wxPoint(0, tab_buttons_.size() * 20),
        wxSize(button_width_, button_height_),
        notify_parent_window_right_mouse_pressed_));

    tab_button_id_counter_++;

    layoutButtons();
}

void TabButtons::layoutButtons()
{
    const int num_buttons = tab_buttons_.size();
    const int y_offset = 10;

    if (tab_buttons_.size() == 1)
    {
        tab_buttons_[0]->hide();
    }
    else
    {
        for (size_t k = 0; k < tab_buttons_.size(); k++)
        {
            tab_buttons_[k]->show();
            tab_buttons_[k]->SetPosition(wxPoint(0, y_offset + k * button_height_ * 2));
        }
    }
}

void TabButtons::windowWasResized(const wxSize new_size)
{
    window_size_ = new_size;

    layoutButtons();
}