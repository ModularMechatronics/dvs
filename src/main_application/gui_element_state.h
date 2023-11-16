#ifndef GUI_ELEMENT_STATE_H
#define GUI_ELEMENT_STATE_H

#include "dvs/enumerations.h"
#include "dvs/fillable_uint8_array.h"

class GuiElementState
{
private:
    dvs::GuiElementType type_;
    std::string handle_string_;

public:
    GuiElementState() : type_{dvs::GuiElementType::Unknown}, handle_string_{""}
    {
    }

    GuiElementState(const dvs::GuiElementType type, const std::string& handle_string)
        : type_{type}, handle_string_{handle_string}
    {
    }
    virtual ~GuiElementState() {}

    virtual std::uint64_t getTotalNumBytes() const
    {
        return handle_string_.length() + sizeof(std::uint8_t) * 2U + sizeof(std::uint32_t);
    }

    virtual void serializeToBuffer(FillableUInt8Array& output_array) const
    {
        output_array.fillWithStaticType(static_cast<std::uint8_t>(type_));
        output_array.fillWithStaticType(static_cast<std::uint8_t>(handle_string_.length()));
        output_array.fillWithDataFromPointer(handle_string_.data(), handle_string_.length());
    }
};

class CheckboxState : public GuiElementState
{
private:
    bool is_checked_;

public:
    CheckboxState() = delete;
    CheckboxState(const std::string& handle_string, const bool is_checked) : GuiElementState(dvs::GuiElementType::CheckBox, handle_string), is_checked_{is_checked}
    {
    }
    ~CheckboxState() override {}

    std::uint64_t getTotalNumBytes() const
    {
        return GuiElementState::getTotalNumBytes() + sizeof(std::uint8_t);
    }

    virtual void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);
        output_array.fillWithStaticType(static_cast<std::uint32_t>(1U)); // Payload size
        output_array.fillWithStaticType(static_cast<std::uint8_t>(is_checked_));
    }
};

class SliderState : public GuiElementState
{
private:
    std::int32_t min_value_;
    std::int32_t max_value_;
    std::int32_t step_size_;
    std::int32_t value_;
    bool is_horizontal_;

public:
    SliderState() = delete;
    SliderState(const std::string& handle_string,
        const std::int32_t min_value,
        const std::int32_t max_value,
        const std::int32_t step_size,
        const std::int32_t value,
        const bool is_horizontal) :
            GuiElementState(dvs::GuiElementType::Slider, handle_string),
            min_value_{min_value},
            max_value_{max_value},
            step_size_{step_size},
            value_{value},
            is_horizontal_{is_horizontal}
    {
    }
    ~SliderState() override {}

    std::uint64_t getTotalNumBytes() const
    {
        return GuiElementState::getTotalNumBytes() + sizeof(std::int32_t) * 4U + sizeof(std::uint8_t);
    }

    virtual void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);

        output_array.fillWithStaticType(static_cast<std::uint32_t>(
            sizeof(std::int32_t) * 4U + sizeof(std::uint8_t))); // Payload size

        output_array.fillWithStaticType(static_cast<std::int32_t>(min_value_));
        output_array.fillWithStaticType(static_cast<std::int32_t>(max_value_));
        output_array.fillWithStaticType(static_cast<std::int32_t>(step_size_));
        output_array.fillWithStaticType(static_cast<std::int32_t>(value_));
        output_array.fillWithStaticType(static_cast<std::uint8_t>(is_horizontal_));
    }
};

class ButtonState : public GuiElementState
{
private:
    bool is_pressed_;

public:
    ButtonState() = delete;
    ButtonState(const std::string& handle_string, const bool is_pressed) : GuiElementState(dvs::GuiElementType::Button, handle_string), is_pressed_{is_pressed}
    {
    }
    ~ButtonState() override {}

    std::uint64_t getTotalNumBytes() const
    {
        return GuiElementState::getTotalNumBytes() + sizeof(std::uint8_t);
    }

    virtual void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);

        output_array.fillWithStaticType(static_cast<std::uint32_t>(1U)); // Payload size
        output_array.fillWithStaticType(static_cast<std::uint8_t>(is_pressed_));
    }
};

#endif  // GUI_ELEMENT_STATE_H
