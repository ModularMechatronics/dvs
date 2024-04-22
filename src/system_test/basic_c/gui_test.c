#include <stdio.h>

#include "basic_c/tests.h"
#include "duoplot/duoplot.h"

void slider0Callback(const duoplot_SliderHandle gui_element_handle)
{
    const duoplot_SliderHandle slider = duoplot_getSliderHandle("slider0");

    printf("Callback function: \"%s\": %i\n",
           DUOPLOT_GET_HANDLE_STRING(gui_element_handle),
           duoplot_getSliderValue(slider));

    const duoplot_TextLabelHandle text_label = duoplot_getTextLabelHandle("text_label0");

    char text[100];
    sprintf(text, "Slider value: %i\n", duoplot_getSliderValue(slider));

    duoplot_setTextLabelValue(text_label, text);
}

void button0Callback(const duoplot_ButtonHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", DUOPLOT_GET_HANDLE_STRING(gui_element_handle));
}

void listBoxCallback(const duoplot_ListBoxHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", DUOPLOT_GET_HANDLE_STRING(gui_element_handle));
    printf("Selected element: %s\n", duoplot_getListBoxSelectedElement(gui_element_handle));

    for (size_t k = 0U; k < duoplot_getListBoxElements(gui_element_handle).size; k++)
    {
        printf("Element %zu: %s\n", k, duoplot_getListBoxElements(gui_element_handle).elements[k]);
    }
}

void dropDownMenuCallback(const duoplot_DropdownMenuHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", DUOPLOT_GET_HANDLE_STRING(gui_element_handle));
    printf("Selected element: %s\n", duoplot_getDropdownMenuSelectedElement(gui_element_handle));

    for (size_t k = 0U; k < duoplot_getDropdownMenuElements(gui_element_handle).size; k++)
    {
        printf("Element %zu: %s\n", k, duoplot_getDropdownMenuElements(gui_element_handle).elements[k]);
    }
}

void radioButtonsCallback(const duoplot_RadioButtonGroupHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", DUOPLOT_GET_HANDLE_STRING(gui_element_handle));
    printf("Selected element idx: %i\n", duoplot_getRadioButtonGroupSelectedButtonIdx(gui_element_handle));

    for (size_t k = 0U; k < duoplot_getRadioButtonGroupButtonNames(gui_element_handle).size; k++)
    {
        printf("Element %zu: %s\n", k, duoplot_getRadioButtonGroupButtonNames(gui_element_handle).elements[k]);
    }
}
void editableTextCallback(const duoplot_EditableTextHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", DUOPLOT_GET_HANDLE_STRING(gui_element_handle));
    printf("Text: %s\n", duoplot_getEditableTextValue(gui_element_handle));
}

void checkboxCallback(const duoplot_CheckboxHandle gui_element_handle)
{
    printf("Callback function: \"%s\"\n", DUOPLOT_GET_HANDLE_STRING(gui_element_handle));
    printf("Checked: %i\n", duoplot_getIsCheckBoxChecked(gui_element_handle));
}

void printValues()
{
    const duoplot_SliderHandle slider = duoplot_getSliderHandle("slider0");
    const duoplot_ListBoxHandle list_box = duoplot_getListBoxHandle("listbox0");
    const duoplot_DropdownMenuHandle drop_down_menu = duoplot_getDropdownMenuHandle("ddm0");

    const duoplot_TextLabelHandle text_label = duoplot_getTextLabelHandle("text_label0");

    printf("Text label: %s\n", duoplot_getTextLabelValue(text_label));

    printf("Listbox size: %zu\n", duoplot_getListBoxElements(list_box).size);

    for (size_t k = 0U; k < duoplot_getListBoxElements(list_box).size; k++)
    {
        printf("Element %zu: %s\n", k, duoplot_getListBoxElements(list_box).elements[k]);
    }
    printf("Selected element: %s\n", duoplot_getListBoxSelectedElement(list_box));

    printf("Slider value: %i\n", duoplot_getSliderValue(slider));

    const duoplot_RadioButtonGroupHandle rb_h = duoplot_getRadioButtonGroupHandle("rbg0");
    printf("Selected element idx: %i\n", duoplot_getRadioButtonGroupSelectedButtonIdx(rb_h));

    for (size_t k = 0U; k < duoplot_getRadioButtonGroupButtonNames(rb_h).size; k++)
    {
        printf("Button %zu: %s\n", k, duoplot_getRadioButtonGroupButtonNames(rb_h).elements[k]);
    }

    const duoplot_EditableTextHandle et_h = duoplot_getEditableTextHandle("text_entry");
    printf("Text: %s\n", duoplot_getEditableTextValue(et_h));

    const duoplot_CheckboxHandle cb_h = duoplot_getCheckboxHandle("checkbox0");
    printf("Checked: %i\n", duoplot_getIsCheckBoxChecked(cb_h));

    printf("DropdownMenu size: %zu\n", duoplot_getDropdownMenuElements(drop_down_menu).size);

    for (size_t k = 0U; k < duoplot_getDropdownMenuElements(drop_down_menu).size; k++)
    {
        printf("Element %zu: %s\n", k, duoplot_getDropdownMenuElements(drop_down_menu).elements[k]);
    }

    printf("Selected element: %s\n", duoplot_getDropdownMenuSelectedElement(drop_down_menu));
}

void testGUIBasic()
{
    duoplot_startGuiReceiveThread();

    duoplot_registerButtonCallback("button0", button0Callback);
    duoplot_registerSliderCallback("slider0", slider0Callback);

    duoplot_registerListBoxCallback("listbox0", listBoxCallback);
    duoplot_registerDropdownMenuCallback("ddm0", dropDownMenuCallback);
    duoplot_registerCheckboxCallback("checkbox0", checkboxCallback);
    duoplot_registerRadioButtonGroupCallback("rbg0", radioButtonsCallback);
    duoplot_registerEditableTextCallback("text_entry", editableTextCallback);

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
