#ifndef MAIN_APPLICATION_GUI_ELEMENT_H_
#define MAIN_APPLICATION_GUI_ELEMENT_H_

#include <wx/notebook.h>
#include <wx/wx.h>

#include <functional>
#include <memory>

#include "communication/received_data.h"
#include "dvs/enumerations.h"
#include "dvs/math/math.h"
#include "project_state/project_settings.h"

using namespace dvs;

class GuiElement
{
protected:
    ElementSettings element_settings_;
    std::function<void(const char key)> notify_main_window_key_pressed_;
    std::function<void(const char key)> notify_main_window_key_released_;
    std::function<void(const wxPoint pos, const std::string& elem_name)> notify_parent_window_right_mouse_pressed_;
    std::function<void(const GuiElement* const)> notify_main_window_element_deleted_;

    int minimum_x_pos_;
    Vec2f parent_size_;  // Pixels

public:
    GuiElement() = delete;
    GuiElement(const ElementSettings& element_settings) : element_settings_{element_settings} {}
    GuiElement(const ElementSettings& element_settings,
               const std::function<void(const char key)>& notify_main_window_key_pressed,
               const std::function<void(const char key)>& notify_main_window_key_released,
               const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                   notify_parent_window_right_mouse_pressed,
               const std::function<void(const GuiElement* const)>& notify_main_window_element_deleted)
        : element_settings_{element_settings},
          notify_main_window_key_pressed_{notify_main_window_key_pressed},
          notify_main_window_key_released_{notify_main_window_key_released},
          notify_parent_window_right_mouse_pressed_{notify_parent_window_right_mouse_pressed},
          notify_main_window_element_deleted_{notify_main_window_element_deleted}
    {
    }

    virtual ~GuiElement() {}

    virtual void setMinXPos(const int min_x_pos) = 0;

    std::string getName() const
    {
        return element_settings_.name;
    }

    void setName(const std::string& new_name)
    {
        element_settings_.name = new_name;
    }

    ElementSettings getElementSettings() const
    {
        return element_settings_;
    }

    virtual void updateSizeFromParent(const wxSize& parent_size) = 0;
    virtual void addData(std::unique_ptr<const ReceivedData> received_data,
                         const dvs::internal::CommunicationHeader& hdr) = 0;
    virtual void keyPressed(const char key) = 0;
    virtual void keyReleased(const char key) = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void destroy() = 0;
    virtual void refresh() = 0;
    virtual void waitForFlush() = 0;
    virtual void showLegend(const bool show_legend)
    {
        static_cast<void>(show_legend);
        std::cout << "Not implemented!" << std::endl;
    }
};

#endif  // MAIN_APPLICATION_GUI_ELEMENT_H_
