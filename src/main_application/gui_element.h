#ifndef MAIN_APPLICATION_GUI_ELEMENT_H_
#define MAIN_APPLICATION_GUI_ELEMENT_H_

#include <wx/notebook.h>
#include <wx/wx.h>

#include <functional>
#include <memory>
#include <queue>

#include "axes/structures/grid_vectors.h"
#include "communication/received_data.h"
#include "dvs/enumerations.h"
#include "dvs/math/math.h"
#include "gui_element_state.h"
#include "input_data.h"
#include "project_state/project_settings.h"

using namespace dvs;

enum class CursorSquareState
{
    INSIDE,
    OUTSIDE,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

struct Bound2D
{
    float x_min;
    float x_max;
    float y_min;
    float y_max;

    Bound2D() = default;
};

class ApplicationGuiElement
{
protected:
    std::shared_ptr<ElementSettings> element_settings_;

    std::function<void(const char key)> notify_main_window_key_pressed_;
    std::function<void(const char key)> notify_main_window_key_released_;
    std::function<void(const wxPoint pos, const std::string& elem_name)> notify_parent_window_right_mouse_pressed_;
    std::function<void()> notify_main_window_about_modification_;

    int minimum_x_pos_;
    int minimum_y_pos_;

    wxSize parent_size_;  // Pixels

    wxPoint current_mouse_pos_;
    wxPoint previous_mouse_pos_;
    wxPoint mouse_pos_at_press_;
    bool control_pressed_at_mouse_press_;
    bool left_mouse_pressed_;

    wxPoint pos_at_press_;
    wxSize size_at_press_;

    CursorSquareState cursor_state_at_press_;

    float edit_size_margin_{20.0f};

    void fillWithBasicData(FillableUInt8Array& output_array) const
    {
        const std::uint8_t handle_string_length = element_settings_->handle_string.length();

        output_array.fillWithStaticType(static_cast<std::uint8_t>(element_settings_->type));
        output_array.fillWithStaticType(handle_string_length);
        output_array.fillWithDataFromPointer(element_settings_->handle_string.data(),
                                             element_settings_->handle_string.length());
    }

    std::uint64_t basicDataSize() const
    {
        const std::uint8_t handle_string_length = element_settings_->handle_string.length();

        const std::uint64_t basic_data_size = handle_string_length +  // the handle_string itself
                                              sizeof(std::uint8_t) +  // length of handle_string
                                              sizeof(std::uint8_t) +  // type
                                              sizeof(std::uint32_t);  // payload size
        return basic_data_size;
    }

    virtual std::uint64_t getGuiPayloadSize() const = 0;
    virtual void fillGuiPayload(FillableUInt8Array& output_array) const = 0;

    void sendGuiData()
    {
        if (element_settings_->handle_string.length() >= 256U)
        {
            throw std::runtime_error("Handle string too long! Maximum length is 255 characters!");
        }

        const std::uint64_t num_bytes_to_send{basicDataSize() + getGuiPayloadSize()};

        FillableUInt8Array output_array{num_bytes_to_send};

        fillWithBasicData(output_array);

        output_array.fillWithStaticType(static_cast<std::uint32_t>(getGuiPayloadSize()));

        fillGuiPayload(output_array);
        sendThroughTcpInterface(output_array.view(), kGuiTcpPortNum);
    }

public:
    ApplicationGuiElement() = delete;
    ApplicationGuiElement(const std::shared_ptr<ElementSettings>& element_settings,
                          const std::function<void(const char key)>& notify_main_window_key_pressed,
                          const std::function<void(const char key)>& notify_main_window_key_released,
                          const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                              notify_parent_window_right_mouse_pressed,
                          const std::function<void()>& notify_main_window_about_modification)
        : element_settings_{element_settings},
          notify_main_window_key_pressed_{notify_main_window_key_pressed},
          notify_main_window_key_released_{notify_main_window_key_released},
          notify_parent_window_right_mouse_pressed_{notify_parent_window_right_mouse_pressed},
          notify_main_window_about_modification_{notify_main_window_about_modification}
    {
    }

    virtual ~ApplicationGuiElement() {}

    virtual void setMinXPos(const int min_x_pos) = 0;

    std::string getHandleString() const
    {
        return element_settings_->handle_string;
    }

    virtual void setHandleString(const std::string& new_name)
    {
        element_settings_->handle_string = new_name;
    }

    std::shared_ptr<ElementSettings> getElementSettings() const
    {
        return element_settings_;
    }

    virtual void updateSizeFromParent(const wxSize& parent_size) = 0;
    virtual void keyPressed(const char key) = 0;
    virtual void keyReleased(const char key) = 0;

    void keyPressedCallback_new(wxKeyEvent& evt)
    {
        const int key = evt.GetUnicodeKey();
        notify_main_window_key_pressed_(key);
    }

    void keyReleasedCallback_new(wxKeyEvent& evt)
    {
        const int key = evt.GetUnicodeKey();
        notify_main_window_key_released_(key);
    }

    virtual void mousePressedGuiElementCallback(wxMouseEvent& event) {}

    virtual void mouseReleasedGuiElementCallback(wxMouseEvent& event) {}

    virtual wxPoint getPosition() const
    {
        DVS_LOG_WARNING() << "getPosition() not implemented!";
        return wxPoint{0, 0};
    }

    virtual wxSize getSize() const
    {
        DVS_LOG_WARNING() << "getSize() not implemented!";
        return wxSize{0, 0};
    }

    virtual wxWindow* getParent() const
    {
        DVS_LOG_WARNING() << "getParent() not implemented!";
        return nullptr;
    }

    virtual void setCursor(const wxCursor& cursor)
    {
        DVS_LOG_WARNING() << "setCursor() not implemented!";
    }

    virtual void setPosition(const wxPoint& new_pos)
    {
        DVS_LOG_WARNING() << "setPosition() not implemented!";
    }

    virtual std::shared_ptr<GuiElementState> getGuiElementState() const
    {
        DVS_LOG_WARNING() << "getGuiElementState() not implemented!";
        return std::make_shared<GuiElementState>();
    }

    void mouseEnteredElement(wxMouseEvent& event)
    {
        const wxPoint current_mouse_point = event.GetPosition();
        const wxPoint current_position = this->getPosition();
        previous_mouse_pos_ = current_position + current_mouse_point;

        if (wxGetKeyState(WXK_COMMAND))
        {
            setCursorDependingOnMousePos(event.GetPosition());
        }
    }

    void mouseLeftElement(wxMouseEvent& event)
    {
        this->setCursor(wxCursor(wxCURSOR_ARROW));
    }

    static CursorSquareState getCursorSquareState(const Bound2D bound,
                                                  const Bound2D bound_margin,
                                                  const Vec2f mouse_pos)
    {
        if ((bound.x_min <= mouse_pos.x) && (mouse_pos.x <= bound.x_max) && (bound.y_min <= mouse_pos.y) &&
            (mouse_pos.y <= bound.y_max))
        {
            if (mouse_pos.x <= bound_margin.x_min)
            {
                if (mouse_pos.y <= bound_margin.y_min)
                {
                    return CursorSquareState::TOP_LEFT;
                }
                else if (bound_margin.y_max <= mouse_pos.y)
                {
                    return CursorSquareState::BOTTOM_LEFT;
                }
                else
                {
                    return CursorSquareState::LEFT;
                }
            }
            else if (bound_margin.x_max <= mouse_pos.x)
            {
                if (mouse_pos.y <= bound_margin.y_min)
                {
                    return CursorSquareState::TOP_RIGHT;
                }
                else if (bound_margin.y_max <= mouse_pos.y)
                {
                    return CursorSquareState::BOTTOM_RIGHT;
                }
                else
                {
                    return CursorSquareState::RIGHT;
                }
            }
            else if (mouse_pos.y <= bound_margin.y_min)
            {
                return CursorSquareState::TOP;
            }
            else if (bound_margin.y_max <= mouse_pos.y)
            {
                return CursorSquareState::BOTTOM;
            }
            else
            {
                return CursorSquareState::INSIDE;
            }
        }
        else
        {
            return CursorSquareState::OUTSIDE;
        }
    }

    void setCursorDependingOnMousePos(const wxPoint& current_mouse_position)
    {
        Bound2D bnd;
        bnd.x_min = 0.0f;
        bnd.x_max = this->getSize().GetWidth();
        bnd.y_min = 0.0f;
        bnd.y_max = this->getSize().GetHeight();

        Bound2D bnd_margin;
        bnd_margin.x_min = bnd.x_min + edit_size_margin_;
        bnd_margin.x_max = bnd.x_max - edit_size_margin_;
        bnd_margin.y_min = bnd.y_min + edit_size_margin_;
        bnd_margin.y_max = bnd.y_max - edit_size_margin_;

        const CursorSquareState cms =
            getCursorSquareState(bnd, bnd_margin, Vec2f(current_mouse_position.x, current_mouse_position.y));

        switch (cms)
        {
            case CursorSquareState::LEFT:
                this->setCursor(wxCursor(wxCURSOR_SIZEWE));
                break;
            case CursorSquareState::RIGHT:
                this->setCursor(wxCursor(wxCURSOR_SIZEWE));
                break;
            case CursorSquareState::TOP:
                this->setCursor(wxCursor(wxCURSOR_SIZENS));
                break;
            case CursorSquareState::BOTTOM:
                this->setCursor(wxCursor(wxCURSOR_SIZENS));
                break;
            case CursorSquareState::BOTTOM_RIGHT:
                this->setCursor(wxCursor(wxCURSOR_SIZENWSE));
                break;
            case CursorSquareState::BOTTOM_LEFT:
                this->setCursor(wxCursor(wxCURSOR_SIZENESW));
                break;
            case CursorSquareState::TOP_RIGHT:
                this->setCursor(wxCursor(wxCURSOR_SIZENESW));
                break;
            case CursorSquareState::TOP_LEFT:
                this->setCursor(wxCursor(wxCURSOR_SIZENWSE));
                break;
            case CursorSquareState::INSIDE:
                this->setCursor(wxCursor(wxCURSOR_HAND));
                break;
            default:
                this->setCursor(wxCursor(wxCURSOR_HAND));
        }
    }

    void mouseMovedOverItem(wxMouseEvent& event)
    {
        if (control_pressed_at_mouse_press_ && event.LeftIsDown())
        {
            const wxPoint current_mouse_position_local = event.GetPosition();
            const wxPoint current_mouse_position_global = current_mouse_position_local + this->getPosition();
            const wxPoint delta = current_mouse_position_global - previous_mouse_pos_;
            this->setPosition(this->getPosition() + delta);

            element_settings_->x = this->getPosition().x / static_cast<float>(parent_size_.x);
            element_settings_->y = this->getPosition().y / static_cast<float>(parent_size_.y);

            previous_mouse_pos_ = current_mouse_position_global;

            notify_main_window_about_modification_();
        }
        else
        {
            event.Skip();
            mousePressedGuiElementCallback(event);
        }
    }

    void mouseRightPressed_tmp(wxMouseEvent& event)
    {
        notify_parent_window_right_mouse_pressed_(this->getPosition() + event.GetPosition(),
                                                  element_settings_->handle_string);
    }

    void mouseLeftReleased(wxMouseEvent& event)
    {
        if (control_pressed_at_mouse_press_)
        {
            control_pressed_at_mouse_press_ = false;
        }
        else
        {
            event.Skip();
        }
    }

    void mouseLeftPressed(wxMouseEvent& event)
    {
        if (wxGetKeyState(WXK_CONTROL))
        {
            control_pressed_at_mouse_press_ = true;

            const wxPoint pos_at_press = this->getPosition();

            mouse_pos_at_press_ = event.GetPosition() + pos_at_press;
            previous_mouse_pos_ = mouse_pos_at_press_;
        }
        else
        {
            mouseReleasedGuiElementCallback(event);
            event.Skip();
        }
    }
};

#endif  // MAIN_APPLICATION_GUI_ELEMENT_H_
