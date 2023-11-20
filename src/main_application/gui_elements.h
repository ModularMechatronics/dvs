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

template <class C>
// class MovableElement : public C, public api_internal::InternalGuiElement, public ApplicationGuiElement
class MovableElement : public C, public ApplicationGuiElement
{
public:
    // Slider
    /*MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const int init_value,
                   const int min_value,
                   const int max_value,
                   const wxPoint& pos,
                   const wxSize& size,
                   const long style)
        : C(parent, id, init_value, min_value, max_value, pos, size, style),
          api_internal::InternalGuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down_ = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }*/

    // Button, Checkbox, ComboBox
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   // const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const std::string& label,
                   const wxPoint& pos,
                   const wxSize& size)
        : C(parent,
            id,
            label,
            pos,
            size)  // ,
                   // api_internal::InternalGuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down_ = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // ListBox
    /*MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const wxPoint& pos,
                   const wxSize& size)
        : C(parent, id, pos, size), api_internal::InternalGuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down_ = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // EditableText
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const std::string& initial_text,
                   const wxPoint& pos,
                   const wxSize& size,
                   const long style)
        : C(parent, id, initial_text, pos, size, style),
          api_internal::InternalGuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down_ = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // RadioButtonGroup
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const std::string& initial_text,
                   const wxPoint& pos,
                   const wxSize& size,
                   const wxArrayString& radio_buttons_to_add,
                   const int n,
                   const long style)
        : C(parent, id, initial_text, pos, size, radio_buttons_to_add, n, style),
          api_internal::InternalGuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down_ = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }*/

    void updateSizeFromParent(const wxSize& parent_size) override {}

    void keyPressed(const char key) override {}

    void keyReleased(const char key) override {}

    void setMouseInteractionType(const MouseInteractionType mit) override {}

    void show() override {}

    void hide() override {}

    void destroy() override {}

    // void refresh() override {}

    // void update() override {}

    void mouseLeftPressed(wxMouseEvent& event)
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

    void mouseLeftReleased(wxMouseEvent& event)
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

    void mouseMovedOverItem(wxMouseEvent& event)
    {
        if (control_pressed_at_mouse_down_ && event.LeftIsDown())
        {
            const wxPoint current_mouse_position_local = event.GetPosition();
            const wxPoint current_mouse_position_global = current_mouse_position_local + this->GetPosition();
            const wxPoint delta = current_mouse_position_global - previous_mouse_pos_;
            this->SetPosition(this->GetPosition() + delta);

            previous_mouse_pos_ = current_mouse_position_global;
        }
        else
        {
            event.Skip();
        }
    }

    /*long getId() const override
    {
        return this->GetId();
    }

    // General
    void setEnabled() override
    {
        wxControl* const contr = dynamic_cast<wxControl* const>(this);
        contr->Enable(true);
    }

    void setDisabled() override
    {
        wxControl* const contr = dynamic_cast<wxControl* const>(this);
        contr->Enable(false);
    }

    void setPosition(const std::int16_t x, const std::int16_t y) override
    {
        this->SetPosition(wxPoint(x, y));
    }

    void setSize(const std::int16_t width, const std::int16_t height) override
    {
        this->SetSize(wxSize(width, height));
    }

    // Slider
    std::int32_t getValue() const override
    {
        const wxSlider* const slider = dynamic_cast<const wxSlider* const>(this);
        return slider->GetValue();
    }

    std::int32_t getMin() const override
    {
        const wxSlider* const slider = dynamic_cast<const wxSlider* const>(this);
        return slider->GetMin();
    }

    std::int32_t getMax() const override
    {
        const wxSlider* const slider = dynamic_cast<const wxSlider* const>(this);
        return slider->GetMax();
    }

    void setMax(const std::int32_t new_max) override
    {
        wxSlider* const slider = dynamic_cast<wxSlider* const>(this);
        slider->SetMax(new_max);
    }

    void setMin(const std::int32_t new_min) override
    {
        wxSlider* const slider = dynamic_cast<wxSlider* const>(this);
        slider->SetMin(new_min);
    }

    void setValue(const std::int32_t new_value) override
    {
        wxSlider* const slider = dynamic_cast<wxSlider* const>(this);
        slider->SetValue(new_value);
    }

    // CheckBox
    bool isChecked() const override
    {
        const wxCheckBox* const check_box = dynamic_cast<const wxCheckBox* const>(this);
        return check_box->IsChecked();
    }

    void setChecked() override
    {
        wxCheckBox* const check_box = dynamic_cast<wxCheckBox* const>(this);
        check_box->SetValue(true);
    }

    void setUnChecked() override
    {
        wxCheckBox* const check_box = dynamic_cast<wxCheckBox* const>(this);
        check_box->SetValue(false);
    }

    // Button, Checkbox, TextLabel
    void setLabel(const std::string& new_label) override
    {
        wxControl* const contr = dynamic_cast<wxControl* const>(this);
        contr->SetLabel(new_label);
    }

    std::string getLabel(void) override
    {
        const wxControl* const contr = dynamic_cast<const wxControl* const>(this);
        return contr->GetLabel().ToStdString();
    }

    // RadioButtonGroup
    std::int16_t getSelectionIndex() const override
    {
        const wxRadioBox* const radio_box = dynamic_cast<const wxRadioBox* const>(this);
        return radio_box->GetSelection();
    }

    std::string getSelectionString() const override
    {
        const wxRadioBox* const radio_box = dynamic_cast<const wxRadioBox* const>(this);
        return radio_box->GetStringSelection().ToStdString();
    }

    // EditableText
    void setText(const std::string& new_text) override
    {
        wxTextCtrl* const text_ctrl = dynamic_cast<wxTextCtrl* const>(this);
        text_ctrl->SetValue(new_text);
    }

    std::string getText() const override
    {
        const wxTextCtrl* const text_ctrl = dynamic_cast<const wxTextCtrl* const>(this);
        return text_ctrl->GetValue().ToStdString();
    }

    bool enterPressed() const override
    {
        const wxTextCtrl* const text_ctrl = dynamic_cast<const wxTextCtrl* const>(this);
        return text_ctrl->HasFocus() && (wxGetKeyState(WXK_RETURN));
    }

    // DropDownMenu, ListBox
    void addItem(const std::string& item_text) override
    {
        wxItemContainer* const elem = dynamic_cast<wxItemContainer* const>(this);
        elem->Append(item_text);
    }

    void removeItem(const std::string& item_text) override
    {
        wxItemContainer* const elem = dynamic_cast<wxItemContainer* const>(this);
        const int idx = elem->FindString(item_text, true);
        if (idx != wxNOT_FOUND)
        {
            elem->Delete(idx);
        }
    }

    void clearItems() override
    {
        wxItemContainer* const elem = dynamic_cast<wxItemContainer* const>(this);
        elem->Clear();
    }

    void selectItem(const std::string& item_text) override
    {
        wxItemContainer* const elem = dynamic_cast<wxItemContainer* const>(this);
        elem->SetStringSelection(item_text);
    }

    void selectItem(const std::int32_t item_idx) override
    {
        wxItemContainer* const elem = dynamic_cast<wxItemContainer* const>(this);
        elem->Select(item_idx);
    }

    std::string getSelectedItem() const override
    {
        const wxItemContainer* const elem = dynamic_cast<const wxItemContainer* const>(this);
        return elem->GetStringSelection().ToStdString();
    }

    std::int32_t getNumItems() const override
    {
        const wxItemContainer* const elem = dynamic_cast<const wxItemContainer* const>(this);
        return elem->GetCount();
    }

    std::int32_t getSelectedItemIndex() const override
    {
        const wxItemContainer* const elem = dynamic_cast<const wxItemContainer* const>(this);
        return elem->GetSelection();
    }*/

private:
    bool control_pressed_at_mouse_down_;
    wxPoint mouse_pos_at_press_;
    wxPoint previous_mouse_pos_;
};

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
                     const wxPoint& pos,
                     const wxSize& size);

    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseMovedOverItem(wxMouseEvent& event);
    void buttonEvent(wxCommandEvent& event);

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

    void setElementPositionAndSize()
    {
        const float px = parent_size_.GetWidth();
        const float py = parent_size_.GetHeight();

        wxPoint new_pos;
        wxSize new_size;

        const float ratio_x = 1.0f - static_cast<float>(minimum_x_pos_) / px;
        const float ratio_y = 1.0f - static_cast<float>(minimum_y_pos_) / py;

        new_size.SetWidth(element_settings_->width * px * ratio_x);
        new_size.SetHeight(element_settings_->height * py * ratio_y);
        new_pos.x = minimum_x_pos_ + element_settings_->x * px * ratio_x;
        new_pos.y = minimum_y_pos_ + element_settings_->y * py * ratio_y;

        SetPosition(new_pos);
        this->SetSize(new_size);
    }

    void keyPressed(const char key) override {}

    void keyReleased(const char key) override {}

    wxPoint getPosition() const override
    {
        return this->GetPosition();
    }

    void setPosition(const wxPoint& new_pos) override
    {
        this->SetPosition(new_pos);
    }

    void updateSizeFromParent(const wxSize& parent_size) override
    {
        parent_size_ = parent_size;

        setElementPositionAndSize();
    }

    void mousePressedGuiElementCallback(wxMouseEvent& event) override
    {
        is_pressed_ = true;
    }

    void mouseReleasedGuiElementCallback(wxMouseEvent& event) override
    {
        is_pressed_ = false;
    }

    std::shared_ptr<GuiElementState> getGuiElementState() const override
    {
        std::shared_ptr<ButtonState> button_state = std::make_shared<ButtonState>(element_settings_->handle_string,
                                                                                   is_pressed_);

        return button_state;
    }
};

class SliderGuiElement : public wxSlider, public ApplicationGuiElement
{
private:
    std::int32_t slider_value_;
public:
    SliderGuiElement(wxFrame* parent,
                     const std::shared_ptr<ElementSettings>& element_settings,
                     const std::function<void(const char key)>& notify_main_window_key_pressed,
                     const std::function<void(const char key)>& notify_main_window_key_released,
                     const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                         notify_parent_window_right_mouse_pressed,
                     const std::function<void()>& notify_main_window_about_modification,
                     const wxPoint& pos,
                     const wxSize& size);

    void sliderEvent(wxCommandEvent& event);

    std::shared_ptr<GuiElementState> getGuiElementState() const override
    {
        std::shared_ptr<SliderState> slider_state = std::make_shared<SliderState>(
            element_settings_->handle_string,
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

    void setElementPositionAndSize()
    {
        const float px = parent_size_.GetWidth();
        const float py = parent_size_.GetHeight();

        wxPoint new_pos;
        wxSize new_size;

        const float ratio_x = 1.0f - static_cast<float>(minimum_x_pos_) / px;
        const float ratio_y = 1.0f - static_cast<float>(minimum_y_pos_) / py;

        new_size.SetWidth(element_settings_->width * px * ratio_x);
        new_size.SetHeight(element_settings_->height * py * ratio_y);
        new_pos.x = minimum_x_pos_ + element_settings_->x * px * ratio_x;
        new_pos.y = minimum_y_pos_ + element_settings_->y * py * ratio_y;

        SetPosition(new_pos);
        this->SetSize(new_size);
    }

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

    void setPosition(const wxPoint& new_pos) override
    {
        this->SetPosition(new_pos);
    }

    void keyPressed(const char key) override {}

    void keyReleased(const char key) override {}

    void updateSizeFromParent(const wxSize& parent_size) override
    {
        parent_size_ = parent_size;

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
                       const wxPoint& pos,
                       const wxSize& size);

    void setMinXPos(const int min_x_pos) override
    {
        minimum_x_pos_ = min_x_pos;
        setElementPositionAndSize();
    }

    void setElementPositionAndSize()
    {
        const float px = parent_size_.GetWidth();
        const float py = parent_size_.GetHeight();

        wxPoint new_pos;
        wxSize new_size;

        const float ratio_x = 1.0f - static_cast<float>(minimum_x_pos_) / px;
        const float ratio_y = 1.0f - static_cast<float>(minimum_y_pos_) / py;

        new_size.SetWidth(element_settings_->width * px * ratio_x);
        new_size.SetHeight(element_settings_->height * py * ratio_y);
        new_pos.x = minimum_x_pos_ + element_settings_->x * px * ratio_x;
        new_pos.y = minimum_y_pos_ + element_settings_->y * py * ratio_y;

        SetPosition(new_pos);
        this->SetSize(new_size);
    }

    void keyPressed(const char key) override {}

    void keyReleased(const char key) override {}

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
        parent_size_ = parent_size;

        setElementPositionAndSize();
    }

    wxPoint getPosition() const override
    {
        return this->GetPosition();
    }

    void setPosition(const wxPoint& new_pos) override
    {
        this->SetPosition(new_pos);
    }

    void checkBoxCallback(wxCommandEvent& event);

    std::shared_ptr<GuiElementState> getGuiElementState() const override
    {
        std::shared_ptr<CheckboxState> button_state = std::make_shared<CheckboxState>(element_settings_->handle_string,
                                                                                   this->GetValue());

        return button_state;
    }
};

#endif  // MAIN_APPLICATION_GUI_ELEMENT_H_
