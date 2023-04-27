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
    TextLabel
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

struct DropDownMenuData
{
    std::string selected_item;
    std::int16_t selected_item_index;

    DropDownMenuData() : selected_item{""}, selected_item_index{-1} {}
};

struct ListBoxData
{
    std::string selected_item;
    std::int32_t selected_item_index;

    ListBoxData() : selected_item{}, selected_item_index{} {}
};

struct EditableTextData
{
    std::string text_data;
    bool enter_pressed;

    EditableTextData() : text_data{""}, enter_pressed{false} {}
};

struct SliderData
{
    std::int32_t value;
    std::int32_t min_value;
    std::int32_t max_value;
    SliderData() : value{0}, min_value{0}, max_value{0} {}
};

class GuiElementEventData
{
public:
    GuiElementEventData(const VectorConstView<std::uint8_t>& gui_element_event_data_raw,
                        const GuiElementType& gui_element_type,
                        const std::string& origin_element_handle_string)
        : gui_element_event_data_raw_{gui_element_event_data_raw},
          gui_element_type_{gui_element_type},
          origin_element_handle_string_{origin_element_handle_string}
    {
    }

    DropDownMenuData getDropDownMenuData() const
    {
        if (gui_element_type_ != GuiElementType::DropDownMenu)
        {
            logErrorForWrongType(__func__);
            return DropDownMenuData{};
        }

        DropDownMenuData drop_down_menu_data;

        std::memcpy(
            &(drop_down_menu_data.selected_item_index), gui_element_event_data_raw_.data(), sizeof(std::int16_t));

        std::uint16_t num_characters;

        std::memcpy(
            &(num_characters), gui_element_event_data_raw_.data() + sizeof(std::int16_t), sizeof(std::uint16_t));

        if (num_characters == 0U)
        {
            drop_down_menu_data.selected_item = "";
        }
        else
        {
            drop_down_menu_data.selected_item.resize(num_characters);
            for (std::size_t k = 0; k < num_characters; k++)
            {
                drop_down_menu_data.selected_item[k] =
                    gui_element_event_data_raw_(k + sizeof(std::uint16_t) + sizeof(std::int16_t));
            }
        }

        return drop_down_menu_data;
    }

    ListBoxData getListBoxData() const
    {
        if (gui_element_type_ != GuiElementType::ListBox)
        {
            logErrorForWrongType(__func__);
            return ListBoxData{};
        }

        ListBoxData list_box_data;

        std::memcpy(&(list_box_data.selected_item_index), gui_element_event_data_raw_.data(), sizeof(std::int16_t));

        std::uint16_t num_characters;

        std::memcpy(
            &(num_characters), gui_element_event_data_raw_.data() + sizeof(std::int16_t), sizeof(std::uint16_t));

        if (num_characters == 0U)
        {
            list_box_data.selected_item = "";
        }
        else
        {
            list_box_data.selected_item.resize(num_characters);
            for (std::size_t k = 0; k < num_characters; k++)
            {
                list_box_data.selected_item[k] =
                    gui_element_event_data_raw_(k + sizeof(std::uint16_t) + sizeof(std::int16_t));
            }
        }

        return list_box_data;
    }

    EditableTextData getEditableTextData() const
    {
        if (gui_element_type_ != GuiElementType::EditableText)
        {
            logErrorForWrongType(__func__);
            return EditableTextData{};
        }

        std::uint16_t num_characters;

        std::memcpy(&(num_characters), gui_element_event_data_raw_.data() + 1U, sizeof(std::uint16_t));

        EditableTextData editable_text_data;
        if (num_characters == 0U)
        {
            editable_text_data.text_data = "";
        }
        else
        {
            editable_text_data.text_data.resize(num_characters);
            for (std::size_t k = 0; k < num_characters; k++)
            {
                editable_text_data.text_data[k] = gui_element_event_data_raw_(k + sizeof(std::uint16_t) + 1U);
            }
        }

        editable_text_data.enter_pressed = gui_element_event_data_raw_(0) == 1;

        return editable_text_data;
    }

    SliderData getSliderData() const
    {
        if (gui_element_type_ != GuiElementType::Slider)
        {
            logErrorForWrongType(__func__);
            return SliderData{};
        }

        SliderData slider_data;

        std::memcpy(&slider_data, gui_element_event_data_raw_.data(), sizeof(SliderData));

        return slider_data;
    }

    bool getCheckBoxData() const
    {
        if (gui_element_type_ != GuiElementType::CheckBox)
        {
            logErrorForWrongType(__func__);
            return false;
        }

        return gui_element_event_data_raw_(0) == 1;
    }

    bool getRadioButtonData() const
    {
        if (gui_element_type_ != GuiElementType::RadioButton)
        {
            logErrorForWrongType(__func__);
            return false;
        }

        return gui_element_event_data_raw_(0) == 1;
    }

private:
    VectorConstView<std::uint8_t> gui_element_event_data_raw_;
    GuiElementType gui_element_type_;
    std::string origin_element_handle_string_;

    void logErrorForWrongType(const std::string& function_name) const
    {
        DVS_LOG_ERROR() << "GuiElementEventData::" << function_name
                        << "() called on a data originating from the gui element \"" << origin_element_handle_string_
                        << "\", which is a " << guiElementTypeToString(gui_element_type_)
                        << "! Returning empty object.";
    }
};
class GuiElement;

using GuiElementCallback = std::function<void(GuiElement* const, const GuiElementEventData&)>;

class Slider
{
private:
public:
    Slider() {}

    virtual long getValue() const = 0;

    /*int getMin() const
    {
        return getMinFromSlider(id_);
    }

    int getMax() const
    {
        return getMaxFromSlider(id_);
    }

    void setValue(const int new_value)
    {
        setSliderValue(id_, new_value);
    }

    void setMin(const int new_min)
    {
        setSliderMin(id_, new_min);
    }*/

    virtual void setMax(const int new_max) = 0;
};

class GuiElement : public Slider
{
public:
    GuiElement(const std::string& handle_string, const GuiElementCallback& elem_callback, const GuiElementType type)
        : type_{type}, callback_function_{elem_callback}, handle_string_{handle_string}
    {
    }

    virtual ~GuiElement() = default;

    virtual long getId() const = 0;

    void callback(const VectorConstView<std::uint8_t>& event_raw_data)
    {
        const GuiElementEventData event_data{event_raw_data, type_, handle_string_};

        if (callback_function_)
        {
            callback_function_(this, event_data);
        }
        else
        {
            DVS_LOG_ERROR() << "No callback function set for GuiElement with handle string: " << getHandleString();
        }
    }

    GuiElementType getType() const
    {
        return type_;
    }

    std::string getHandleString() const
    {
        return handle_string_;
    }

    Slider* asSlider()
    {
        if (type_ == GuiElementType::Slider)
        {
            return static_cast<Slider*>(this);
        }
        else
        {
            DVS_LOG_ERROR() << "GuiElement with handle string: " << getHandleString()
                            << " is not a slider! Returning nullptr.";
            return nullptr;
        }
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

class LoadedDataBase
{
public:
    LoadedDataBase() = default;
};

class LoadedDataDerived : public LoadedDataBase
{
public:
    LoadedDataDerived() = default;
    LoadedDataDerived(const std::string& path_to_file) {}
};

/*class IGuiElement
{
protected:
    long id_;

public:
    IGuiElement() = default;

    void getHandleString() const
    {
        return gethandleString(id_);
    }

    long getId() const
    {
        return id_;
    }
};
*/

#endif  // MODULE_API_H
