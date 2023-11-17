#ifndef MODULE_API2_H
#define MODULE_API2_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "dvs/enumerations.h"
#include "dvs/logging.h"
#include "dvs/fillable_uint8_array.h"

namespace dvs
{

namespace internal
{
class InternalGuiElementHandle
{
private:
    std::string handle_string_;
    dvs::GuiElementType type_;

public:
    InternalGuiElementHandle() {}
    InternalGuiElementHandle(const std::string& handle_string, const dvs::GuiElementType type) :
        handle_string_{handle_string}, type_{type}
    {}

    dvs::GuiElementType getType() const
    {
        return type_;
    }

    std::string getHandleString() const
    {
        return handle_string_;
    }

    virtual void updateState(const UInt8ArrayView& data_view) = 0;
};

class SliderInternal : public InternalGuiElementHandle
{
public:
    std::int32_t min_value_;
    std::int32_t max_value_;
    std::int32_t step_size_;
    std::int32_t value_;

public:
    SliderInternal(const std::string& handle_string, const UInt8ArrayView& data_view) :
        InternalGuiElementHandle{handle_string, dvs::GuiElementType::Slider}
    {
        const std::uint8_t* const raw_gui_data = data_view.data();
        min_value_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data);
        max_value_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + sizeof(std::int32_t));
        step_size_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + 2U * sizeof(std::int32_t));
        value_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + 3U * sizeof(std::int32_t));
    }

    void updateState(const UInt8ArrayView& data_view) override
    {
        const std::uint8_t* const raw_gui_data = data_view.data();
        min_value_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data);
        max_value_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + sizeof(std::int32_t));
        step_size_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + 2U * sizeof(std::int32_t));
        value_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + 3U * sizeof(std::int32_t));
    }

};

class ButtonInternal : public InternalGuiElementHandle
{
public:
    bool is_pressed_;

public:
    ButtonInternal() {}
    ButtonInternal(const std::string& handle_string, const UInt8ArrayView& data_view) : 
        InternalGuiElementHandle{handle_string, dvs::GuiElementType::Button}
    {
        is_pressed_ = data_view.data()[0];
    }

    void updateState(const UInt8ArrayView& data_view) override
    {
        is_pressed_ = data_view.data()[0];
    }
};

class CheckboxInternal : public InternalGuiElementHandle
{
public:
    bool is_checked_;

public:
    CheckboxInternal() {}
    CheckboxInternal(const std::string& handle_string, const UInt8ArrayView& data_view) :
    InternalGuiElementHandle{handle_string, dvs::GuiElementType::CheckBox}
    {
        is_checked_ = data_view.data()[0];
    }

    void updateState(const UInt8ArrayView& data_view) override
    {
        is_checked_ = data_view.data()[0];
        std::cout << "Checkbox state: " << is_checked_ << std::endl;
    }
};

inline void throwExceptionIfPointerIsNotInitialized(const std::shared_ptr<internal::SliderInternal>& internal_ptr)
{
    if (internal_ptr == nullptr)
    {
        throw std::runtime_error("Pointer not initialized!");
    }
}

}  // namespace internal

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

    void setMinValue(const std::int32_t new_min_value)
    {
        throwExceptionIfPointerIsNotInitialized(internal_ptr_);
        // TODO...
    }

    void setMaxValue(const std::int32_t new_max_value)
    {
        throwExceptionIfPointerIsNotInitialized(internal_ptr_);
        // TODO...
    }

    void setStepSize(const std::int32_t new_step_size)
    {
        throwExceptionIfPointerIsNotInitialized(internal_ptr_);
        // TODO...
    }

    void setValue(const std::int32_t new_value)
    {
        throwExceptionIfPointerIsNotInitialized(internal_ptr_);
        // TODO...
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
    ButtonHandle() {}
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
    CheckboxHandle() {}

    bool getIsChecked() const
    {
        return internal_ptr_->is_checked_;
    }
};

class GuiElementHandle
{
private:
    std::string handle_string_;
    dvs::GuiElementType type_;

    std::shared_ptr<internal::InternalGuiElementHandle> internal_handle_;

public:
    GuiElementHandle() {}
    GuiElementHandle(const std::string& handle_string,
                     const dvs::GuiElementType type,
                     const UInt8ArrayView& data_view)
        : handle_string_{handle_string}, type_{type}
    {
    }
};

inline void registerGuiCallback_new(const std::string& handle_string, std::function<void(const SliderHandle&)> callback_function)
{

}

inline void registerGuiCallback_new(const std::string& handle_string, std::function<void(const ButtonHandle&)> callback_function)
{

}

inline void registerGuiCallback_new(const std::string& handle_string, std::function<void(const CheckboxHandle&)> callback_function)
{

}


}  // namespace dvs

#endif  // MODULE_API_H
