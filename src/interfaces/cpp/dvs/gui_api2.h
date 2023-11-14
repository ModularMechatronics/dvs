#ifndef MODULE_API2_H
#define MODULE_API2_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "dvs/enumerations.h"
// #include "dvs/gui_api.h"
#include "dvs/logging.h"

namespace dvs
{

namespace internal
{
class InternalGuiElementHandle
{
private:
public:
    InternalGuiElementHandle() {}
};
}  // namespace internal

class SliderInternal
{
private:
    std::int32_t min_value;
    std::int32_t max_value;
    std::int32_t step_size;
    std::int32_t value;

    std::string handle_string_;

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

class Slider
{
private:
    std::int32_t min_value;
    std::int32_t max_value;
    std::int32_t step_size;
    std::int32_t value;

    std::shared_ptr<SliderInternal> internal_ptr_;

public:
    Slider() {}
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
                     const std::uint8_t* const raw_gui_data)
        : handle_string_{handle_string}, type_{type}
    {
    }
};

}  // namespace dvs

#endif  // MODULE_API_H
