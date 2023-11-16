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
};
}  // namespace internal

class SliderInternal
{
public:
    std::int32_t min_value;
    std::int32_t max_value;
    std::int32_t step_size;
    std::int32_t value;

public:
    SliderInternal() {}
    SliderInternal(const std::uint8_t* const raw_gui_data)
    {
        min_value = *reinterpret_cast<const std::int32_t* const>(raw_gui_data);
        max_value = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + sizeof(std::int32_t));
        step_size = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + 2U * sizeof(std::int32_t));
        value = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + 3U * sizeof(std::int32_t));
    }
};

class SliderHandle
{
private:

    std::shared_ptr<SliderInternal> internal_ptr_;

public:
    SliderHandle() {}
    std::int32_t getMinValue() const
    {
        return internal_ptr_->min_value;
    }

    std::int32_t getMaxValue() const
    {
        return internal_ptr_->max_value;
    }

    std::int32_t getStepSize() const
    {
        return internal_ptr_->step_size;
    }

    std::int32_t getValue() const
    {
        return internal_ptr_->value;
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

}  // namespace dvs

#endif  // MODULE_API_H
