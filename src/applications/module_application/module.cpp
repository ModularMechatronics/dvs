#include "module_api.h"

void registerCallbacks(std::map<std::string, GuiElementCallback>& callbacks,
                       std::shared_ptr<LoadedDataBase>& loaded_data)
{
    callbacks["button0"] = [](const GuiElementEventData& gui_element_event_data) {
        // Do something, no data from button
    };

    callbacks["slider"] = [](const GuiElementEventData& gui_element_event_data) {
        const SliderData slider_data = gui_element_event_data.getSliderData();
        // Do something with slider_data
    };

    callbacks["check_box"] = [](const GuiElementEventData& gui_element_event_data) {
        const bool is_checked = gui_element_event_data.getCheckBoxData();
        // Do something
    };

    callbacks["radio_button"] = [](const GuiElementEventData& gui_element_event_data) {
        const bool is_checked = gui_element_event_data.getRadioButtonData();
        // Do something
    };

    callbacks["editable_text"] = [](const GuiElementEventData& gui_element_event_data) {
        const EditableTextData editable_text_data = gui_element_event_data.getEditableTextData();

        // Do something
    };

    callbacks["list_box_0"] = [](const GuiElementEventData& gui_element_event_data) {
        const ListBoxData list_box_data = gui_element_event_data.getListBoxData();

        // Do something
    };

    callbacks["drop_down_0"] = [](const GuiElementEventData& gui_element_event_data) {
        const DropDownMenuData drop_down_menu_data = gui_element_event_data.getDropDownMenuData();

        // Do something
    };
}
