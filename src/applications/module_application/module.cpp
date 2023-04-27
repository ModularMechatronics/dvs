#include "module_api.h"

/*
Put id in gui_element_event_data, and be able to retrieve it from there?.
Or the handle_string?
*/

extern "C" void registerCallbacks(std::map<std::string, GuiElementCallback>& callbacks)
{
    callbacks["button0"] = [](GuiElement* const source_gui_element,
                              const GuiElementEventData& gui_element_event_data) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["slider0"] = [](GuiElement* const source_gui_element,
                              const GuiElementEventData& gui_element_event_data) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        const SliderData slider_data = gui_element_event_data.getSliderData();

        Slider* const source_slider = source_gui_element->asSlider();

        std::cout << "Slider min: " << slider_data.min_value << std::endl;
        std::cout << "Slider max: " << slider_data.max_value << std::endl;
        std::cout << "Slider data: " << slider_data.value << std::endl;

        std::cout << "Slider source value: " << source_slider->getValue() << std::endl;
        if (source_slider->getValue() == 57)
        {
            source_slider->setMax(200);
        }
        else if (source_slider->getValue() == 32)
        {
            source_slider->setMax(100);
        }
    };

    callbacks["check_box0"] = [](GuiElement* const source_gui_element,
                                 const GuiElementEventData& gui_element_event_data) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["editable_text0"] = [](GuiElement* const source_gui_element,
                                     const GuiElementEventData& gui_element_event_data) -> void {
        static_cast<void>(gui_element_event_data);

        const EditableTextData editable_text_data = gui_element_event_data.getEditableTextData();
        std::cout << "User callback from " << source_gui_element->getHandleString()
                  << " with text data: " << editable_text_data.text_data << std::endl;
    };

    callbacks["drop_down_menu0"] = [](GuiElement* const source_gui_element,
                                      const GuiElementEventData& gui_element_event_data) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
        const DropDownMenuData ddmd = gui_element_event_data.getDropDownMenuData();
        std::cout << "Selected item index: " << ddmd.selected_item_index << std::endl;
        std::cout << "Selected item: " << ddmd.selected_item << std::endl;
    };

    callbacks["list_box0"] = [](GuiElement* const source_gui_element,
                                const GuiElementEventData& gui_element_event_data) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
        const ListBoxData lbd = gui_element_event_data.getListBoxData();
        std::cout << "Selected item index: " << lbd.selected_item_index << std::endl;
        std::cout << "Selected item: " << lbd.selected_item << std::endl;
    };

    callbacks["radio_button0x"] = [](GuiElement* const source_gui_element,
                                     const GuiElementEventData& gui_element_event_data) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };
}
