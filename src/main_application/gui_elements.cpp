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
    this->Bind(wxEVT_LEFT_UP, &ButtonGuiElement::mouseLeftReleased, this);
}

void ButtonGuiElement::mouseLeftReleased(wxMouseEvent& event)
{
    is_pressed_ = false;

    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    const std::uint8_t handle_string_length = element_settings_->handle_string.length();

    const std::uint64_t num_bytes_to_send = handle_string_length + // the handle_string itself
        sizeof(std::uint8_t) + // length of handle_string
        sizeof(std::uint8_t) + // type
        sizeof(std::uint8_t) + // is_pressed
        sizeof(std::uint32_t); // payload size

    FillableUInt8Array output_array{num_bytes_to_send};

    output_array.fillWithStaticType(static_cast<std::uint8_t>(element_settings_->type));
    output_array.fillWithStaticType(handle_string_length);
    output_array.fillWithDataFromPointer(element_settings_->handle_string.data(),
                                         element_settings_->handle_string.length());
    // Payload size
    output_array.fillWithStaticType(static_cast<std::uint32_t>(1U));
    output_array.fillWithStaticType(static_cast<std::uint8_t>(is_pressed_));

    sendThroughTcpInterface(output_array.view(), kGuiTcpPortNum);
}

void ButtonGuiElement::mouseLeftPressed(wxMouseEvent& event)
{
    is_pressed_ = true;

    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    const std::uint8_t handle_string_length = element_settings_->handle_string.length();

    const std::uint64_t num_bytes_to_send = handle_string_length + // the handle_string itself
        sizeof(std::uint8_t) + // length of handle_string
        sizeof(std::uint8_t) + // type
        sizeof(std::uint8_t) + // is_pressed
        sizeof(std::uint32_t); // payload size

    FillableUInt8Array output_array{num_bytes_to_send};

    output_array.fillWithStaticType(static_cast<std::uint8_t>(element_settings_->type));
    output_array.fillWithStaticType(handle_string_length);
    output_array.fillWithDataFromPointer(element_settings_->handle_string.data(),
                                         element_settings_->handle_string.length());
    // Payload size
    output_array.fillWithStaticType(static_cast<std::uint32_t>(1U));
    output_array.fillWithStaticType(static_cast<std::uint8_t>(is_pressed_));

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
}

void SliderGuiElement::sliderEvent(wxCommandEvent& event)
{
    // std::cout << "Slider event!" << std::endl;
    

    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    const std::uint8_t handle_string_length = element_settings_->handle_string.length();

    const std::uint64_t num_bytes_to_send = handle_string_length + // the handle_string itself
        sizeof(std::uint8_t) + // length of handle_string
        sizeof(std::uint8_t) + // type
        4U * sizeof(std::int32_t) + // gui data
        sizeof(std::uint32_t); // payload size

    FillableUInt8Array output_array{num_bytes_to_send};

    output_array.fillWithStaticType(static_cast<std::uint8_t>(element_settings_->type));
    output_array.fillWithStaticType(handle_string_length);
    output_array.fillWithDataFromPointer(element_settings_->handle_string.data(),
                                         element_settings_->handle_string.length());
    // Payload size
    output_array.fillWithStaticType(static_cast<std::uint32_t>(4U * sizeof(std::int32_t)));

    const std::int32_t value{this->GetValue()};
    const std::int32_t step_size{std::dynamic_pointer_cast<SliderSettings>(element_settings_)->step_size};
    const std::int32_t min_value{std::dynamic_pointer_cast<SliderSettings>(element_settings_)->min_value};
    const std::int32_t max_value{std::dynamic_pointer_cast<SliderSettings>(element_settings_)->max_value};

    output_array.fillWithStaticType(min_value);
    output_array.fillWithStaticType(max_value);
    output_array.fillWithStaticType(step_size);
    output_array.fillWithStaticType(value);

    sendThroughTcpInterface(output_array.view(), kGuiTcpPortNum);
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
    if (element_settings_->handle_string.length() >= 256U)
    {
        throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
    }

    const std::uint8_t handle_string_length = element_settings_->handle_string.length();

    const std::uint64_t num_bytes_to_send = handle_string_length + // the handle_string itself
        sizeof(std::uint8_t) + // length of handle_string
        sizeof(std::uint8_t) + // type
        sizeof(std::uint8_t) + // is_pressed
        sizeof(std::uint32_t); // payload size

    FillableUInt8Array output_array{num_bytes_to_send};

    output_array.fillWithStaticType(static_cast<std::uint8_t>(element_settings_->type));
    output_array.fillWithStaticType(handle_string_length);
    output_array.fillWithDataFromPointer(element_settings_->handle_string.data(),
                                         element_settings_->handle_string.length());
    // Payload size
    output_array.fillWithStaticType(static_cast<std::uint32_t>(1U));

    const std::uint8_t is_checked{this->GetValue()};
    output_array.fillWithStaticType(static_cast<std::uint8_t>(is_checked));

    sendThroughTcpInterface(output_array.view(), kGuiTcpPortNum);
}