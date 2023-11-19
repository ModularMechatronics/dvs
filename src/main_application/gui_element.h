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

    bool control_pressed_at_mouse_down_;
    wxPoint previous_mouse_pos_;
    wxPoint mouse_pos_at_press_;

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

        const std::uint64_t basic_data_size = handle_string_length + // the handle_string itself
            sizeof(std::uint8_t) + // length of handle_string
            sizeof(std::uint8_t) + // type
            sizeof(std::uint32_t); // payload size
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

        const std::uint64_t num_bytes_to_send{
            basicDataSize() + 
            getGuiPayloadSize()
        };

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
    virtual void refresh() = 0;
    virtual void update() = 0;

    virtual std::shared_ptr<GuiElementState> getGuiElementState() const
    {
        std::cout << "getGuiElementState() not implemented!" << std::endl;
        return std::make_shared<GuiElementState>();
    }
};

#endif  // MAIN_APPLICATION_GUI_ELEMENT_H_
