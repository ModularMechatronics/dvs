#include "module_api.h"

void printUserCallbackString(const GuiElement& source_gui_element)
{
    std::cout << "User callback from " << source_gui_element.getHandleString() << std::endl;
}

struct TimerCallbackS
{
    std::int32_t timer_id;
    std::function<void(std::int32_t)> callback;
};

extern "C" void registerCallbacks(std::map<std::string, GuiElementCallback>& gui_element_callbacks,
                                  std::map<std::string, TimerCallback>& timer_callbacks,
                                  const std::function<GuiElement(const std::string&)>& gui_element_getter,
                                  const std::function<Timer(const std::string&)>& timer_getter)
{

    timer_callbacks["timer0"] = [](const Timer& source_timer) -> void {
        std::cout << "Timer callback from FAST timer: " <<  source_timer.getIterationNumber() << std::endl;
    };

    timer_callbacks["timer1"] = [&timer_getter](const Timer& source_timer) -> void {
        const Timer timer0 = timer_getter("timer0");
        std::cout << "Timer callback from SLOW timer. timer0 progress: " << timer0.getIterationNumber() << std::endl;
    };

    gui_element_callbacks["button1"] = [&gui_element_getter, &timer_getter](const GuiElement& source_gui_element) -> void {
        // Stop timer
        timer_getter("timer0").stop();
        // timer_getter("timer1").stop();
    };

    gui_element_callbacks["button0"] = [&gui_element_getter, &timer_getter](const GuiElement& source_gui_element) -> void {
        printUserCallbackString(source_gui_element);
        const Button source_button = source_gui_element.asButton();
        const DropDownMenu ddm = gui_element_getter("drop_down_menu0").asDropDownMenu();
        const ListBox lb = gui_element_getter("list_box0").asListBox();
        ddm.addItem("New item");
        lb.addItem("New item lb");

        timer_getter("timer0").start(100);
        timer_getter("timer1").start(500);
    };

    gui_element_callbacks["slider0"] = [&gui_element_getter](const GuiElement& source_gui_element) -> void {
        printUserCallbackString(source_gui_element);
        Slider source_slider = source_gui_element.asSlider();
        const std::int32_t slider_value = source_slider.getValue();
        std::cout << "Slider value: " << slider_value << std::endl;

        if (slider_value == 57)
        {
            source_slider.setMax(200);
        }
        else if (slider_value == 32)
        {
            source_slider.setMax(100);
        }

        const Button button = gui_element_getter("button0").asButton();
        button.setPosition(slider_value, 0);

        const EditableText et = gui_element_getter("editable_text0").asEditableText();
        et.setSize(slider_value + 100, 40);

        et.setText(std::to_string(slider_value));

        const TextLabel tl = gui_element_getter("text_label0").asTextLabel();
        tl.setLabel(std::to_string(slider_value));

        const CheckBox check_box = gui_element_getter("check_box0").asCheckBox();

        std::cout << "Slider source value: " << source_slider.getValue() << std::endl;
        if (source_slider.getValue() == 57)
        {
            source_slider.setMax(200);
        }
        else if (source_slider.getValue() == 32)
        {
            source_slider.setMax(100);
        }

        if (source_slider.getValue() < 50)
        {
            button.setLabel("Not that much");
            check_box.setLabel("Not that much");
        }
        else
        {
            button.setLabel("A lot!!!!");
            check_box.setLabel("A lot!!!!");
        }
    };

    gui_element_callbacks["check_box0"] = [&gui_element_getter](const GuiElement& source_gui_element) -> void {
        printUserCallbackString(source_gui_element);

        const CheckBox cb = source_gui_element.asCheckBox();
        const bool is_checked = cb.isChecked();
        std::cout << "Is checked: " << is_checked << std::endl;

        const Slider slider = gui_element_getter("slider0").asSlider();
        const RadioButtonGroup rbg = gui_element_getter("radio_button_group0").asRadioButtonGroup();

        if (is_checked)
        {
            slider.setMax(32);
            rbg.setEnabled();
        }
        else
        {
            slider.setMax(57);
            rbg.setDisabled();
        }
    };

    gui_element_callbacks["editable_text0"] = [&gui_element_getter](const GuiElement& source_gui_element) -> void {
        printUserCallbackString(source_gui_element);
        const EditableText et = source_gui_element.asEditableText();

        const std::string editable_text_data = et.getText();
        std::cout << "Text data: " << editable_text_data << std::endl;

        const bool enter_pressed = et.enterPressed();
        if (enter_pressed)
        {
            std::cout << "Enter pressed" << std::endl;
        }
        else
        {
            std::cout << "Enter not pressed" << std::endl;
        }
    };

    gui_element_callbacks["drop_down_menu0"] = [&gui_element_getter](const GuiElement& source_gui_element) -> void {
        printUserCallbackString(source_gui_element);

        const DropDownMenu ddm = source_gui_element.asDropDownMenu();

        std::cout << "Selected item index: " << ddm.getSelectedItemIndex() << std::endl;
        std::cout << "Selected item: " << ddm.getSelectedItem() << std::endl;
    };

    gui_element_callbacks["list_box0"] = [&gui_element_getter](const GuiElement& source_gui_element) -> void {
        printUserCallbackString(source_gui_element);

        const ListBox lb = source_gui_element.asListBox();

        std::cout << "Selected item index: " << lb.getSelectedItemIndex() << std::endl;
        std::cout << "Selected item: " << lb.getSelectedItem() << std::endl;
    };

    gui_element_callbacks["radio_button_group0"] = [&gui_element_getter](const GuiElement& source_gui_element) -> void {
        printUserCallbackString(source_gui_element);
        const RadioButtonGroup rbg = source_gui_element.asRadioButtonGroup();
        std::cout << "Idx: " << rbg.getSelectionIndex() << ", "  << rbg.getSelectionString() <<std::endl;
    };
}
