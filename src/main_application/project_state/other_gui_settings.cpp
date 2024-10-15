#include "project_state/project_settings.h"

ButtonSettings::ButtonSettings() : ElementSettings{}, label{""}
{
    x = 0.0;
    y = 0.0;
    width = 0.4;
    height = 0.4;
    type = duoplot::GuiElementType::Button;
}

ButtonSettings::ButtonSettings(const nlohmann::json& j_data) : ElementSettings{j_data}, label{""}
{
    if (!j_data.contains("element_specific_settings"))
    {
        return;
    }

    const nlohmann::json j_ess = j_data["element_specific_settings"];
    label = j_ess["label"];
}

bool ButtonSettings::operator==(const ButtonSettings& other) const
{
    return ElementSettings::operator==(other) && label == other.label;
}

bool ButtonSettings::operator!=(const ButtonSettings& other) const
{
    return !(*this == other);
}

nlohmann::json ButtonSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();
    j["element_specific_settings"]["label"] = label;
    return j;
}

CheckboxSettings::CheckboxSettings() : ElementSettings{}, label{""}
{
    x = 0.0;
    y = 0.0;
    width = 0.4;
    height = 0.4;
    type = duoplot::GuiElementType::Checkbox;
}
CheckboxSettings::CheckboxSettings(const nlohmann::json& j_data) : ElementSettings{j_data}, label{""}
{
    if (!j_data.contains("element_specific_settings"))
    {
        return;
    }

    const nlohmann::json j_ess = j_data["element_specific_settings"];
    label = j_ess["label"];
}
bool CheckboxSettings::operator==(const CheckboxSettings& other) const
{
    return ElementSettings::operator==(other) && label == other.label;
}

bool CheckboxSettings::operator!=(const CheckboxSettings& other) const
{
    return !(*this == other);
}

nlohmann::json CheckboxSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();
    j["element_specific_settings"]["label"] = label;
    return j;
}

EditableTextSettings::EditableTextSettings() : ElementSettings{}, init_value{""}
{
    x = 0.0;
    y = 0.0;
    width = 0.4;
    height = 0.4;
    type = duoplot::GuiElementType::EditableText;
}
EditableTextSettings::EditableTextSettings(const nlohmann::json& j_data) : ElementSettings{j_data}, init_value{""}
{
    if (!j_data.contains("element_specific_settings"))
    {
        return;
    }

    const nlohmann::json j_ess = j_data["element_specific_settings"];
    init_value = j_ess["init_value"];
}

bool EditableTextSettings::operator==(const EditableTextSettings& other) const
{
    return ElementSettings::operator==(other) && init_value == other.init_value;
}

bool EditableTextSettings::operator!=(const EditableTextSettings& other) const
{
    return !(*this == other);
}

nlohmann::json EditableTextSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();
    j["element_specific_settings"]["init_value"] = init_value;
    return j;
}

DropdownMenuSettings::DropdownMenuSettings() : ElementSettings{}, initially_selected_item{""}, elements{}
{
    x = 0.0;
    y = 0.0;
    width = 0.4;
    height = 0.4;
    type = duoplot::GuiElementType::DropdownMenu;
}
DropdownMenuSettings::DropdownMenuSettings(const nlohmann::json& j_data)
    : ElementSettings{j_data}, initially_selected_item{""}, elements{}
{
    if (!j_data.contains("element_specific_settings"))
    {
        return;
    }

    const nlohmann::json j_ess = j_data["element_specific_settings"];

    if (j_ess.contains("elements"))
    {
        for (const std::string elem : j_ess["elements"])
        {
            if (elem != "")
            {
                elements.push_back(elem);
            }
        }
    }

    if (j_ess.contains("initially_selected_item"))
    {
        initially_selected_item = j_ess["initially_selected_item"];
    }
    else
    {
        initially_selected_item = "";
    }

    if (std::find(elements.begin(), elements.end(), initially_selected_item) == elements.end())
    {
        initially_selected_item = "";
    }
}

bool DropdownMenuSettings::operator==(const DropdownMenuSettings& other) const
{
    return ElementSettings::operator==(other) && initially_selected_item == other.initially_selected_item;
}

bool DropdownMenuSettings::operator!=(const DropdownMenuSettings& other) const
{
    return !(*this == other);
}

nlohmann::json DropdownMenuSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();
    j["element_specific_settings"]["initially_selected_item"] = initially_selected_item;
    j["element_specific_settings"]["elements"] = elements;
    return j;
}

ListBoxSettings::ListBoxSettings() : ElementSettings{}, elements{}
{
    x = 0.0;
    y = 0.0;
    width = 0.4;
    height = 0.4;
    type = duoplot::GuiElementType::ListBox;
}
ListBoxSettings::ListBoxSettings(const nlohmann::json& j_data) : ElementSettings{j_data}, elements{}
{
    if (!j_data.contains("element_specific_settings"))
    {
        return;
    }

    const nlohmann::json j_ess = j_data["element_specific_settings"];

    if (j_ess.contains("elements"))
    {
        for (const std::string elem : j_ess["elements"])
        {
            elements.push_back(elem);
        }
    }
}

bool ListBoxSettings::operator==(const ListBoxSettings& other) const
{
    return ElementSettings::operator==(other);
}

bool ListBoxSettings::operator!=(const ListBoxSettings& other) const
{
    return !(*this == other);
}

nlohmann::json ListBoxSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();
    j["element_specific_settings"]["elements"] = elements;
    return j;
}

RadioButtonSettings::RadioButtonSettings() : label{""} {}
RadioButtonSettings::RadioButtonSettings(const nlohmann::json& j_data)
{
    label = j_data["label"];
}

bool RadioButtonSettings::operator==(const RadioButtonSettings& other) const
{
    return label == other.label;
}

bool RadioButtonSettings::operator!=(const RadioButtonSettings& other) const
{
    return !(*this == other);
}

nlohmann::json RadioButtonSettings::toJson() const
{
    nlohmann::json j;
    j["label"] = label;
    return j;
}

RadioButtonGroupSettings::RadioButtonGroupSettings() : ElementSettings{}, label{""} {}
RadioButtonGroupSettings::RadioButtonGroupSettings(const nlohmann::json& j_data)
    : ElementSettings{j_data}, label{""}, radio_buttons{}
{
    if (!j_data.contains("element_specific_settings"))
    {
        return;
    }

    const nlohmann::json j_ess = j_data["element_specific_settings"];

    label = j_ess["label"];

    for (const nlohmann::json& radio_button : j_ess["elements"])
    {
        radio_buttons.push_back(RadioButtonSettings{radio_button});
    }
}

bool RadioButtonGroupSettings::operator==(const RadioButtonGroupSettings& other) const
{
    bool all_equal = true;
    for (size_t i = 0; i < radio_buttons.size(); ++i)
    {
        all_equal = all_equal && radio_buttons[i] == other.radio_buttons[i];
    }
    return ElementSettings::operator==(other) && label == other.label && all_equal;
}

bool RadioButtonGroupSettings::operator!=(const RadioButtonGroupSettings& other) const
{
    return !(*this == other);
}

nlohmann::json RadioButtonGroupSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();
    j["element_specific_settings"]["label"] = label;
    for (const RadioButtonSettings& radio_button : radio_buttons)
    {
        j["element_specific_settings"]["elements"].push_back(radio_button.toJson());
    }
    return j;
}

TextLabelSettings::TextLabelSettings() : ElementSettings{}, label{""}
{
    x = 0.0;
    y = 0.0;
    width = 0.4;
    height = 0.4;
    type = duoplot::GuiElementType::TextLabel;
}
TextLabelSettings::TextLabelSettings(const nlohmann::json& j_data) : ElementSettings{j_data}, label{""}
{
    if (!j_data.contains("element_specific_settings"))
    {
        return;
    }

    const nlohmann::json j_ess = j_data["element_specific_settings"];
    label = j_ess["label"];
}

bool TextLabelSettings::operator==(const TextLabelSettings& other) const
{
    return ElementSettings::operator==(other) && label == other.label;
}

bool TextLabelSettings::operator!=(const TextLabelSettings& other) const
{
    return !(*this == other);
}

nlohmann::json TextLabelSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();
    j["element_specific_settings"]["label"] = label;
    return j;
}

SliderSettings::SliderSettings()
    : ElementSettings{}, min_value{0}, max_value{100}, init_value{0}, step_size{1}, is_horizontal{true}
{
    x = 0.0;
    y = 0.0;
    width = 0.4;
    height = 0.4;
    type = duoplot::GuiElementType::Slider;
}

SliderSettings::SliderSettings(const nlohmann::json& j_data) : ElementSettings{j_data}
{
    if (j_data.contains("element_specific_settings"))
    {
        const nlohmann::json j_ess = j_data["element_specific_settings"];

        min_value = getOptionalValue(j_ess, "min", 0);
        max_value = getOptionalValue(j_ess, "max", 100);
        init_value = getOptionalValue(j_ess, "init_value", 0);
        step_size = getOptionalValue(j_ess, "step_size", 1);

        is_horizontal = true;

        // TODO: Only support horizontal sliders for now
        /*if (j_ess.contains("style"))
        {
            if (j_ess["style"] == "vertical")
            {
                is_horizontal = false;
            }
            else if (j_ess["style"] == "horizontal")
            {
                is_horizontal = true;
            }
            else
            {
                is_horizontal = true;
            }
        }
        else
        {
            is_horizontal = true;
        }*/
    }
    else
    {
        min_value = 0;
        max_value = 100;
        init_value = 0;
        step_size = 1;
        is_horizontal = true;
    }
}

bool SliderSettings::operator==(const SliderSettings& other) const
{
    return ElementSettings::operator==(other) && min_value == other.min_value && max_value == other.max_value &&
           init_value == other.init_value && step_size == other.step_size && is_horizontal == other.is_horizontal;
}

bool SliderSettings::operator!=(const SliderSettings& other) const
{
    return !(*this == other);
}

nlohmann::json SliderSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();
    j["element_specific_settings"]["min"] = min_value;
    j["element_specific_settings"]["max"] = max_value;
    j["element_specific_settings"]["init_value"] = init_value;
    j["element_specific_settings"]["step_size"] = step_size;
    // j["element_specific_settings"]["style"] = is_horizontal ? "horizontal" : "vertical";
    return j;
}

// std::string label;
// std::vector<std::shared_ptr<ElementSettings>> elements;

StaticBoxSettings::StaticBoxSettings() : label{""}, elements{}
{
    x = 0.0;
    y = 0.0;
    width = 0.4;
    height = 0.4;
    type = duoplot::GuiElementType::StaticBox;
}

StaticBoxSettings::StaticBoxSettings(const nlohmann::json& j_data) : ElementSettings{j_data}, label{""}, elements{}
{
    if (j_data.contains("element_specific_settings"))
    {
        const nlohmann::json j_ess = j_data["element_specific_settings"];

        label = j_ess["label"];

        for (const nlohmann::json& j_element : j_ess["elements"])
        {
            const duoplot::GuiElementType ge_type = parseGuiElementType(j_element);

            if (ge_type == duoplot::GuiElementType::Button)
            {
                elements.push_back(std::make_shared<ButtonSettings>(j_element));
            }
            else if (ge_type == duoplot::GuiElementType::Checkbox)
            {
                elements.push_back(std::make_shared<CheckboxSettings>(j_element));
            }
            else if (ge_type == duoplot::GuiElementType::EditableText)
            {
                elements.push_back(std::make_shared<EditableTextSettings>(j_element));
            }
            else if (ge_type == duoplot::GuiElementType::DropdownMenu)
            {
                elements.push_back(std::make_shared<DropdownMenuSettings>(j_element));
            }
            else if (ge_type == duoplot::GuiElementType::ListBox)
            {
                elements.push_back(std::make_shared<ListBoxSettings>(j_element));
            }
            else if (ge_type == duoplot::GuiElementType::RadioButtonGroup)
            {
                elements.push_back(std::make_shared<RadioButtonGroupSettings>(j_element));
            }
            else if (ge_type == duoplot::GuiElementType::ScrollingText)
            {
                elements.push_back(std::make_shared<ScrollingTextSettings>(j_element));
            }
            else if (ge_type == duoplot::GuiElementType::TextLabel)
            {
                elements.push_back(std::make_shared<TextLabelSettings>(j_element));
            }
            else if (ge_type == duoplot::GuiElementType::Slider)
            {
                elements.push_back(std::make_shared<SliderSettings>(j_element));
            }
            else if (ge_type == duoplot::GuiElementType::StaticBox)
            {
                elements.push_back(std::make_shared<StaticBoxSettings>(j_element));
            }
        }
    }
}

nlohmann::json StaticBoxSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();
    for (const std::shared_ptr<ElementSettings>& element : elements)
    {
        j["element_specific_settings"]["elements"].push_back(element->toJson());
    }

    return j;
}

bool StaticBoxSettings::operator==(const StaticBoxSettings& other) const
{
    bool all_equal = true;
    for (size_t i = 0; i < elements.size(); ++i)
    {
        const std::shared_ptr<ElementSettings> current_element = elements[i];
        const auto q = std::find_if(other.elements.begin(),
                                    other.elements.end(),
                                    [&current_element](const std::shared_ptr<ElementSettings>& other_element) -> bool {
                                        return areDerivedElementEqual(current_element, other_element);
                                    });

        all_equal = all_equal && q != elements.end();
    }

    return ElementSettings::operator==(other) && label == other.label && all_equal;
}
bool StaticBoxSettings::operator!=(const StaticBoxSettings& other) const
{
    return !(*this == other);
}
