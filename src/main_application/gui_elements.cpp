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
    control_pressed_at_mouse_press_ = false;
    previous_mouse_pos_ = wxPoint(0, 0);

    minimum_x_pos_ = 70;
    minimum_y_pos_ = 30;

    this->Bind(wxEVT_BUTTON, &ButtonGuiElement::buttonEvent, this);

    Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed_tmp, this);
    Bind(wxEVT_KEY_DOWN, &ApplicationGuiElement::keyPressedCallback_new, this);
    Bind(wxEVT_KEY_UP, &ApplicationGuiElement::keyReleasedCallback_new, this);

    Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);

    this->Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    this->Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    this->Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);
}

void ButtonGuiElement::buttonEvent(wxCommandEvent& event)
{
    sendGuiData();
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

    this->Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    this->Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    this->Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);
}

void SliderGuiElement::sliderEvent(wxCommandEvent& event)
{
    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    const std::int32_t new_value{this->GetValue()};
    if (new_value == slider_value_)
    {
        return;
    }

    slider_value_ = new_value;

    sendGuiData();
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

    control_pressed_at_mouse_press_ = false;
    previous_mouse_pos_ = wxPoint(0, 0);

    parent_size_ = parent->GetSize();
    this->Bind(wxEVT_CHECKBOX, &CheckboxGuiElement::checkBoxCallback, this);

    this->Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    this->Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    this->Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);
}

void CheckboxGuiElement::checkBoxCallback(wxCommandEvent& event)
{
    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    sendGuiData();
}
