#ifndef MODULE_API_H
#define MODULE_API_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "dvs/dvs.h"

using namespace dvs;

enum class GuiElementType
{
    Button,
    Slider,
    CheckBox,
    EditableText,
    DropDownMenu,
    ListBox,
    RadioButton,
    RadioButtonGroup,
    TextLabel,
    StaticBox
};

inline std::string guiElementTypeToString(const GuiElementType& gui_element_type)
{
    switch (gui_element_type)
    {
        case GuiElementType::Button:
            return "Button";
        case GuiElementType::Slider:
            return "Slider";
        case GuiElementType::CheckBox:
            return "CheckBox";
        case GuiElementType::EditableText:
            return "EditableText";
        case GuiElementType::DropDownMenu:
            return "DropDownMenu";
        case GuiElementType::ListBox:
            return "ListBox";
        case GuiElementType::RadioButton:
            return "RadioButton";
        case GuiElementType::TextLabel:
            return "TextLabel";
        default:
            DVS_LOG_ERROR() << "Unknown GuiElementType! Returning empty string.";
            return std::string{};
    }
}

class GuiElement;

using GuiElementCallback = std::function<void(const GuiElement&)>;
namespace api_internal
{
class InternalGuiElement
{
public:
    InternalGuiElement(const std::string& handle_string,
                       const GuiElementCallback& elem_callback,
                       const GuiElementType type)
        : type_{type}, callback_function_{elem_callback}, handle_string_{handle_string}
    {
    }

    virtual ~InternalGuiElement() = default;

    virtual void setEnabled() = 0;
    virtual void setDisabled() = 0;
    virtual void setPosition(const std::int16_t x, const std::int16_t y) = 0;
    virtual void setSize(const std::int16_t width, const std::int16_t height) = 0;
    virtual void setLabel(const std::string& new_label) = 0;
    virtual std::string getLabel(void) = 0;
    virtual bool isChecked() const = 0;
    virtual void setChecked() = 0;
    virtual void setUnChecked() = 0;
    virtual void addItem(const std::string& item_text) = 0;
    virtual void removeItem(const std::string& item_text) = 0;
    virtual void clearItems() = 0;
    virtual void selectItem(const std::string& item_text) = 0;
    virtual void selectItem(const std::int32_t item_idx) = 0;
    virtual std::string getSelectedItem() const = 0;
    virtual std::int32_t getNumItems() const = 0;
    virtual std::int32_t getSelectedItemIndex() const = 0;
    virtual void setText(const std::string& new_text) = 0;
    virtual std::string getText() const = 0;
    virtual bool enterPressed() const = 0;
    virtual std::int32_t getValue() const = 0;
    virtual std::int32_t getMin() const = 0;
    virtual std::int32_t getMax() const = 0;
    virtual void setValue(const std::int32_t new_value) = 0;
    virtual void setMin(const std::int32_t new_min) = 0;
    virtual void setMax(const std::int32_t new_max) = 0;
    virtual long getId() const = 0;

    void callback();

    GuiElementType getType() const
    {
        return type_;
    }

    std::string getHandleString() const
    {
        return handle_string_;
    }

protected:
    GuiElementType type_;
    GuiElementCallback callback_function_;
    std::string handle_string_;

    float x_;
    float y_;
    float width_;
    float height_;
};
}  // namespace api_internal

namespace api_internal
{

class GuiElementBase
{
public:
    GuiElementBase() : internal_element_{nullptr} {}
    GuiElementBase(InternalGuiElement* const internal_element) : internal_element_{internal_element} {}

    GuiElementType getType() const
    {
        return internal_element_->getType();
    }

    long getId() const
    {
        return internal_element_->getId();
    }

    std::string getHandleString() const
    {
        return internal_element_->getHandleString();
    }

    void setEnabled() const
    {
        internal_element_->setEnabled();
    }
    void setDisabled() const
    {
        internal_element_->setDisabled();
    }

    void setPosition(const std::int16_t x, const std::int16_t y) const
    {
        internal_element_->setPosition(x, y);
    }

    void setSize(const std::int16_t width, const std::int16_t height) const
    {
        internal_element_->setSize(width, height);
    }

protected:
    InternalGuiElement* internal_element_;
};

class Control : public GuiElementBase
{
private:
public:
    Control() : GuiElementBase{} {}
    Control(InternalGuiElement* const internal_element) : GuiElementBase{internal_element} {}

    void setLabel(const std::string& new_label) const
    {
        internal_element_->setLabel(new_label);
    }

    std::string getLabel() const
    {
        return internal_element_->getLabel();
    }
};

class ListCommon : public GuiElementBase
{
private:
public:
    ListCommon() : GuiElementBase{} {}
    ListCommon(InternalGuiElement* const internal_element) : GuiElementBase{internal_element} {}

    void addItem(const std::string& item_text) const
    {
        internal_element_->addItem(item_text);
    }

    void removeItem(const std::string& item_text) const
    {
        internal_element_->removeItem(item_text);
    }

    void clearItems() const
    {
        internal_element_->clearItems();
    }

    void selectItem(const std::string& item_text) const
    {
        internal_element_->selectItem(item_text);
    }

    void selectItem(const std::int32_t item_idx) const
    {
        internal_element_->selectItem(item_idx);
    }

    std::string getSelectedItem() const
    {
        return internal_element_->getSelectedItem();
    }

    std::int32_t getNumItems() const
    {
        return internal_element_->getNumItems();
    }

    std::int32_t getSelectedItemIndex() const
    {
        return internal_element_->getSelectedItemIndex();
    }
};

}  // namespace api_internal

class Button : public api_internal::Control
{
private:
    friend class GuiElement;
    Button(api_internal::InternalGuiElement* const internal_element) : api_internal::Control{internal_element} {}

public:
    Button() : api_internal::Control{} {}
};

class CheckBox : public api_internal::Control
{
private:
    friend class GuiElement;
    CheckBox(api_internal::InternalGuiElement* const internal_element) : api_internal::Control{internal_element} {}

public:
    CheckBox() : api_internal::Control{} {}

    bool isChecked() const
    {
        return internal_element_->isChecked();
    }

    void setChecked() const
    {
        internal_element_->setChecked();
    }

    void setUnChecked() const
    {
        internal_element_->setUnChecked();
    }
};

class RadioButton : public api_internal::Control
{
private:
    friend class GuiElement;
    RadioButton(api_internal::InternalGuiElement* const internal_element) : api_internal::Control{internal_element} {}

public:
    RadioButton() : api_internal::Control{} {}

    bool isChecked() const
    {
        return internal_element_->isChecked();
    }

    void setChecked() const
    {
        internal_element_->setChecked();
    }

    void setUnChecked() const
    {
        internal_element_->setUnChecked();
    }
};

class DropDownMenu : public api_internal::ListCommon
{
private:
    friend class GuiElement;
    DropDownMenu(api_internal::InternalGuiElement* const internal_element) : api_internal::ListCommon{internal_element}
    {
    }

public:
    DropDownMenu() : api_internal::ListCommon{} {}
};

class ListBox : public api_internal::ListCommon
{
private:
    friend class GuiElement;
    ListBox(api_internal::InternalGuiElement* const internal_element) : api_internal::ListCommon{internal_element} {}

public:
    ListBox() : api_internal::ListCommon{} {}
};

class EditableText : public api_internal::GuiElementBase
{
private:
    friend class GuiElement;
    EditableText(api_internal::InternalGuiElement* const internal_element)
        : api_internal::GuiElementBase{internal_element}
    {
    }

public:
    EditableText() : api_internal::GuiElementBase{} {}

    void setText(const std::string& new_text) const
    {
        return internal_element_->setText(new_text);
    }
    std::string getText() const
    {
        return internal_element_->getText();
    }

    bool enterPressed() const
    {
        return internal_element_->enterPressed();
    }
};

class TextLabel : public api_internal::Control
{
private:
    friend class GuiElement;
    TextLabel(api_internal::InternalGuiElement* const internal_element) : api_internal::Control{internal_element} {}

public:
    TextLabel() : api_internal::Control{} {}
};

class Slider : public api_internal::GuiElementBase
{
private:
    friend class GuiElement;
    Slider(api_internal::InternalGuiElement* const internal_element) : api_internal::GuiElementBase{internal_element} {}

public:
    Slider() : api_internal::GuiElementBase{} {}

    std::int32_t getValue() const
    {
        if (internal_element_)
        {
            return internal_element_->getValue();
        }
        else
        {
            DVS_LOG_ERROR() << "internal_element is nullptr for Slider! Returning -1.";
            return -1;
        }
    }

    void setMax(const std::int32_t new_max) const
    {
        if (internal_element_)
        {
            internal_element_->setMax(new_max);
        }
        else
        {
            DVS_LOG_ERROR() << "internal_element is nullptr for Slider!";
        }
    }
};

class GuiElement : public api_internal::GuiElementBase
{
public:
    GuiElement() : api_internal::GuiElementBase{} {}
    GuiElement(api_internal::InternalGuiElement* const internal_element)
        : api_internal::GuiElementBase{internal_element}
    {
    }

    Slider asSlider() const
    {
        if (internal_element_->getType() == GuiElementType::Slider)
        {
            return Slider{internal_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << internal_element_->getHandleString()
                            << " is not a slider! Returning empty object.";
            return Slider{};
        }
    }

    Button asButton() const
    {
        if (internal_element_->getType() == GuiElementType::Button)
        {
            return Button{internal_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << internal_element_->getHandleString()
                            << " is not a button! Returning empty object.";
            return Button{};
        }
    }

    CheckBox asCheckBox() const
    {
        if (internal_element_->getType() == GuiElementType::CheckBox)
        {
            return CheckBox{internal_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << internal_element_->getHandleString()
                            << " is not a checkbox! Returning empty object.";
            return CheckBox{};
        }
    }

    RadioButton asRadioButton() const
    {
        if (internal_element_->getType() == GuiElementType::RadioButton)
        {
            return RadioButton{internal_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << internal_element_->getHandleString()
                            << " is not a radiobutton! Returning empty object.";
            return RadioButton{};
        }
    }

    DropDownMenu asDropDownMenu() const
    {
        if (internal_element_->getType() == GuiElementType::DropDownMenu)
        {
            return DropDownMenu{internal_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << internal_element_->getHandleString()
                            << " is not a dropdownmenu! Returning empty object.";
            return DropDownMenu{};
        }
    }

    ListBox asListBox() const
    {
        if (internal_element_->getType() == GuiElementType::ListBox)
        {
            return ListBox{internal_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << internal_element_->getHandleString()
                            << " is not a listbox! Returning empty object.";
            return ListBox{};
        }
    }

    EditableText asEditableText() const
    {
        if (internal_element_->getType() == GuiElementType::EditableText)
        {
            return EditableText{internal_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << internal_element_->getHandleString()
                            << " is not an editabletext! Returning empty object.";
            return EditableText{};
        }
    }

    TextLabel asTextLabel() const
    {
        if (internal_element_->getType() == GuiElementType::TextLabel)
        {
            return TextLabel{internal_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << internal_element_->getHandleString()
                            << " is not a textlabel! Returning empty object.";
            return TextLabel{};
        }
    }

private:
};

namespace api_internal
{
inline void InternalGuiElement::callback()
{
    if (callback_function_)
    {
        callback_function_(GuiElement{this});
    }
    else
    {
        DVS_LOG_ERROR() << "No callback function set for GuiElement with handle string: " << getHandleString();
    }
}
}  // namespace api_internal

#endif  // MODULE_API_H
