#include "module_api.h"

std::int32_t selected_idx = 0;

void printUserCallbackString(GuiElement* const source_gui_element)
{
    std::cout << "User callback from " << source_gui_element->getHandleString() << std::endl;
}

extern "C" void registerCallbacks(std::map<std::string, GuiElementCallback>& callbacks,
                                  const std::function<GuiElement*(const std::string&)>& gui_element_getter)
{
    callbacks["button0"] = [&gui_element_getter](GuiElement* const source_gui_element,
                                                 const GuiElementEventData& gui_element_event_data) -> void {
        printUserCallbackString(source_gui_element);
        Button* const button = source_gui_element->asButton();

        DropDownMenu* const ddm = gui_element_getter("drop_down_menu0")->asDropDownMenu();
        ListBox* const lb = gui_element_getter("list_box0")->asListBox();
        ddm->addItem("New item");
        // ddm->removeItem("combo Test1");

        lb->addItem("New item lb");
        std::cout << "Selected item: " << lb->getSelectedItem() << std::endl;
        std::cout << "Selected index: " << lb->getSelectedItemIndex() << std::endl;

        // lb->removeItem("Test1");
        // lb->selectItem(selected_idx);
        // lb->selectItem(selected_idx);

        selected_idx++;

        // lb->clearItems();
        // ddm->clearItems();

        static_cast<void>(gui_element_event_data);
    };

    callbacks["slider0"] = [&gui_element_getter](GuiElement* const source_gui_element,
                                                 const GuiElementEventData& gui_element_event_data) -> void {
        printUserCallbackString(source_gui_element);

        Slider* const source_slider = source_gui_element->asSlider();
        const std::int32_t slider_value = source_slider->getValue();

        Button* const button = gui_element_getter("button0")->asButton();
        button->setPosition(slider_value, 0);

        EditableText* const et = gui_element_getter("editable_text0")->asEditableText();
        et->setSize(slider_value + 100, 40);

        et->setText(std::to_string(slider_value));

        TextLabel* const tl = gui_element_getter("text_label0")->asTextLabel();
        tl->setLabel(std::to_string(slider_value));

        std::cout << "Slider source value: " << source_slider->getValue() << std::endl;
        if (source_slider->getValue() == 57)
        {
            source_slider->setMax(200);
        }
        else if (source_slider->getValue() == 32)
        {
            source_slider->setMax(100);
        }

        if (source_slider->getValue() < 50)
        {
            CheckBox* const check_box = gui_element_getter("check_box0")->asCheckBox();
            button->setLabel("Not that much");
            check_box->setLabel("Not that much");
        }
        else
        {
            CheckBox* const check_box = gui_element_getter("check_box0")->asCheckBox();
            button->setLabel("A lot!!!!");
            check_box->setLabel("A lot!!!!");
        }
    };

    callbacks["check_box0"] = [&gui_element_getter](GuiElement* const source_gui_element,
                                                    const GuiElementEventData& gui_element_event_data) -> void {
        printUserCallbackString(source_gui_element);
        CheckBox* cb = source_gui_element->asCheckBox();
        const bool is_checked = cb->isChecked();
        std::cout << "Is checked: " << is_checked << std::endl;

        Slider* const slider = gui_element_getter("slider0")->asSlider();
        RadioButton* const rb = gui_element_getter("radio_button0x")->asRadioButton();

        if (is_checked)
        {
            slider->setMax(32);
            rb->setEnabled();
        }
        else
        {
            slider->setMax(57);
            rb->setDisabled();
        }
    };

    callbacks["editable_text0"] = [](GuiElement* const source_gui_element,
                                     const GuiElementEventData& gui_element_event_data) -> void {
        static_cast<void>(gui_element_event_data);
        EditableText* et = source_gui_element->asEditableText();

        const std::string editable_text_data = et->getText();
        printUserCallbackString(source_gui_element);
        std::cout << "Text data: " << editable_text_data << std::endl;

        const bool enter_pressed = et->enterPressed();
        if (enter_pressed)
        {
            std::cout << "Enter pressed" << std::endl;
        }
        else
        {
            std::cout << "Enter not pressed" << std::endl;
        }
    };

    callbacks["drop_down_menu0"] = [](GuiElement* const source_gui_element,
                                      const GuiElementEventData& gui_element_event_data) -> void {
        printUserCallbackString(source_gui_element);
        static_cast<void>(gui_element_event_data);

        DropDownMenu* ddm = source_gui_element->asDropDownMenu();

        std::cout << "Selected item index: " << ddm->getSelectedItemIndex() << std::endl;
        std::cout << "Selected item: " << ddm->getSelectedItem() << std::endl;
    };

    callbacks["list_box0"] = [](GuiElement* const source_gui_element,
                                const GuiElementEventData& gui_element_event_data) -> void {
        printUserCallbackString(source_gui_element);

        ListBox* lb = source_gui_element->asListBox();

        std::cout << "Selected item index: " << lb->getSelectedItemIndex() << std::endl;
        std::cout << "Selected item: " << lb->getSelectedItem() << std::endl;
    };

    callbacks["radio_button0x"] = [](GuiElement* const source_gui_element,
                                     const GuiElementEventData& gui_element_event_data) -> void {
        printUserCallbackString(source_gui_element);
        RadioButton* rb = source_gui_element->asRadioButton();
        const bool is_checked = rb->isChecked();
        std::cout << "Is checked: " << is_checked << std::endl;
    };
}
