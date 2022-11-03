#include "tab_buttons.h"

TabButtons::TabButtons(wxFrame* parent,
                       const WindowSettings& window_settings,
                       const std::function<void(std::string)> tab_changed_callback,
                       const std::function<void(const wxPoint pos)>& notify_parent_window_right_mouse_pressed)
{
    tab_changed_callback_ = tab_changed_callback;
    window_settings_ = window_settings;
    tab_button_id_counter_ = 0;

    button_height_ = 30;
    button_width_ = 70;

    for (size_t k = 0; k < window_settings_.tabs.size(); k++)
    {
        tab_buttons_.push_back(new TabButton(
            parent,
            window_settings_.tabs[k].name,
            [this](std::string name) {
                // setSelection(id);
                tab_changed_callback_(name);
            },
            tab_button_id_counter_,
            wxPoint(0, k * 20),
            wxSize(button_width_, button_height_),
            notify_parent_window_right_mouse_pressed));

        tab_button_id_counter_++;
    }
}

void TabButtons::setSelection(const std::string name)
{
    /*for (size_t k = 0; k < tab_buttons_.size(); k++)
    {
        if (tab_buttons_[k]->getId() == id)
        {
            tab_buttons_[k]->setSelected();
        }
        else
        {
            tab_buttons_[k]->setDeselected();
        }
    }*/
}

void TabButtons::layoutButtons()
{
    const int num_buttons = tab_buttons_.size();
    const int y_offset = 10;

    for (size_t k = 0; k < tab_buttons_.size(); k++)
    {
        tab_buttons_[k]->SetPosition(wxPoint(0, y_offset + k * button_height_ * 2));
    }
}

void TabButtons::windowWasResized(const wxSize new_size)
{
    window_size_ = new_size;

    layoutButtons();
}