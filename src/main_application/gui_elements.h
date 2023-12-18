#ifndef MAIN_APPLICATION_GUI_ELEMENTS_H_
#define MAIN_APPLICATION_GUI_ELEMENTS_H_

#include <wx/notebook.h>
#include <wx/wx.h>

#include <functional>
#include <memory>
#include <queue>

#include "communication/received_data.h"
#include "dvs/enumerations.h"
#include "dvs/math/math.h"
#include "gui_element.h"
#include "project_state/project_settings.h"

class ButtonGuiElement : public wxButton, public ApplicationGuiElement
{
private:
    bool is_pressed_;

public:
    ButtonGuiElement(wxFrame* parent,
                     const std::shared_ptr<ElementSettings>& element_settings,
                     const std::function<void(const char key)>& notify_main_window_key_pressed,
                     const std::function<void(const char key)>& notify_main_window_key_released,
                     const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                         notify_parent_window_right_mouse_pressed,
                     const std::function<void()>& notify_main_window_about_modification,
                     const std::function<void(const wxPoint& pos, const wxSize& size, const bool is_editing)>&
                         notify_tab_about_editing,
                     const std::function<void(const Color_t, const std::string&)>& push_text_to_cmdl_output_window,
                     const wxPoint& pos,
                     const wxSize& size);

    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseMovedOverItem(wxMouseEvent& event);
    void buttonEvent(wxCommandEvent& event);

    void updateElementSettings(const std::map<std::string, std::string>& new_settings) override;

    std::uint64_t getGuiPayloadSize() const override
    {
        return sizeof(std::int8_t);
    }

    void fillGuiPayload(FillableUInt8Array& output_array) const override
    {
        output_array.fillWithStaticType(static_cast<std::uint8_t>(is_pressed_));
    }

    void setMinXPos(const int min_x_pos) override
    {
        minimum_x_pos_ = min_x_pos;
        setElementPositionAndSize();
    }

    void keyPressedElementSpecific(const char key) override {}

    void keyReleasedElementSpecific(const char key) override {}

    wxPoint getPosition() const override
    {
        return this->GetPosition();
    }

    wxSize getSize() const override
    {
        return this->GetSize();
    }

    void setPosition(const wxPoint& new_pos) override
    {
        this->SetPosition(new_pos);
    }

    void setSize(const wxSize& new_size) override
    {
        this->SetSize(new_size);
    }

    void hide() override
    {
        Hide();
    }

    void show() override
    {
        Show();
    }

    wxWindow* getParent() const override
    {
        return this->GetParent();
    }

    void updateSizeFromParent(const wxSize& parent_size) override
    {
        setElementPositionAndSize();
    }

    void mouseLeftPressedGuiElementSpecific(wxMouseEvent& event) override
    {
        is_pressed_ = true;
    }

    void mouseLeftReleasedGuiElementSpecific(wxMouseEvent& event) override
    {
        is_pressed_ = false;
    }

    std::shared_ptr<GuiElementState> getGuiElementState() const override
    {
        std::shared_ptr<ButtonState> button_state =
            std::make_shared<ButtonState>(element_settings_->handle_string, is_pressed_);

        return button_state;
    }

    void setLabel(const std::string& new_label) override;
};

class SliderGuiElement : public wxSlider, public ApplicationGuiElement
{
private:
    std::int32_t slider_value_;
    wxStaticText* value_text_;
    wxStaticText* min_text_;
    wxStaticText* max_text_;

public:
    SliderGuiElement(wxFrame* parent,
                     const std::shared_ptr<ElementSettings>& element_settings,
                     const std::function<void(const char key)>& notify_main_window_key_pressed,
                     const std::function<void(const char key)>& notify_main_window_key_released,
                     const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                         notify_parent_window_right_mouse_pressed,
                     const std::function<void()>& notify_main_window_about_modification,
                     const std::function<void(const wxPoint& pos, const wxSize& size, const bool is_editing)>&
                         notify_tab_about_editing,
                     const std::function<void(const Color_t, const std::string&)>& push_text_to_cmdl_output_window,
                     const wxPoint& pos,
                     const wxSize& size);

    void sliderEvent(wxCommandEvent& event);

    void updateElementSettings(const std::map<std::string, std::string>& new_settings) override;

    std::shared_ptr<GuiElementState> getGuiElementState() const override
    {
        std::shared_ptr<SliderState> slider_state =
            std::make_shared<SliderState>(element_settings_->handle_string,
                                          std::dynamic_pointer_cast<SliderSettings>(element_settings_)->min_value,
                                          std::dynamic_pointer_cast<SliderSettings>(element_settings_)->max_value,
                                          std::dynamic_pointer_cast<SliderSettings>(element_settings_)->step_size,
                                          this->GetValue(),
                                          true);

        return slider_state;
    }

    void setMinXPos(const int min_x_pos) override
    {
        minimum_x_pos_ = min_x_pos;
        setElementPositionAndSize();
    }

    void setElementPositionAndSize() override;

    std::uint64_t getGuiPayloadSize() const override
    {
        return 4U * sizeof(std::int32_t);
    }

    void fillGuiPayload(FillableUInt8Array& output_array) const override
    {
        const std::int32_t step_size{std::dynamic_pointer_cast<SliderSettings>(element_settings_)->step_size};
        const std::int32_t min_value{std::dynamic_pointer_cast<SliderSettings>(element_settings_)->min_value};
        const std::int32_t max_value{std::dynamic_pointer_cast<SliderSettings>(element_settings_)->max_value};

        output_array.fillWithStaticType(min_value);
        output_array.fillWithStaticType(max_value);
        output_array.fillWithStaticType(step_size);
        output_array.fillWithStaticType(slider_value_);
    }

    void mouseMovedOverItem(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);

    wxPoint getPosition() const override
    {
        return this->GetPosition();
    }

    wxSize getSize() const override
    {
        return this->GetSize();
    }

    wxWindow* getParent() const override
    {
        return this->GetParent();
    }

    void setPosition(const wxPoint& new_pos) override
    {
        this->SetPosition(new_pos);
    }

    void setSize(const wxSize& new_size) override
    {
        this->SetSize(new_size);
    }

    void hide() override
    {
        Hide();
        value_text_->Hide();
        min_text_->Hide();
        max_text_->Hide();
    }

    void show() override
    {
        Show();
        value_text_->Show();
        min_text_->Show();
        max_text_->Show();
    }

    void keyPressedElementSpecific(const char key) override {}

    void keyReleasedElementSpecific(const char key) override {}

    void updateSizeFromParent(const wxSize& parent_size) override
    {
        setElementPositionAndSize();
    }
};

class CheckboxGuiElement : public wxCheckBox, public ApplicationGuiElement
{
private:
public:
    CheckboxGuiElement(wxFrame* parent,
                       const std::shared_ptr<ElementSettings>& element_settings,
                       const std::function<void(const char key)>& notify_main_window_key_pressed,
                       const std::function<void(const char key)>& notify_main_window_key_released,
                       const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                           notify_parent_window_right_mouse_pressed,
                       const std::function<void()>& notify_main_window_about_modification,
                       const std::function<void(const wxPoint& pos, const wxSize& size, const bool is_editing)>&
                           notify_tab_about_editing,
                       const std::function<void(const Color_t, const std::string&)>& push_text_to_cmdl_output_window,
                       const wxPoint& pos,
                       const wxSize& size);

    void setMinXPos(const int min_x_pos) override
    {
        minimum_x_pos_ = min_x_pos;
        setElementPositionAndSize();
    }

    void updateElementSettings(const std::map<std::string, std::string>& new_settings) override;

    void keyPressedElementSpecific(const char key) override {}

    void keyReleasedElementSpecific(const char key) override {}

    std::uint64_t getGuiPayloadSize() const override
    {
        return sizeof(std::int8_t);
    }

    void fillGuiPayload(FillableUInt8Array& output_array) const override
    {
        const bool is_checked = this->IsChecked();
        output_array.fillWithStaticType(static_cast<std::uint8_t>(is_checked));
    }

    void updateSizeFromParent(const wxSize& parent_size) override
    {
        setElementPositionAndSize();
    }

    wxPoint getPosition() const override
    {
        return this->GetPosition();
    }

    wxSize getSize() const override
    {
        return this->GetSize();
    }

    wxWindow* getParent() const override
    {
        return this->GetParent();
    }

    void setPosition(const wxPoint& new_pos) override
    {
        this->SetPosition(new_pos);
    }

    void setSize(const wxSize& new_size) override
    {
        this->SetSize(new_size);
    }

    void hide() override
    {
        Hide();
    }

    void show() override
    {
        Show();
    }

    void checkBoxCallback(wxCommandEvent& event);

    std::shared_ptr<GuiElementState> getGuiElementState() const override
    {
        std::shared_ptr<CheckboxState> checkbox_state =
            std::make_shared<CheckboxState>(element_settings_->handle_string, this->GetValue());

        return checkbox_state;
    }
};

class TextLabelGuiElement : public wxStaticText, public ApplicationGuiElement
{
private:
    std::string label_;

public:
    TextLabelGuiElement(wxFrame* parent,
                        const std::shared_ptr<ElementSettings>& element_settings,
                        const std::function<void(const char key)>& notify_main_window_key_pressed,
                        const std::function<void(const char key)>& notify_main_window_key_released,
                        const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                            notify_parent_window_right_mouse_pressed,
                        const std::function<void()>& notify_main_window_about_modification,
                        const std::function<void(const wxPoint& pos, const wxSize& size, const bool is_editing)>&
                            notify_tab_about_editing,
                        const std::function<void(const Color_t, const std::string&)>& push_text_to_cmdl_output_window,
                        const wxPoint& pos,
                        const wxSize& size);

    void setMinXPos(const int min_x_pos) override
    {
        minimum_x_pos_ = min_x_pos;
        setElementPositionAndSize();
    }

    void updateElementSettings(const std::map<std::string, std::string>& new_settings) override;

    void keyPressedElementSpecific(const char key) override {}

    void keyReleasedElementSpecific(const char key) override {}

    std::uint64_t getGuiPayloadSize() const override
    {
        return sizeof(std::int8_t) + label_.length();
    }

    void fillGuiPayload(FillableUInt8Array& output_array) const override
    {
        output_array.fillWithStaticType(static_cast<std::uint8_t>(label_.length()));
        output_array.fillWithDataFromPointer(label_.data(), label_.length());
    }

    void updateSizeFromParent(const wxSize& parent_size) override
    {
        setElementPositionAndSize();
    }

    wxPoint getPosition() const override
    {
        return this->GetPosition();
    }

    wxSize getSize() const override
    {
        return this->GetSize();
    }

    wxWindow* getParent() const override
    {
        return this->GetParent();
    }

    void setPosition(const wxPoint& new_pos) override
    {
        this->SetPosition(new_pos);
    }

    void setSize(const wxSize& new_size) override
    {
        this->SetSize(new_size);
    }

    void hide() override
    {
        Hide();
    }

    void show() override
    {
        Show();
    }

    void checkBoxCallback(wxCommandEvent& event);

    std::shared_ptr<GuiElementState> getGuiElementState() const override
    {
        std::shared_ptr<TextLabelState> text_label_state =
            std::make_shared<TextLabelState>(element_settings_->handle_string, this->GetLabel().ToStdString());

        return text_label_state;
    }

    void setLabel(const std::string& new_label);
};

#endif  // MAIN_APPLICATION_GUI_ELEMENT_H_
