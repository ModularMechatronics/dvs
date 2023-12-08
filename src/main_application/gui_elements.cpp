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
    : wxButton(parent,
               wxID_ANY,
               std::dynamic_pointer_cast<ButtonSettings>(element_settings)->label,
               pos,
               size,
               wxBU_EXACTFIT),
      ApplicationGuiElement(element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification)
{
    is_pressed_ = false;

    this->Bind(wxEVT_BUTTON, &ButtonGuiElement::buttonEvent, this);

    Bind(wxEVT_KEY_DOWN, &ApplicationGuiElement::keyPressedCallback_new, this);
    Bind(wxEVT_KEY_UP, &ApplicationGuiElement::keyReleasedCallback_new, this);

    Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);

    Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);

    Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed, this);
}

void ButtonGuiElement::buttonEvent(wxCommandEvent& event)
{
    sendGuiData();
}

void ButtonGuiElement::setLabel(const std::string& new_label)
{
    CallAfter(&ButtonGuiElement::SetLabel, new_label);
}

void ButtonGuiElement::setElementSettings(const std::map<std::string, std::string>& new_settings)
{
    ButtonSettings* button_settings = dynamic_cast<ButtonSettings*>(element_settings_.get());

    if (new_settings.count("label") > 0U)
    {
        this->SetLabel(new_settings.at("label"));
        button_settings->label = new_settings.at("label");
    }
    element_settings_->handle_string = new_settings.at("handle_string");
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
    Bind(wxEVT_SLIDER, &SliderGuiElement::sliderEvent, this);

    this->Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    this->Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    this->Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);

    Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);

    Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed, this);
}

void SliderGuiElement::sliderEvent(wxCommandEvent& event)
{
    const std::int32_t new_value{this->GetValue()};
    if (new_value == slider_value_)
    {
        return;
    }

    slider_value_ = new_value;

    sendGuiData();
}

void SliderGuiElement::setElementSettings(const std::map<std::string, std::string>& new_settings) {}

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
    this->Bind(wxEVT_CHECKBOX, &CheckboxGuiElement::checkBoxCallback, this);

    Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);

    Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);

    Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed, this);
}

void CheckboxGuiElement::checkBoxCallback(wxCommandEvent& event)
{
    sendGuiData();
}

void CheckboxGuiElement::setElementSettings(const std::map<std::string, std::string>& new_settings) {}
