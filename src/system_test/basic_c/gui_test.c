#include <stdio.h>

#include "basic_c/tests.h"
#include "duoplot/duoplot.h"

void slider0Callback(const SliderHandle gui_element_handle)
{
    const SliderHandle slider = getSliderHandle("slider0");

    printf("Callback function: \"%s\": %i\n", GET_HANDLE_STRING(gui_element_handle), getSliderValue(slider));

    const TextLabelHandle text_label = getTextLabelHandle("text_label0");

    char text[100];
    sprintf(text, "Slider value: %i\n", getSliderValue(slider));

    setTextLabelValue(text_label, text);
}

void button0Callback(const ButtonHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
}

void listBoxCallback(const ListBoxHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
    printf("Selected element: %s\n", getListBoxSelectedElement(gui_element_handle));

    for (size_t k = 0U; k < getListBoxElements(gui_element_handle).size; k++)
    {
        printf("Element %zu: %s\n", k, getListBoxElements(gui_element_handle).elements[k]);
    }
}

void dropDownMenuCallback(const DropdownMenuHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
    printf("Selected element: %s\n", getDropdownMenuSelectedElement(gui_element_handle));

    for (size_t k = 0U; k < getDropdownMenuElements(gui_element_handle).size; k++)
    {
        printf("Element %zu: %s\n", k, getDropdownMenuElements(gui_element_handle).elements[k]);
    }
}

void radioButtonsCallback(const RadioButtonGroupHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
    printf("Selected element idx: %i\n", getRadioButtonGroupSelectedButtonIdx(gui_element_handle));

    for (size_t k = 0U; k < getRadioButtonGroupButtonNames(gui_element_handle).size; k++)
    {
        printf("Element %zu: %s\n", k, getRadioButtonGroupButtonNames(gui_element_handle).elements[k]);
    }
}
void editableTextCallback(const EditableTextHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
    printf("Text: %s\n", getEditableTextValue(gui_element_handle));
}

void checkboxCallback(const CheckboxHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", GET_HANDLE_STRING(gui_element_handle));
    printf("Checked: %i\n", getIsCheckBoxChecked(gui_element_handle));
}

void printValues()
{
    const SliderHandle slider = getSliderHandle("slider0");
    const ListBoxHandle list_box = getListBoxHandle("listbox0");
    const DropdownMenuHandle drop_down_menu = getDropdownMenuHandle("ddm0");

    const TextLabelHandle text_label = getTextLabelHandle("text_label0");

    printf("Text label: %s\n", getTextLabelValue(text_label));

    printf("Listbox size: %zu\n", getListBoxElements(list_box).size);

    for (size_t k = 0U; k < getListBoxElements(list_box).size; k++)
    {
        printf("Element %zu: %s\n", k, getListBoxElements(list_box).elements[k]);
    }
    printf("Selected element: %s\n", getListBoxSelectedElement(list_box));

    printf("Slider value: %i\n", getSliderValue(slider));

    const RadioButtonGroupHandle rb_h = getRadioButtonGroupHandle("rbg0");
    printf("Selected element idx: %i\n", getRadioButtonGroupSelectedButtonIdx(rb_h));

    for (size_t k = 0U; k < getRadioButtonGroupButtonNames(rb_h).size; k++)
    {
        printf("Button %zu: %s\n", k, getRadioButtonGroupButtonNames(rb_h).elements[k]);
    }

    const EditableTextHandle et_h = getEditableTextHandle("text_entry");
    printf("Text: %s\n", getEditableTextValue(et_h));

    const CheckboxHandle cb_h = getCheckboxHandle("checkbox0");
    printf("Checked: %i\n", getIsCheckBoxChecked(cb_h));

    printf("DropdownMenu size: %zu\n", getDropdownMenuElements(drop_down_menu).size);

    for (size_t k = 0U; k < getDropdownMenuElements(drop_down_menu).size; k++)
    {
        printf("Element %zu: %s\n", k, getDropdownMenuElements(drop_down_menu).elements[k]);
    }

    printf("Selected element: %s\n", getDropdownMenuSelectedElement(drop_down_menu));
}

void testGUIBasic()
{
    startGuiReceiveThread();

    registerButtonCallback("button0", button0Callback);
    registerSliderCallback("slider0", slider0Callback);

    registerListBoxCallback("listbox0", listBoxCallback);
    registerDropdownMenuCallback("ddm0", dropDownMenuCallback);
    registerCheckboxCallback("checkbox0", checkboxCallback);
    registerRadioButtonGroupCallback("rbg0", radioButtonsCallback);
    registerEditableTextCallback("text_entry", editableTextCallback);

    char input_array[1000];

    while (true)
    {
        scanf("%s", input_array);

        if (strcmp(input_array, "q") == 0)
        {
            break;
        }
        else if (strcmp(input_array, "v") == 0)
        {
            printValues();
        }
    }
}
