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
    RadioButtonGroup,
    TextLabel,
    StaticBox,
    PlotPane,
    Unknown
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
        case GuiElementType::RadioButtonGroup:
            return "RadioButtonGroup";
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

    InternalGuiElement() = delete;

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
    virtual std::int16_t getSelectionIndex() const = 0;
    virtual std::string getSelectionString() const = 0;

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
    GuiElementBase() : gui_element_{nullptr} {}
    GuiElementBase(InternalGuiElement* const internal_element) : gui_element_{internal_element} {}

    GuiElementType getType() const
    {
        if (printErrorIfNotInitialized("GuiElement", __func__))
        {
            return gui_element_->getType();
        }
        else
        {
            return GuiElementType::Unknown;
        }
    }

    long getId() const
    {
        if (printErrorIfNotInitialized("GuiElement", __func__))
        {
            return gui_element_->getId();
        }
        else
        {
            return -1;
        }
    }

    std::string getHandleString() const
    {
        if (printErrorIfNotInitialized("GuiElement", __func__))
        {
            return gui_element_->getHandleString();
        }
        else
        {
            return "";
        }
    }

    void setEnabled() const
    {
        if (printErrorIfNotInitialized("GuiElement", __func__))
        {
            gui_element_->setEnabled();
        }
    }

    void setDisabled() const
    {
        if (printErrorIfNotInitialized("GuiElement", __func__))
        {
            gui_element_->setDisabled();
        }
    }

    void setPosition(const std::int16_t x, const std::int16_t y) const
    {
        if (printErrorIfNotInitialized("GuiElement", __func__))
        {
            gui_element_->setPosition(x, y);
        }
    }

    void setSize(const std::int16_t width, const std::int16_t height) const
    {
        if (printErrorIfNotInitialized("GuiElement", __func__))
        {
            gui_element_->setSize(width, height);
        }
    }

protected:
    InternalGuiElement* gui_element_;
    bool printErrorIfNotInitialized(const std::string& class_name, const std::string& function_name) const
    {
        if (gui_element_ == nullptr)
        {
            DVS_LOG_ERROR() << "Internal gui element pointer is nullptr for \e[1m\033[36m" << class_name
                            << "\033[0m\e[0m in function \e[1m\033[36m" << function_name
                            << "\033[0m!\e[0m Did you attempt to use a non existent element?";
            return false;
        }
        else
        {
            return true;
        }
    }
};

class Control : public GuiElementBase
{
private:
public:
    Control() : GuiElementBase{} {}
    Control(InternalGuiElement* const internal_element) : GuiElementBase{internal_element} {}

    void setLabel(const std::string& new_label) const
    {
        if (printErrorIfNotInitialized("Control", __func__))
        {
            gui_element_->setLabel(new_label);
        }
    }

    std::string getLabel() const
    {
        if (printErrorIfNotInitialized("Control", __func__))
        {
            return gui_element_->getLabel();
        }
        else
        {
            return "";
        }
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
        if (printErrorIfNotInitialized("ListCommon", __func__))
        {
            gui_element_->addItem(item_text);
        }
    }

    void removeItem(const std::string& item_text) const
    {
        if (printErrorIfNotInitialized("ListCommon", __func__))
        {
            gui_element_->removeItem(item_text);
        }
    }

    void clearItems() const
    {
        if (printErrorIfNotInitialized("ListCommon", __func__))
        {
            gui_element_->clearItems();
        }
    }

    void selectItem(const std::string& item_text) const
    {
        if (printErrorIfNotInitialized("ListCommon", __func__))
        {
            gui_element_->selectItem(item_text);
        }
    }

    void selectItem(const std::int32_t item_idx) const
    {
        if (printErrorIfNotInitialized("ListCommon", __func__))
        {
            gui_element_->selectItem(item_idx);
        }
    }

    std::string getSelectedItem() const
    {
        if (printErrorIfNotInitialized("ListCommon", __func__))
        {
            return gui_element_->getSelectedItem();
        }
        else
        {
            return "";
        }
    }

    std::int32_t getNumItems() const
    {
        if (printErrorIfNotInitialized("ListCommon", __func__))
        {
            return gui_element_->getNumItems();
        }
        else
        {
            return 0;
        }
    }

    std::int32_t getSelectedItemIndex() const
    {
        if (printErrorIfNotInitialized("ListCommon", __func__))
        {
            return gui_element_->getSelectedItemIndex();
        }
        else
        {
            return -1;
        }
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
        if (printErrorIfNotInitialized("CheckBox", __func__))
        {
            return gui_element_->isChecked();
        }
        else
        {
            return false;
        }
    }

    void setChecked() const
    {
        if (printErrorIfNotInitialized("CheckBox", __func__))
        {
            gui_element_->setChecked();
        }
    }

    void setUnChecked() const
    {
        if (printErrorIfNotInitialized("CheckBox", __func__))
        {
            gui_element_->setUnChecked();
        }
    }
};

class RadioButtonGroup : public api_internal::Control
{
private:
    friend class GuiElement;
    RadioButtonGroup(api_internal::InternalGuiElement* const internal_element) : api_internal::Control{internal_element} {}

public:
    RadioButtonGroup() : api_internal::Control{} {}

    std::int16_t getSelectionIndex() const
    {
        if (printErrorIfNotInitialized("RadioButtonGroup", __func__))
        {
            return gui_element_->getSelectionIndex();
        }
        else
        {
            return -1;
        }
    }

    std::string getSelectionString() const
    {
        if (printErrorIfNotInitialized("RadioButtonGroup", __func__))
        {
            return gui_element_->getSelectionString();
        }
        else
        {
            return "";
        }
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
        if (printErrorIfNotInitialized("EditableText", __func__))
        {
            return gui_element_->setText(new_text);
        }
    }
    std::string getText() const
    {
        if (printErrorIfNotInitialized("EditableText", __func__))
        {
            return gui_element_->getText();
        }
        else
        {
            return "";
        }
    }

    bool enterPressed() const
    {
        if (printErrorIfNotInitialized("EditableText", __func__))
        {
            return gui_element_->enterPressed();
        }
        else
        {
            return false;
        }
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
        if (printErrorIfNotInitialized("Slider", __func__))
        {
            return gui_element_->getValue();
        }
        else
        {
            return -1;
        }
    }

    std::int32_t getMin() const
    {
        if (printErrorIfNotInitialized("Slider", __func__))
        {
            return gui_element_->getMin();
        }
        else
        {
            return -1;
        }
    }

    std::int32_t getMax() const
    {
        if (printErrorIfNotInitialized("Slider", __func__))
        {
            return gui_element_->getMax();
        }
        else
        {
            return -1;
        }
    }

    void setValue(const std::int32_t new_value) const
    {
        if (printErrorIfNotInitialized("Slider", __func__))
        {
            gui_element_->setValue(new_value);
        }
    }

    void setMin(const std::int32_t new_min) const
    {
        if (printErrorIfNotInitialized("Slider", __func__))
        {
            gui_element_->setMin(new_min);
        }
    }

    void setMax(const std::int32_t new_max) const
    {
        if (printErrorIfNotInitialized("Slider", __func__))
        {
            gui_element_->setMax(new_max);
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
        if (gui_element_->getType() == GuiElementType::Slider)
        {
            return Slider{gui_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << gui_element_->getHandleString()
                            << " is not a slider! Returning empty object.";
            return Slider{};
        }
    }

    Button asButton() const
    {
        if (gui_element_->getType() == GuiElementType::Button)
        {
            return Button{gui_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << gui_element_->getHandleString()
                            << " is not a button! Returning empty object.";
            return Button{};
        }
    }

    CheckBox asCheckBox() const
    {
        if (gui_element_->getType() == GuiElementType::CheckBox)
        {
            return CheckBox{gui_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << gui_element_->getHandleString()
                            << " is not a checkbox! Returning empty object.";
            return CheckBox{};
        }
    }

    RadioButtonGroup asRadioButtonGroup() const
    {
        if (gui_element_->getType() == GuiElementType::RadioButtonGroup)
        {
            return RadioButtonGroup{gui_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << gui_element_->getHandleString()
                            << " is not a radiobuttongroup! Returning empty object.";
            return RadioButtonGroup{};
        }
    }

    DropDownMenu asDropDownMenu() const
    {
        if (gui_element_->getType() == GuiElementType::DropDownMenu)
        {
            return DropDownMenu{gui_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << gui_element_->getHandleString()
                            << " is not a dropdownmenu! Returning empty object.";
            return DropDownMenu{};
        }
    }

    ListBox asListBox() const
    {
        if (gui_element_->getType() == GuiElementType::ListBox)
        {
            return ListBox{gui_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << gui_element_->getHandleString()
                            << " is not a listbox! Returning empty object.";
            return ListBox{};
        }
    }

    EditableText asEditableText() const
    {
        if (gui_element_->getType() == GuiElementType::EditableText)
        {
            return EditableText{gui_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << gui_element_->getHandleString()
                            << " is not an editabletext! Returning empty object.";
            return EditableText{};
        }
    }

    TextLabel asTextLabel() const
    {
        if (gui_element_->getType() == GuiElementType::TextLabel)
        {
            return TextLabel{gui_element_};
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << gui_element_->getHandleString()
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
