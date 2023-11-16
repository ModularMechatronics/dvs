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
    this->Bind(wxEVT_LEFT_DOWN, &ButtonGuiElement::mouseLeftPressed, this);
    this->Bind(wxEVT_LEFT_DOWN, &ButtonGuiElement::mouseLeftReleased, this);
}

void ButtonGuiElement::mouseLeftReleased(wxMouseEvent& event)
{
    is_pressed_ = false;
}

void ButtonGuiElement::mouseLeftPressed(wxMouseEvent& event)
{
    is_pressed_ = true;

    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    const std::uint8_t handle_string_length = element_settings_->handle_string.length();

    const std::uint64_t num_bytes_to_send = handle_string_length + sizeof(std::uint8_t);

    FillableUInt8Array output_array{num_bytes_to_send};

    output_array.fillWithStaticType(handle_string_length);
    output_array.fillWithDataFromPointer(element_settings_->handle_string.data(),
                                         element_settings_->handle_string.length());

    sendThroughTcpInterface(output_array.view(), kGuiTcpPortNum);
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
                            notify_main_window_about_modification)
{
    parent_size_ = parent->GetSize();
    Bind(wxEVT_SLIDER, &SliderGuiElement::sliderEvent, this);
    // this->Bind(wxEVT_LEFT_DOWN, &SliderGuiElement::mouseLeftPressed, this);
}

void SliderGuiElement::sliderEvent(wxCommandEvent& event)
{
    std::cout << "Slider event!" << std::endl;
    std::cout << "Value: " << this->GetValue() << std::endl;

    /*if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    const std::uint8_t handle_string_length = element_settings_->handle_string.length();

    const std::uint64_t num_bytes_to_send = handle_string_length + sizeof(std::uint8_t);

    FillableUInt8Array output_array{num_bytes_to_send};

    output_array.fillWithStaticType(handle_string_length);
    output_array.fillWithDataFromPointer(element_settings_->handle_string.data(),
                                         element_settings_->handle_string.length());

    sendThroughTcpInterface(output_array.view(), kGuiTcpPortNum);*/
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
    parent_size_ = parent->GetSize();
    this->Bind(wxEVT_CHECKBOX, &CheckboxGuiElement::checkBoxCallback, this);
}

void CheckboxGuiElement::checkBoxCallback(wxCommandEvent& event)
{
    const std::uint8_t is_checked{this->GetValue()};

    std::cout << "Checkbox value: " << static_cast<int>(is_checked) << std::endl;

    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    const std::uint8_t handle_string_length = element_settings_->handle_string.length();

    const std::uint64_t num_bytes_to_send = handle_string_length + sizeof(std::uint8_t);

    FillableUInt8Array output_array{num_bytes_to_send};

    output_array.fillWithStaticType(handle_string_length);
    output_array.fillWithDataFromPointer(element_settings_->handle_string.data(),
                                         element_settings_->handle_string.length());

    // output_array.fillWithStaticType(static_cast<std::uint8_t>(element_settings_->type));

    output_array.fillWithStaticType(is_checked);

    sendThroughTcpInterface(output_array.view(), kGuiTcpPortNum);
}