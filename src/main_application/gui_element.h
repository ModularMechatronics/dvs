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

    bool pointIsWithin(const wxPoint& point) const
    {
        return (x_min <= point.x) && (point.x <= x_max) && (y_min <= point.y) && (point.y <= y_max);
    }
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

    wxPoint element_pos_at_press_;
    wxSize element_size_at_press_;

    bool control_pressed_at_mouse_press_;
    bool left_mouse_pressed_;

    CursorSquareState cursor_state_at_press_;

    float edit_size_margin_{5.0f};

    void fillWithBasicData(FillableUInt8Array& output_array) const;
    std::uint64_t basicDataSize() const;
    virtual std::uint64_t getGuiPayloadSize() const = 0;
    virtual void fillGuiPayload(FillableUInt8Array& output_array) const = 0;

    void sendGuiData();

    std::pair<Bound2D, Bound2D> getBounds() const
    {
        Bound2D bnd;
        bnd.x_min = 0.0f;
        bnd.x_max = this->getSize().GetWidth();
        bnd.y_min = 0.0f;
        bnd.y_max = this->getSize().GetHeight();

        Bound2D bnd_with_margin;
        bnd_with_margin.x_min = bnd.x_min + edit_size_margin_;
        bnd_with_margin.x_max = bnd.x_max - edit_size_margin_;
        bnd_with_margin.y_min = bnd.y_min + edit_size_margin_;
        bnd_with_margin.y_max = bnd.y_max - edit_size_margin_;

        return std::make_pair(bnd, bnd_with_margin);
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

    static CursorSquareState getCursorSquareState(const Bound2D bound,
                                                  const Bound2D bound_margin,
                                                  const wxPoint mouse_pos);

    void setCursorDependingOnMousePos(const wxPoint& current_mouse_position);

    void adjustPaneSizeOnMouseMoved();
    virtual void setElementPositionAndSize()
    {
        DVS_LOG_WARNING() << "setElementPositionAndSize() not implemented!";
    }

    // Keyboard functions
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

    // Mouse functions
    virtual void mousePressedGuiElementSpecific(wxMouseEvent& event) {}

    virtual void mouseReleasedGuiElementSpecific(wxMouseEvent& event) {}

    void mouseEnteredElement(wxMouseEvent& event);
    void mouseLeftElement(wxMouseEvent& event);

    void mouseMovedOverItem(wxMouseEvent& event);

    void mouseLeftReleased(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
};

#endif  // MAIN_APPLICATION_GUI_ELEMENT_H_
