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
               getSizeInternal(element_settings, parent),
               wxSL_HORIZONTAL),
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

    if (is_horizontal_)
    {
        min_y_size_ = 30;
        max_y_size_ = 30;
    }
    else
    {
        min_x_size_ = 30;
        max_x_size_ = 30;
    }
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

ListBoxGuiElement::ListBoxGuiElement(
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
    : wxListBox{parent, wxID_ANY, pos, size},
      ApplicationGuiElement{element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification,
                            notify_tab_about_editing,
                            push_text_to_cmdl_output_window}
{
    elements_ = std::dynamic_pointer_cast<ListBoxSettings>(element_settings)->elements;

    for (const std::string& element : elements_)
    {
        this->Append(element);
    }

    this->SetSelection(0);

    if (this->GetSelection() >= 0 && this->GetSelection() < elements_.size())
    {
        selected_element_ = elements_.at(this->GetSelection());
    }
    else
    {
        selected_element_ = "";
    }

    Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);

    Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);

    Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed, this);

    Bind(wxEVT_LISTBOX, &ListBoxGuiElement::listBoxCallback, this);
}

void ListBoxGuiElement::listBoxCallback(wxCommandEvent& event)
{
    const int selection = this->GetSelection();
    if (selection >= 0 && selection < elements_.size())
    {
        selected_element_ = elements_.at(selection);
    }
    else
    {
        selected_element_ = "";
    }

    sendGuiData();
}

void ListBoxGuiElement::updateElementSettings(const std::map<std::string, std::string>& new_settings)
{
    /*ListBoxGuiElement* button_settings = dynamic_cast<TextLabelSettings*>(element_settings_.get());

    if (new_settings.count("label") > 0U)
    {
        this->SetLabel(new_settings.at("label"));
        button_settings->label = new_settings.at("label");
    }
    element_settings_->handle_string = new_settings.at("handle_string");*/
}

EditableTextGuiElement::EditableTextGuiElement(
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
    : wxTextCtrl{parent, wxID_ANY, "empty", pos, size, wxTE_PROCESS_ENTER | wxTE_LEFT},
      ApplicationGuiElement{element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification,
                            notify_tab_about_editing,
                            push_text_to_cmdl_output_window}
{
    text_ = std::dynamic_pointer_cast<EditableTextSettings>(element_settings)->init_value;

    this->SetLabel(text_);

    Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);

    Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);

    Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed, this);

    Bind(wxEVT_TEXT, &EditableTextGuiElement::editableTextCallback, this);
    Bind(wxEVT_TEXT_ENTER, &EditableTextGuiElement::editableTextEnterPressedCallback, this);
}

void EditableTextGuiElement::editableTextEnterPressedCallback(wxCommandEvent& event)
{
    enter_pressed_ = true;
    sendGuiData();
    enter_pressed_ = false;
}

void EditableTextGuiElement::editableTextCallback(wxCommandEvent& event)
{
    enter_pressed_ = false;
    text_ = this->GetValue();

    sendGuiData();
}

void EditableTextGuiElement::updateElementSettings(const std::map<std::string, std::string>& new_settings)
{
    /*EditableTextGuiElement* button_settings = dynamic_cast<TextLabelSettings*>(element_settings_.get());

    if (new_settings.count("label") > 0U)
    {
        this->SetLabel(new_settings.at("label"));
        button_settings->label = new_settings.at("label");
    }
    element_settings_->handle_string = new_settings.at("handle_string");*/
}

DropdownMenuGuiElement::DropdownMenuGuiElement(
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
    : wxComboBox{parent, wxID_ANY, "", pos, size, 0, NULL, wxCB_READONLY},
      ApplicationGuiElement{element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification,
                            notify_tab_about_editing,
                            push_text_to_cmdl_output_window}
{
    this->SetWindowStyle(wxCB_READONLY);
    elements_ = std::dynamic_pointer_cast<DropdownMenuSettings>(element_settings)->elements;

    for (const std::string& element : elements_)
    {
        this->Append(element);
    }

    this->SetSelection(0);

    if (this->GetSelection() >= 0 && this->GetSelection() < elements_.size())
    {
        selected_element_ = elements_.at(this->GetSelection());
    }
    else
    {
        selected_element_ = "";
    }

    Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);

    Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);

    Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed, this);

    Bind(wxEVT_COMBOBOX, &DropdownMenuGuiElement::dropdownCallback, this);
}

void DropdownMenuGuiElement::dropdownCallback(wxCommandEvent& event)
{
    const int selection = this->GetSelection();
    if (selection >= 0 && selection < elements_.size())
    {
        selected_element_ = elements_.at(selection);
    }
    else
    {
        selected_element_ = "";
    }

    sendGuiData();
}

void DropdownMenuGuiElement::updateElementSettings(const std::map<std::string, std::string>& new_settings)
{
    /*DropdownMenuGuiElement* button_settings = dynamic_cast<TextLabelSettings*>(element_settings_.get());

    if (new_settings.count("label") > 0U)
    {
        this->SetLabel(new_settings.at("label"));
        button_settings->label = new_settings.at("label");
    }
    element_settings_->handle_string = new_settings.at("handle_string");*/
}

RadioButtonGroupGuiElement::RadioButtonGroupGuiElement(
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
    : wxRadioBox{parent,
                 wxID_ANY,
                 std::dynamic_pointer_cast<RadioButtonGroupSettings>(element_settings)->label,
                 pos,
                 size,
                 getRadioButtons(element_settings),
                 1,
                 wxRA_SPECIFY_COLS},
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

    Bind(wxEVT_RADIOBOX, &RadioButtonGroupGuiElement::radioButtonGroupCallback, this);

    buttons_.clear();

    for (size_t k = 0; k < this->GetCount(); ++k)
    {
        buttons_.push_back(this->GetString(k).ToStdString());
    }

    selected_idx_ = this->GetSelection();
}

void RadioButtonGroupGuiElement::radioButtonGroupCallback(wxCommandEvent& event)
{
    buttons_.clear();

    for (size_t k = 0; k < this->GetCount(); ++k)
    {
        buttons_.push_back(this->GetString(k).ToStdString());
    }

    selected_idx_ = this->GetSelection();

    sendGuiData();
}

void RadioButtonGroupGuiElement::updateElementSettings(const std::map<std::string, std::string>& new_settings)
{
    /*RadioButtonGroupGuiElement* button_settings = dynamic_cast<TextLabelSettings*>(element_settings_.get());

    if (new_settings.count("label") > 0U)
    {
        this->SetLabel(new_settings.at("label"));
        button_settings->label = new_settings.at("label");
    }
    element_settings_->handle_string = new_settings.at("handle_string");*/
}

ScrollingTextGuiElement::ScrollingTextGuiElement(
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
    : wxTextCtrl{parent,
                 wxID_ANY,
                 std::dynamic_pointer_cast<ScrollingTextSettings>(element_settings)->title,
                 pos,
                 size,
                 wxTE_MULTILINE | wxTE_READONLY},
      ApplicationGuiElement{element_settings,
                            notify_main_window_key_pressed,
                            notify_main_window_key_released,
                            notify_parent_window_right_mouse_pressed,
                            notify_main_window_about_modification,
                            notify_tab_about_editing,
                            push_text_to_cmdl_output_window}
{
    const wxFont fnt{13, wxFontFamily::wxFONTFAMILY_TELETYPE, wxNORMAL, wxNORMAL};
    SetFont(fnt);

    Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);

    Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);
    Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);

    Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed, this);

    black_color_ = wxColor(0, 0, 0);

    std::shared_ptr<ScrollingTextSettings> scrolling_text_settings =
        std::dynamic_pointer_cast<ScrollingTextSettings>(element_settings);
    for (const auto& subscribed_stream : scrolling_text_settings->subscribed_streams)
    {
        if (subscribed_stream.text_color.has_value())
        {
            const RGBTripletf& color = subscribed_stream.text_color.value();
            topic_id_to_color_[subscribed_stream.topic_id] = wxColor(static_cast<uint8_t>(color.red * 255.0f),
                                                                     static_cast<uint8_t>(color.green * 255.0f),
                                                                     static_cast<uint8_t>(color.blue * 255.0f));
        }
        else
        {
            topic_id_to_color_[subscribed_stream.topic_id] = black_color_;
        }
    }

    print_timestamp_ = scrolling_text_settings->print_timestamp;
    print_topic_id_ = scrolling_text_settings->print_topic_id;
}

void ScrollingTextGuiElement::setSize(const wxSize& new_size)
{
    this->SetSize(new_size);
}

void ScrollingTextGuiElement::pushNewText(const TopicId topic_id,
                                          const std::vector<std::pair<uint64_t, std::string>>& timestamps_and_strings)
{
    std::ostream stream(this);

    for (const std::pair<uint64_t, std::string>& ts_and_str : timestamps_and_strings)
    {
        SetDefaultStyle(black_color_);

        if (print_topic_id_)
        {
            stream << "[" << topic_id << "]";
        }

        if (print_timestamp_)
        {
            stream << "[" << ts_and_str.first << "]";
        }

        if (print_timestamp_ || print_topic_id_)
        {
            stream << ": ";
        }

        SetDefaultStyle(topic_id_to_color_[topic_id]);
        stream << ts_and_str.second << std::endl;
    }

    stream.flush();
}

void ScrollingTextGuiElement::updateElementSettings(const std::map<std::string, std::string>& new_settings)
{
    /*ScrollingTextGuiElement* button_settings = dynamic_cast<TextLabelSettings*>(element_settings_.get());

    if (new_settings.count("label") > 0U)
    {
        this->SetLabel(new_settings.at("label"));
        button_settings->label = new_settings.at("label");
    }
    element_settings_->handle_string = new_settings.at("handle_string");*/
}
