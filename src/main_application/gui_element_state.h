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
    GuiElementState() : type_{dvs::GuiElementType::Unknown}, handle_string_{""} {}

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
    CheckboxState(const std::string& handle_string, const bool is_checked)
        : GuiElementState(dvs::GuiElementType::Checkbox, handle_string), is_checked_{is_checked}
    {
    }
    ~CheckboxState() override {}

    std::uint64_t getTotalNumBytes() const override
    {
        return GuiElementState::getTotalNumBytes() + sizeof(std::uint8_t);
    }

    void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);
        output_array.fillWithStaticType(static_cast<std::uint32_t>(1U));  // Payload size
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
                const bool is_horizontal)
        : GuiElementState(dvs::GuiElementType::Slider, handle_string),
          min_value_{min_value},
          max_value_{max_value},
          step_size_{step_size},
          value_{value},
          is_horizontal_{is_horizontal}
    {
    }
    ~SliderState() override {}

    std::uint64_t getTotalNumBytes() const override
    {
        return GuiElementState::getTotalNumBytes() + sizeof(std::int32_t) * 4U + sizeof(std::uint8_t);
    }

    void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);

        output_array.fillWithStaticType(
            static_cast<std::uint32_t>(sizeof(std::int32_t) * 4U + sizeof(std::uint8_t)));  // Payload size

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
    ButtonState(const std::string& handle_string, const bool is_pressed)
        : GuiElementState(dvs::GuiElementType::Button, handle_string), is_pressed_{is_pressed}
    {
    }
    ~ButtonState() override {}

    std::uint64_t getTotalNumBytes() const override
    {
        return GuiElementState::getTotalNumBytes() + sizeof(std::uint8_t);
    }

    void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);

        output_array.fillWithStaticType(static_cast<std::uint32_t>(1U));  // Payload size
        output_array.fillWithStaticType(static_cast<std::uint8_t>(is_pressed_));
    }
};

class TextLabelState : public GuiElementState
{
private:
    std::string label_;

public:
    TextLabelState() = delete;
    TextLabelState(const std::string& handle_string, const std::string& label)
        : GuiElementState(dvs::GuiElementType::TextLabel, handle_string), label_{label}
    {
    }
    ~TextLabelState() override {}

    std::uint64_t getTotalNumBytes() const override
    {
        return GuiElementState::getTotalNumBytes() + sizeof(std::uint8_t) + label_.length();
    }

    void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);

        output_array.fillWithStaticType(static_cast<std::uint32_t>(label_.length() + 1U));  // Payload size
        output_array.fillWithStaticType(static_cast<std::uint8_t>(label_.length()));
        output_array.fillWithDataFromPointer(label_.data(), label_.length());
    }
};

class ListBoxState : public GuiElementState
{
private:
    std::vector<std::string> elements_;
    std::string selected_element_;

public:
    ListBoxState() = delete;
    ListBoxState(const std::string& handle_string,
                 const std::vector<std::string>& elements,
                 const std::string& selected_element)
        : GuiElementState{dvs::GuiElementType::ListBox, handle_string},
          elements_{elements},
          selected_element_{selected_element}
    {
    }
    ~ListBoxState() override {}

    std::uint64_t getTotalNumBytes() const override
    {
        std::uint64_t total_num_bytes = GuiElementState::getTotalNumBytes();
        total_num_bytes += sizeof(std::uint8_t) + selected_element_.length();

        total_num_bytes += sizeof(std::uint16_t);  // Number of elements

        for (const auto& element : elements_)
        {
            total_num_bytes += sizeof(std::uint8_t) + element.length();
        }
        return total_num_bytes;
    }

    void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);

        std::uint32_t payload_size = sizeof(std::uint8_t) + selected_element_.length();

        payload_size += sizeof(std::uint16_t);  // Number of elements

        for (const auto& element : elements_)
        {
            payload_size += sizeof(std::uint8_t) + element.length();
        }

        output_array.fillWithStaticType(payload_size);
        output_array.fillWithStaticType(static_cast<std::uint8_t>(selected_element_.length()));
        output_array.fillWithDataFromPointer(selected_element_.data(), selected_element_.length());

        output_array.fillWithStaticType(static_cast<std::uint16_t>(elements_.size()));

        for (const auto& element : elements_)
        {
            output_array.fillWithStaticType(static_cast<std::uint8_t>(element.length()));
            output_array.fillWithDataFromPointer(element.data(), element.length());
        }
    }
};

class EditableTextState : public GuiElementState
{
private:
    bool enter_pressed_;
    std::string text_;

public:
    EditableTextState() = delete;
    EditableTextState(const std::string& handle_string, const bool enter_pressed, const std::string& text)
        : GuiElementState{dvs::GuiElementType::EditableText, handle_string}, enter_pressed_{enter_pressed}, text_{text}
    {
    }
    ~EditableTextState() override {}

    std::uint64_t getTotalNumBytes() const override
    {
        std::uint64_t total_num_bytes = GuiElementState::getTotalNumBytes();
        total_num_bytes += sizeof(std::uint8_t) + sizeof(std::uint8_t) + text_.length();

        return total_num_bytes;
    }

    void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);

        const std::uint32_t payload_size = sizeof(std::uint8_t) + sizeof(std::uint8_t) + text_.length();

        output_array.fillWithStaticType(payload_size);
        output_array.fillWithStaticType(static_cast<std::uint8_t>(enter_pressed_));
        output_array.fillWithStaticType(static_cast<std::uint8_t>(text_.length()));
        output_array.fillWithDataFromPointer(text_.data(), text_.length());
    }
};

class DropdownMenuState : public GuiElementState
{
private:
    std::vector<std::string> elements_;
    std::string selected_element_;

public:
    DropdownMenuState() = delete;
    DropdownMenuState(const std::string& handle_string,
                      const std::vector<std::string>& elements,
                      const std::string& selected_element)
        : GuiElementState{dvs::GuiElementType::DropdownMenu, handle_string},
          elements_{elements},
          selected_element_{selected_element}
    {
    }
    ~DropdownMenuState() override {}

    std::uint64_t getTotalNumBytes() const override
    {
        std::uint64_t total_num_bytes = GuiElementState::getTotalNumBytes();
        total_num_bytes += sizeof(std::uint8_t) + selected_element_.length();

        total_num_bytes += sizeof(std::uint16_t);  // Number of elements

        for (const auto& element : elements_)
        {
            total_num_bytes += sizeof(std::uint8_t) + element.length();
        }
        return total_num_bytes;
    }

    void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);

        std::uint32_t payload_size = sizeof(std::uint8_t) + selected_element_.length();

        payload_size += sizeof(std::uint16_t);  // Number of elements

        for (const auto& element : elements_)
        {
            payload_size += sizeof(std::uint8_t) + element.length();
        }

        output_array.fillWithStaticType(payload_size);
        output_array.fillWithStaticType(static_cast<std::uint8_t>(selected_element_.length()));
        output_array.fillWithDataFromPointer(selected_element_.data(), selected_element_.length());

        output_array.fillWithStaticType(static_cast<std::uint16_t>(elements_.size()));

        for (const auto& element : elements_)
        {
            output_array.fillWithStaticType(static_cast<std::uint8_t>(element.length()));
            output_array.fillWithDataFromPointer(element.data(), element.length());
        }
    }
};

class RadioButtonGroupState : public GuiElementState
{
private:
    std::vector<std::string> buttons_;
    std::int32_t selected_button_;

public:
    RadioButtonGroupState() = delete;
    RadioButtonGroupState(const std::string& handle_string,
                          const std::vector<std::string>& buttons,
                          const std::int32_t selected_button)
        : GuiElementState{dvs::GuiElementType::RadioButtonGroup, handle_string},
          buttons_{buttons},
          selected_button_{selected_button}
    {
    }
    ~RadioButtonGroupState() override {}

    std::uint64_t getTotalNumBytes() const override
    {
        std::uint64_t total_num_bytes = GuiElementState::getTotalNumBytes();

        total_num_bytes += sizeof(std::uint16_t);  // Number of buttons
        total_num_bytes += sizeof(std::uint32_t);  // Selected button

        for (const auto& button : buttons_)
        {
            total_num_bytes += sizeof(std::uint8_t) + button.length();
        }

        return total_num_bytes;
    }

    void serializeToBuffer(FillableUInt8Array& output_array) const override
    {
        GuiElementState::serializeToBuffer(output_array);

        std::uint32_t payload_size = 0U;

        payload_size += sizeof(std::uint16_t);  // Number of buttons
        payload_size += sizeof(std::uint32_t);  // Selected button

        for (const auto& button : buttons_)
        {
            payload_size += sizeof(std::uint8_t) + button.length();
        }

        output_array.fillWithStaticType(payload_size);
        output_array.fillWithStaticType(selected_button_);

        output_array.fillWithStaticType(static_cast<std::uint16_t>(buttons_.size()));

        for (const auto& button : buttons_)
        {
            output_array.fillWithStaticType(static_cast<std::uint8_t>(button.length()));
            output_array.fillWithDataFromPointer(button.data(), button.length());
        }
    }
};

#endif  // GUI_ELEMENT_STATE_H
