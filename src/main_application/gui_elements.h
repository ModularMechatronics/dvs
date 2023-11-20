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

    void keyPressed(const char key) override
    {
        if (wxGetKeyState(WXK_COMMAND))
        {
            wxWindow* parent_handle = this->getParent();

            const float x0 = static_cast<float>(this->getPosition().x);
            const float y0 = static_cast<float>(this->getPosition().y);

            const float x1 = x0 + static_cast<float>(this->getSize().GetWidth());
            const float y1 = y0 + static_cast<float>(this->getSize().GetHeight());

            const wxPoint pt = wxGetMousePosition() - parent_handle->GetPosition();

            if ((pt.x > x0) && (pt.x < x1) && (pt.y > y0) && (pt.y < y1))
            {
                setCursorDependingOnMousePos(pt - this->getPosition());
            }
        }
    }

    void keyReleased(const char key) override
    {
        if (!wxGetKeyState(WXK_COMMAND))
        {
            this->setCursor(wxCursor(wxCURSOR_ARROW));
        }
    }

    wxPoint getPosition() const override
    {
        return this->GetPosition();
    }

    wxSize getSize() const override
    {
        return this->GetSize();
    }

    void setCursor(const wxCursor& cursor) override
    {
        wxSetCursor(cursor);
    }

    wxWindow* getParent() const override
    {
        return this->GetParent();
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
        std::shared_ptr<ButtonState> button_state =
            std::make_shared<ButtonState>(element_settings_->handle_string, is_pressed_);

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

    wxSize getSize() const override
    {
        return this->GetSize();
    }

    void setCursor(const wxCursor& cursor) override
    {
        wxSetCursor(cursor);
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

    wxSize getSize() const override
    {
        return this->GetSize();
    }

    void setCursor(const wxCursor& cursor) override
    {
        wxSetCursor(cursor);
    }

    void setPosition(const wxPoint& new_pos) override
    {
        this->SetPosition(new_pos);
    }

    void checkBoxCallback(wxCommandEvent& event);

    std::shared_ptr<GuiElementState> getGuiElementState() const override
    {
        std::shared_ptr<CheckboxState> button_state =
            std::make_shared<CheckboxState>(element_settings_->handle_string, this->GetValue());

        return button_state;
    }
};

#endif  // MAIN_APPLICATION_GUI_ELEMENT_H_
