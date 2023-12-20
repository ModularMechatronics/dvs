#include "gui_elements.h"

ButtonGuiElement::ButtonGuiElement(
    wxFrame* parent,
    const std::shared_ptr<ElementSettings>& element_settings,
    const std::function<void(const char key)>& notify_main_window_key_pressed,
    const std::function<void(const char key)>& notify_main_window_key_released,
    const std::function<void(const wxPoint pos, const std::string& elem_name)>&
        notify_parent_window_right_mouse_pressed,
    const std::function<void()>& notify_main_window_about_modification,
    const std::function<void(const wxPoint& pos, const wxSize& size, const bool is_editing)>& notify_tab_about_editing,
    const std::function<void(const Color_t, const std::string&)>& push_text_to_cmdl_output_window,
    const wxPoint& pos,
    const wxSize& size)
    : wxButton(parent,
               wxID_ANY,
               std::dynamic_pointer_cast<ButtonSettings>(element_settings)->label,
               pos,
               size,
               wxBU_EXACTFIT),
      ApplicationGuiElement{element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification,
                            notify_tab_about_editing,
                            push_text_to_cmdl_output_window}
{
    is_pressed_ = false;

    this->Bind(wxEVT_BUTTON, &ButtonGuiElement::buttonEvent, this);

    Bind(wxEVT_KEY_DOWN, &ApplicationGuiElement::keyPressedCallback, this);
    Bind(wxEVT_KEY_UP, &ApplicationGuiElement::keyReleasedCallback, this);

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

void ButtonGuiElement::updateElementSettings(const std::map<std::string, std::string>& new_settings)
{
    ButtonSettings* button_settings = dynamic_cast<ButtonSettings*>(element_settings_.get());

    if (new_settings.count("label") > 0U)
    {
        this->SetLabel(new_settings.at("label"));
        button_settings->label = new_settings.at("label");
    }
    element_settings_->handle_string = new_settings.at("handle_string");
}

SliderGuiElement::SliderGuiElement(
    wxFrame* parent,
    const std::shared_ptr<ElementSettings>& element_settings,
    const std::function<void(const char key)>& notify_main_window_key_pressed,
    const std::function<void(const char key)>& notify_main_window_key_released,
    const std::function<void(const wxPoint pos, const std::string& elem_name)>&
        notify_parent_window_right_mouse_pressed,
    const std::function<void()>& notify_main_window_about_modification,
    const std::function<void(const wxPoint& pos, const wxSize& size, const bool is_editing)>& notify_tab_about_editing,
    const std::function<void(const Color_t, const std::string&)>& push_text_to_cmdl_output_window,
    const wxPoint& pos,
    const wxSize& size)
    : wxSlider(parent,
               wxID_ANY,
               std::dynamic_pointer_cast<SliderSettings>(element_settings)->init_value,
               std::dynamic_pointer_cast<SliderSettings>(element_settings)->min_value,
               std::dynamic_pointer_cast<SliderSettings>(element_settings)->max_value,
               pos,
               size,
               getStyle(element_settings)),
      ApplicationGuiElement{element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification,
                            notify_tab_about_editing,
                            push_text_to_cmdl_output_window},
      slider_value_{std::dynamic_pointer_cast<SliderSettings>(element_settings)->init_value},
      is_horizontal_{std::dynamic_pointer_cast<SliderSettings>(element_settings)->is_horizontal}
{
    Bind(wxEVT_SLIDER, &SliderGuiElement::sliderEvent, this);

    this->Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    this->Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    this->Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);

    Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);

    Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed, this);

    value_text_ = new wxStaticText(parent, wxID_ANY, std::to_string(slider_value_));
    min_text_ = new wxStaticText(
        parent, wxID_ANY, std::to_string(std::dynamic_pointer_cast<SliderSettings>(element_settings)->min_value));
    max_text_ = new wxStaticText(
        parent, wxID_ANY, std::to_string(std::dynamic_pointer_cast<SliderSettings>(element_settings)->max_value));

    value_text_->SetLabel(std::to_string(this->GetValue()));

    /*if (is_horizontal_)
    {
        if (element_settings_->height > element_settings_->width)
        {
            std::swap(element_settings_->width, element_settings_->height);
            this->SetWindowStyle(wxSL_HORIZONTAL);
        }
    }
    else
    {
        if (element_settings_->width > element_settings_->height)
        {
            std::cout << "Swapping width and height" << std::endl;
            std::swap(element_settings_->width, element_settings_->height);
            element_settings_->height *= 4.0f;
            this->setSize(wxSize(element_settings_->width, element_settings_->height));
            this->SetWindowStyle(wxSL_VERTICAL | wxSL_INVERSE | wxSL_RIGHT);
        }
    }*/
}

int SliderGuiElement::getStyle(const std::shared_ptr<ElementSettings>& element_settings) const
{
    if (std::dynamic_pointer_cast<SliderSettings>(element_settings)->is_horizontal)
    {
        return wxSL_HORIZONTAL;
    }
    else
    {
        return wxSL_VERTICAL | wxSL_INVERSE | wxSL_RIGHT;
    }
}

void SliderGuiElement::sliderEvent(wxCommandEvent& event)
{
    const std::int32_t new_value{this->GetValue()};
    if (new_value == slider_value_)
    {
        return;
    }

    value_text_->SetLabel(std::to_string(new_value));

    slider_value_ = new_value;

    sendGuiData();
}

void SliderGuiElement::setElementPositionAndSize()
{
    ApplicationGuiElement::setElementPositionAndSize();
    const wxSize size{this->GetSize()};
    const wxPoint pos{this->GetPosition()};

    if (is_horizontal_)
    {
        value_text_->SetPosition(wxPoint(pos.x + size.x / 2, pos.y + size.y / 2 + 7));

        min_text_->SetPosition(wxPoint(pos.x - 13, pos.y + size.y / 2 - 10));
        max_text_->SetPosition(wxPoint(pos.x + size.x + 5, pos.y + size.y / 2 - 10));
    }
    else
    {
        value_text_->SetPosition(wxPoint(pos.x + size.x / 2 + 7, pos.y + size.y / 2));

        min_text_->SetPosition(wxPoint(pos.x + size.x / 2, pos.y - 13));
        max_text_->SetPosition(wxPoint(pos.x + size.x / 2, pos.y + size.y + 5));
    }
}

void SliderGuiElement::updateElementSettings(const std::map<std::string, std::string>& new_settings)
{
    SliderSettings* slider_settings = dynamic_cast<SliderSettings*>(element_settings_.get());

    if (new_settings.count("min_value") > 0U)
    {
        slider_settings->min_value = std::stoi(new_settings.at("min_value"));
        this->SetMin(slider_settings->min_value);
    }

    if (new_settings.count("max_value") > 0U)
    {
        slider_settings->max_value = std::stoi(new_settings.at("max_value"));
        this->SetMax(slider_settings->max_value);
    }

    min_text_->SetLabel(std::to_string(slider_settings->min_value));
    max_text_->SetLabel(std::to_string(slider_settings->max_value));

    element_settings_->handle_string = new_settings.at("handle_string");
}

CheckboxGuiElement::CheckboxGuiElement(
    wxFrame* parent,
    const std::shared_ptr<ElementSettings>& element_settings,
    const std::function<void(const char key)>& notify_main_window_key_pressed,
    const std::function<void(const char key)>& notify_main_window_key_released,
    const std::function<void(const wxPoint pos, const std::string& elem_name)>&
        notify_parent_window_right_mouse_pressed,
    const std::function<void()>& notify_main_window_about_modification,
    const std::function<void(const wxPoint& pos, const wxSize& size, const bool is_editing)>& notify_tab_about_editing,
    const std::function<void(const Color_t, const std::string&)>& push_text_to_cmdl_output_window,
    const wxPoint& pos,
    const wxSize& size)
    : wxCheckBox(parent, wxID_ANY, std::dynamic_pointer_cast<CheckboxSettings>(element_settings)->label, pos, size),
      ApplicationGuiElement{element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification,
                            notify_tab_about_editing,
                            push_text_to_cmdl_output_window}
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

void CheckboxGuiElement::updateElementSettings(const std::map<std::string, std::string>& new_settings) {}

TextLabelGuiElement::TextLabelGuiElement(
    wxFrame* parent,
    const std::shared_ptr<ElementSettings>& element_settings,
    const std::function<void(const char key)>& notify_main_window_key_pressed,
    const std::function<void(const char key)>& notify_main_window_key_released,
    const std::function<void(const wxPoint pos, const std::string& elem_name)>&
        notify_parent_window_right_mouse_pressed,
    const std::function<void()>& notify_main_window_about_modification,
    const std::function<void(const wxPoint& pos, const wxSize& size, const bool is_editing)>& notify_tab_about_editing,
    const std::function<void(const Color_t, const std::string&)>& push_text_to_cmdl_output_window,
    const wxPoint& pos,
    const wxSize& size)
    : wxStaticText(parent, wxID_ANY, std::dynamic_pointer_cast<TextLabelSettings>(element_settings)->label, pos, size),
      ApplicationGuiElement{element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification,
                            notify_tab_about_editing,
                            push_text_to_cmdl_output_window}
{
    Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);

    Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);

    Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed, this);
}

void TextLabelGuiElement::setLabel(const std::string& new_label)
{
    CallAfter(&TextLabelGuiElement::SetLabel, new_label);
}

void TextLabelGuiElement::updateElementSettings(const std::map<std::string, std::string>& new_settings)
{
    TextLabelSettings* button_settings = dynamic_cast<TextLabelSettings*>(element_settings_.get());

    if (new_settings.count("label") > 0U)
    {
        this->SetLabel(new_settings.at("label"));
        button_settings->label = new_settings.at("label");
    }
    element_settings_->handle_string = new_settings.at("handle_string");
}
