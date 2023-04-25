#include "module_api.h"

/*
Put id in gui_element_event_data, and be able to retrieve it from there?.
Or the handle_string?
*/

void registerCallbacks(std::map<std::string, GuiElementCallback>& callbacks)
{
    callbacks["button0"] = [](const GuiElementEventData& gui_element_event_data,
                              const GuiElement* const source_gui_element) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["slider0"] = [](const GuiElementEventData& gui_element_event_data,
                              const GuiElement* const source_gui_element) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        const SliderData slider_data = gui_element_event_data.getSliderData();

        std::cout << "Slider min: " << slider_data.min_value << std::endl;
        std::cout << "Slider max: " << slider_data.max_value << std::endl;
        std::cout << "Slider data: " << slider_data.value << std::endl;

        // Slider* source_gui_element->as<Slider>();
    };

    callbacks["check_box0"] = [](const GuiElementEventData& gui_element_event_data,
                                 const GuiElement* const source_gui_element) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["editable_text0"] = [](const GuiElementEventData& gui_element_event_data,
                                     const GuiElement* const source_gui_element) -> void {
        static_cast<void>(gui_element_event_data);

        const EditableTextData editable_text_data = gui_element_event_data.getEditableTextData();
        std::cout << "User callback from " << source_gui_element->getHandleString()
                  << " with text data: " << editable_text_data.text_data << std::endl;
    };

    callbacks["drop_down_menu0"] = [](const GuiElementEventData& gui_element_event_data,
                                      const GuiElement* const source_gui_element) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["list_box0"] = [](const GuiElementEventData& gui_element_event_data,
                                const GuiElement* const source_gui_element) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["radio_button0x"] = [](const GuiElementEventData& gui_element_event_data,
                                     const GuiElement* const source_gui_element) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["text_label0"] = [](const GuiElementEventData& gui_element_event_data,
                                  const GuiElement* const source_gui_element) -> void {
        std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };
}
