#ifndef GUI_API_H
#define GUI_API_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "dvs/enumerations.h"
#include "dvs/logging.h"
#include "dvs/fillable_uint8_array.h"
#include "dvs/gui_internal.h"

namespace dvs
{
namespace gui
{
class SliderHandle
{
private:
    std::shared_ptr<internal::SliderInternal> internal_ptr_;

    template <typename T> friend T getGuiElementHandle(const std::string& handle_string);

    SliderHandle(const std::shared_ptr<internal::InternalGuiElementHandle>& internal_ptr) : 
        internal_ptr_{std::dynamic_pointer_cast<internal::SliderInternal>(internal_ptr)} {}

public:
    SliderHandle() : internal_ptr_{nullptr} {}

    std::int32_t getMinValue() const
    {
        throwExceptionIfPointerIsNotInitialized(internal_ptr_);
        return internal_ptr_->min_value_;
    }

    std::int32_t getMaxValue() const
    {
        throwExceptionIfPointerIsNotInitialized(internal_ptr_);
        return internal_ptr_->max_value_;
    }

    std::int32_t getStepSize() const
    {
        throwExceptionIfPointerIsNotInitialized(internal_ptr_);
        return internal_ptr_->step_size_;
    }

    std::int32_t getValue() const
    {
        throwExceptionIfPointerIsNotInitialized(internal_ptr_);
        return internal_ptr_->value_;
    }
};

class ButtonHandle
{
private:

    std::shared_ptr<internal::ButtonInternal> internal_ptr_;
    template <typename T> friend T getGuiElementHandle(const std::string& handle_string);

    ButtonHandle(const std::shared_ptr<internal::InternalGuiElementHandle>& internal_ptr) : 
        internal_ptr_{std::dynamic_pointer_cast<internal::ButtonInternal>(internal_ptr)} {}

public:
    ButtonHandle() : internal_ptr_{nullptr} {}

    bool getIsPressed() const
    {
        return internal_ptr_->is_pressed_;
    }
};

class CheckboxHandle
{
private:
    std::shared_ptr<internal::CheckboxInternal> internal_ptr_;
    template <typename T> friend T getGuiElementHandle(const std::string& handle_string);

    CheckboxHandle(const std::shared_ptr<internal::InternalGuiElementHandle>& internal_ptr) : 
        internal_ptr_{std::dynamic_pointer_cast<internal::CheckboxInternal>(internal_ptr)} {}

public:
    CheckboxHandle() : internal_ptr_{nullptr} {}

    bool getIsChecked() const
    {
        return internal_ptr_->is_checked_;
    }
};

using SliderCallbackFunction = std::function<void(const SliderHandle&)>;
using ButtonCallbackFunction = std::function<void(const ButtonHandle&)>;
using CheckboxCallbackFunction = std::function<void(const CheckboxHandle&)>;

}

namespace internal
{

inline std::map<std::string, gui::SliderCallbackFunction>& getSliderCallbacks()
{
    static std::map<std::string, gui::SliderCallbackFunction> gui_callbacks;

    return gui_callbacks;
}

inline std::map<std::string, gui::ButtonCallbackFunction>& getButtonCallbacks()
{
    static std::map<std::string, gui::ButtonCallbackFunction> gui_callbacks;

    return gui_callbacks;
}

inline std::map<std::string, gui::CheckboxCallbackFunction>& getCheckboxCallbacks()
{
    static std::map<std::string, gui::CheckboxCallbackFunction> gui_callbacks;

    return gui_callbacks;
}

}

namespace gui
{

inline void registerGuiCallback(const std::string& handle_string, std::function<void(const SliderHandle&)> callback_function)
{
    std::map<std::string, SliderCallbackFunction>& gui_callbacks = internal::getSliderCallbacks();

    if (gui_callbacks.find(handle_string) != gui_callbacks.end())
    {
        DVS_LOG_WARNING() << "Gui callback with name " << handle_string << " already exists!";
        return;
    }

    gui_callbacks[handle_string] = callback_function;
}

inline void registerGuiCallback(const std::string& handle_string, std::function<void(const ButtonHandle&)> callback_function)
{
    std::map<std::string, ButtonCallbackFunction>& gui_callbacks = internal::getButtonCallbacks();

    if (gui_callbacks.find(handle_string) != gui_callbacks.end())
    {
        DVS_LOG_WARNING() << "Gui callback with name " << handle_string << " already exists!";
        return;
    }

    gui_callbacks[handle_string] = callback_function;
}

inline void registerGuiCallback(const std::string& handle_string, std::function<void(const CheckboxHandle&)> callback_function)
{
    std::map<std::string, CheckboxCallbackFunction>& gui_callbacks = internal::getCheckboxCallbacks();

    if (gui_callbacks.find(handle_string) != gui_callbacks.end())
    {
        DVS_LOG_WARNING() << "Gui callback with name " << handle_string << " already exists!";
        return;
    }

    gui_callbacks[handle_string] = callback_function;
}

template <typename T> T getGuiElementHandle(const std::string& handle_string);

template <> inline SliderHandle getGuiElementHandle(const std::string& handle_string)
{
    std::map<std::string, std::shared_ptr<internal::InternalGuiElementHandle>>& gui_element_handles = internal::getGuiElementHandles();

    if(gui_element_handles.count(handle_string) == 0U)
    {
        throw std::runtime_error("Gui element with handle string " + handle_string + " does not exist!");
    }

    const std::shared_ptr<internal::InternalGuiElementHandle> gui_element{gui_element_handles[handle_string]};

    if(gui_element->getType() != dvs::GuiElementType::Slider)
    {
        throw std::runtime_error("Gui element with handle string " + handle_string + " is not a slider!");
    }

    return SliderHandle{gui_element};
}

template <> inline ButtonHandle getGuiElementHandle(const std::string& handle_string)
{
    std::map<std::string, std::shared_ptr<internal::InternalGuiElementHandle>>& gui_element_handles = internal::getGuiElementHandles();

    if(gui_element_handles.count(handle_string) == 0U)
    {
        throw std::runtime_error("Gui element with handle string " + handle_string + " does not exist!");
    }

    const std::shared_ptr<internal::InternalGuiElementHandle> gui_element{gui_element_handles[handle_string]};

    if(gui_element->getType() != dvs::GuiElementType::Button)
    {
        throw std::runtime_error("Gui element with handle string " + handle_string + " is not a button!");
    }

    return ButtonHandle{gui_element};
}

template <> inline CheckboxHandle getGuiElementHandle(const std::string& handle_string)
{
    std::map<std::string, std::shared_ptr<internal::InternalGuiElementHandle>>& gui_element_handles = internal::getGuiElementHandles();

    if(gui_element_handles.count(handle_string) == 0U)
    {
        throw std::runtime_error("Gui element with handle string " + handle_string + " does not exist!");
    }

    const std::shared_ptr<internal::InternalGuiElementHandle> gui_element{gui_element_handles[handle_string]};

    if(gui_element->getType() != dvs::GuiElementType::CheckBox)
    {
        throw std::runtime_error("Gui element with handle string " + handle_string + " is not a checkbox!");
    }

    return CheckboxHandle{gui_element};
}

}  // namespace gui

namespace internal
{

inline void callGuiCallbackFunction(const ReceivedGuiData& received_gui_data)
{
    size_t idx = 0U;

    const std::uint8_t* const raw_data = received_gui_data.data();

    const dvs::GuiElementType type{raw_data[idx]};
    idx += sizeof(std::uint8_t);

    const std::uint8_t handle_string_length = raw_data[1];
    idx += sizeof(std::uint8_t);

    std::string handle_string = "";

    for(std::size_t k = 0; k < handle_string_length; k++)
    {
        handle_string.push_back(raw_data[idx]);
        idx += sizeof(std::uint8_t);
    }

    std::uint32_t payload_size;
    std::memcpy(&payload_size, raw_data + idx, sizeof(std::uint32_t));
    idx += sizeof(std::uint32_t);

    UInt8ArrayView payload_data_view{raw_data + idx, payload_size};

    if(type == dvs::GuiElementType::Slider)
    {
        std::map<std::string, gui::SliderCallbackFunction>& gui_callbacks = getSliderCallbacks();

        if (gui_callbacks.find(handle_string) != gui_callbacks.end())
        {
            gui_callbacks[handle_string](gui::getGuiElementHandle<gui::SliderHandle>(handle_string));
        }
    }
    else if(type == dvs::GuiElementType::Button)
    {
        std::map<std::string, gui::ButtonCallbackFunction>& gui_callbacks = getButtonCallbacks();

        if (gui_callbacks.find(handle_string) != gui_callbacks.end())
        {
            gui_callbacks[handle_string](gui::getGuiElementHandle<gui::ButtonHandle>(handle_string));
        }
    }
    else if(type == dvs::GuiElementType::CheckBox)
    {
        std::map<std::string, gui::CheckboxCallbackFunction>& gui_callbacks = getCheckboxCallbacks();

        if (gui_callbacks.find(handle_string) != gui_callbacks.end())
        {
            gui_callbacks[handle_string](gui::getGuiElementHandle<gui::CheckboxHandle>(handle_string));
        }
    }
}

}

namespace gui
{
inline void startGuiReceiveThread()
{
    internal::initTcpSocket();

    if(internal::isDvsRunning())
    {
        std::thread query_thread([]() {
            // Sleep 100ms in order for client execution to create the waiting
            // TCP connection that receives the data GUI from the DVS application
            usleep(1000U * 100U);
            internal::queryForSyncOfGuiData();
        
        });
        query_thread.detach();
    }

    internal::waitForSyncForAllGuiElements();

    std::thread gui_receive_thread([]() {
        while (true)
        {
            // TODO: Fix case where client app has already been updated once,
            // and then dvs restarts and tries to submit gui data again.
            // Preferebly that should just be handled by this loop, as
            // any normal gui element update.

            // receiveGuiData is a blocking method
            const internal::ReceivedGuiData received_data{internal::receiveGuiData()};

            // updateGuiState must execute before callGuiCallbackFunction to properly
            // update the gui element state before calling the callback function.
            internal::updateGuiState(received_data);
            internal::callGuiCallbackFunction(received_data);
        }
    });

    gui_receive_thread.detach();

    std::thread dvs_application_heart_beat_monitor_thread([]() {
        while (true)
        {
            // TODO: If dvs is not running, kill gui_receive_thread and call waitForSyncForAllGuiElements again
            usleep(1000U * 1000U);
            if (internal::isDvsRunning())
            {
                // DVS_LOG_INFO() << "DVS is running!";
            }
            else
            {
                // DVS_LOG_ERROR() << "DVS is not running!";
            }
        }
    });

    dvs_application_heart_beat_monitor_thread.detach();
}

}  // namespace gui

}  // namespace dvs

#endif  // GUI_API_H
