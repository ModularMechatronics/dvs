#include "gui_elements.h"

ButtonGuiElement::ButtonGuiElement(wxFrame* parent,
                                   const std::shared_ptr<ElementSettings>& element_settings,
                                   const std::function<void(const char key)>& notify_main_window_key_pressed,
                                   const std::function<void(const char key)>& notify_main_window_key_released,
                                   const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                                       notify_parent_window_right_mouse_pressed,
                                   const std::function<void()>& notify_main_window_about_modification,
                                   const wxPoint& pos,
                                   const wxSize& size)
    : wxButton(parent, wxID_ANY, std::dynamic_pointer_cast<ButtonSettings>(element_settings)->label, pos, size),
      ApplicationGuiElement(element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification)
{
    parent_size_ = parent->GetSize();
    is_pressed_ = false;
    control_pressed_at_mouse_down_ = false;
    previous_mouse_pos_ = wxPoint(0, 0);

    minimum_x_pos_ = 70;
    minimum_y_pos_ = 30;

    this->Bind(wxEVT_BUTTON, &ButtonGuiElement::buttonEvent, this);

    this->Bind(wxEVT_LEFT_DOWN, &ButtonGuiElement::mouseLeftPressed, this);
    this->Bind(wxEVT_LEFT_UP, &ButtonGuiElement::mouseLeftReleased, this);
    this->Bind(wxEVT_MOTION, &ButtonGuiElement::mouseMovedOverItem, this);
}

void ButtonGuiElement::buttonEvent(wxCommandEvent& event)
{
    sendGuiData();
}

void ButtonGuiElement::mouseMovedOverItem(wxMouseEvent& event)
{
    if (control_pressed_at_mouse_down_ && event.LeftIsDown())
    {
        const wxPoint current_mouse_position_local = event.GetPosition();
        const wxPoint current_mouse_position_global = current_mouse_position_local + this->GetPosition();
        const wxPoint delta = current_mouse_position_global - previous_mouse_pos_;
        this->SetPosition(this->GetPosition() + delta);

        notify_main_window_about_modification_();

        element_settings_->x = this->GetPosition().x / static_cast<float>(parent_size_.x);
        element_settings_->y = this->GetPosition().y / static_cast<float>(parent_size_.y);

        previous_mouse_pos_ = current_mouse_position_global;
    }
    else
    {
        event.Skip();
    }
}

void ButtonGuiElement::mouseLeftReleased(wxMouseEvent& event)
{
    if (control_pressed_at_mouse_down_)
    {
        control_pressed_at_mouse_down_ = false;
    }
    else
    {
        event.Skip();

        is_pressed_ = false;
    }
}

void ButtonGuiElement::mouseLeftPressed(wxMouseEvent& event)
{
    if (wxGetKeyState(WXK_CONTROL))
    {
        control_pressed_at_mouse_down_ = true;

        const wxPoint pos_at_press = this->GetPosition();

        mouse_pos_at_press_ = event.GetPosition() + pos_at_press;
        previous_mouse_pos_ = mouse_pos_at_press_;
    }
    else
    {
        event.Skip();
        is_pressed_ = true;
    }
}

SliderGuiElement::SliderGuiElement(wxFrame* parent,
                                   const std::shared_ptr<ElementSettings>& element_settings,
                                   const std::function<void(const char key)>& notify_main_window_key_pressed,
                                   const std::function<void(const char key)>& notify_main_window_key_released,
                                   const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                                       notify_parent_window_right_mouse_pressed,
                                   const std::function<void()>& notify_main_window_about_modification,
                                   const wxPoint& pos,
                                   const wxSize& size)
    : wxSlider(parent,
               wxID_ANY,
               std::dynamic_pointer_cast<SliderSettings>(element_settings)->init_value,
               std::dynamic_pointer_cast<SliderSettings>(element_settings)->min_value,
               std::dynamic_pointer_cast<SliderSettings>(element_settings)->max_value,
               pos,
               size),
      ApplicationGuiElement(element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification),
      slider_value_{std::dynamic_pointer_cast<SliderSettings>(element_settings)->init_value}
{
    minimum_x_pos_ = 70;
    minimum_y_pos_ = 30;

    parent_size_ = parent->GetSize();
    Bind(wxEVT_SLIDER, &SliderGuiElement::sliderEvent, this);

    this->Bind(wxEVT_LEFT_DOWN, &SliderGuiElement::mouseLeftPressed, this);
    this->Bind(wxEVT_LEFT_UP, &SliderGuiElement::mouseLeftReleased, this);
    this->Bind(wxEVT_MOTION, &SliderGuiElement::mouseMovedOverItem, this);
}

void SliderGuiElement::sliderEvent(wxCommandEvent& event)
{
    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    const std::int32_t new_value{this->GetValue()};
    if(new_value == slider_value_)
    {
        return;
    }

    slider_value_ = new_value;

    sendGuiData();
}

void SliderGuiElement::mouseMovedOverItem(wxMouseEvent& event)
{
    if (control_pressed_at_mouse_down_ && event.LeftIsDown())
    {
        const wxPoint current_mouse_position_local = event.GetPosition();
        const wxPoint current_mouse_position_global = current_mouse_position_local + this->GetPosition();
        const wxPoint delta = current_mouse_position_global - previous_mouse_pos_;
        this->SetPosition(this->GetPosition() + delta);

        notify_main_window_about_modification_();

        element_settings_->x = this->GetPosition().x / static_cast<float>(parent_size_.x);
        element_settings_->y = this->GetPosition().y / static_cast<float>(parent_size_.y);

        previous_mouse_pos_ = current_mouse_position_global;
    }
    else
    {
        event.Skip();
    }
}

void SliderGuiElement::mouseLeftReleased(wxMouseEvent& event)
{
    if (control_pressed_at_mouse_down_)
    {
        control_pressed_at_mouse_down_ = false;
    }
    else
    {
        event.Skip();
    }
}

void SliderGuiElement::mouseLeftPressed(wxMouseEvent& event)
{
    if (wxGetKeyState(WXK_CONTROL))
    {
        control_pressed_at_mouse_down_ = true;

        const wxPoint pos_at_press = this->GetPosition();

        mouse_pos_at_press_ = event.GetPosition() + pos_at_press;
        previous_mouse_pos_ = mouse_pos_at_press_;
    }
    else
    {
        event.Skip();
    }
}

CheckboxGuiElement::CheckboxGuiElement(wxFrame* parent,
                                       const std::shared_ptr<ElementSettings>& element_settings,
                                       const std::function<void(const char key)>& notify_main_window_key_pressed,
                                       const std::function<void(const char key)>& notify_main_window_key_released,
                                       const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                                           notify_parent_window_right_mouse_pressed,
                                       const std::function<void()>& notify_main_window_about_modification,
                                       const wxPoint& pos,
                                       const wxSize& size)
    : wxCheckBox(parent, wxID_ANY, std::dynamic_pointer_cast<CheckBoxSettings>(element_settings)->label, pos, size),
      ApplicationGuiElement(element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification)
{
    minimum_x_pos_ = 70;
    minimum_y_pos_ = 30;

    control_pressed_at_mouse_down_ = false;
    previous_mouse_pos_ = wxPoint(0, 0);

    parent_size_ = parent->GetSize();
    this->Bind(wxEVT_CHECKBOX, &CheckboxGuiElement::checkBoxCallback, this);

    this->Bind(wxEVT_LEFT_DOWN, &CheckboxGuiElement::mouseLeftPressed, this);
    this->Bind(wxEVT_LEFT_UP, &CheckboxGuiElement::mouseLeftReleased, this);
    this->Bind(wxEVT_MOTION, &CheckboxGuiElement::mouseMovedOverItem, this);
}


void CheckboxGuiElement::mouseMovedOverItem(wxMouseEvent& event)
{
    if (control_pressed_at_mouse_down_ && event.LeftIsDown())
    {
        const wxPoint current_mouse_position_local = event.GetPosition();
        const wxPoint current_mouse_position_global = current_mouse_position_local + this->GetPosition();
        const wxPoint delta = current_mouse_position_global - previous_mouse_pos_;
        this->SetPosition(this->GetPosition() + delta);

        notify_main_window_about_modification_();

        element_settings_->x = this->GetPosition().x / static_cast<float>(parent_size_.x);
        element_settings_->y = this->GetPosition().y / static_cast<float>(parent_size_.y);

        previous_mouse_pos_ = current_mouse_position_global;
    }
    else
    {
        event.Skip();
    }
}

void CheckboxGuiElement::mouseLeftReleased(wxMouseEvent& event)
{
    if (control_pressed_at_mouse_down_)
    {
        control_pressed_at_mouse_down_ = false;
    }
    else
    {
        event.Skip();
    }
}

void CheckboxGuiElement::mouseLeftPressed(wxMouseEvent& event)
{
    if (wxGetKeyState(WXK_CONTROL))
    {
        control_pressed_at_mouse_down_ = true;

        const wxPoint pos_at_press = this->GetPosition();

        mouse_pos_at_press_ = event.GetPosition() + pos_at_press;
        previous_mouse_pos_ = mouse_pos_at_press_;
    }
    else
    {
        event.Skip();
    }
}

void CheckboxGuiElement::checkBoxCallback(wxCommandEvent& event)
{
    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    sendGuiData();
}
