#include "main_window.h"

#include <unistd.h>
#include <wx/wfstream.h>
#include <wx/wxprec.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

std::tuple<wxPoint, wxSize> getPosAndSizeInPixelCoords(const wxSize current_window_size,
                                                       const ElementAttributes* const element_attributes)
{
    const float x_pos_in_pixels = element_attributes->x * current_window_size.GetWidth();
    const float y_pos_in_pixels = element_attributes->y * current_window_size.GetHeight();

    const float width_in_pixels = element_attributes->width * current_window_size.GetWidth();
    const float height_in_pixels = element_attributes->height * current_window_size.GetHeight();

    return std::make_tuple(wxPoint(x_pos_in_pixels, y_pos_in_pixels), wxSize(width_in_pixels, height_in_pixels));
}

void MainWindow::setupButton(const ButtonAttributes& button_attributes, const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &button_attributes);

    MovableElement<wxButton>* btn = new MovableElement<wxButton>(this,
                                                                 button_attributes.handle_string,
                                                                 elem_callback,
                                                                 GuiElementType::Button,
                                                                 wxID_ANY,
                                                                 button_attributes.label,
                                                                 elem_pos,
                                                                 elem_size);

    gui_elements_[btn->getId()] = static_cast<GuiElement*>(btn);
    Bind(wxEVT_BUTTON, &MainWindow::buttonCallback, this, btn->getId());
}

void MainWindow::setupCheckBox(const CheckBoxAttributes& check_box_attributes, const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &check_box_attributes);

    MovableElement<wxCheckBox>* cb = new MovableElement<wxCheckBox>(this,
                                                                    check_box_attributes.handle_string,
                                                                    elem_callback,
                                                                    GuiElementType::CheckBox,
                                                                    wxID_ANY,
                                                                    check_box_attributes.label,
                                                                    elem_pos,
                                                                    elem_size);

    gui_elements_[cb->getId()] = static_cast<GuiElement*>(cb);
    Bind(wxEVT_CHECKBOX, &MainWindow::checkBoxCallback, this, cb->getId());
}

void MainWindow::setupSlider(const SliderAttributes& slider_attributes, const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &slider_attributes);

    const long style = slider_attributes.is_horizontal ? wxSL_HORIZONTAL : wxSL_VERTICAL;

    MovableElement<wxSlider>* slider = new MovableElement<wxSlider>(this,
                                                                    slider_attributes.handle_string,
                                                                    elem_callback,
                                                                    GuiElementType::Slider,
                                                                    wxID_ANY,
                                                                    slider_attributes.init_value,
                                                                    slider_attributes.min_value,
                                                                    slider_attributes.max_value,
                                                                    elem_pos,
                                                                    elem_size,
                                                                    style);

    gui_elements_[slider->getId()] = static_cast<GuiElement*>(slider);
    Bind(wxEVT_SLIDER, &MainWindow::sliderCallback, this, slider->getId());
}

void MainWindow::setupEditableText(const EditableTextAttributes& editable_text_attributes,
                                   const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &editable_text_attributes);

    MovableElement<wxTextCtrl>* editable_text = new MovableElement<wxTextCtrl>(this,
                                                                               editable_text_attributes.handle_string,
                                                                               elem_callback,
                                                                               GuiElementType::EditableText,
                                                                               wxID_ANY,
                                                                               editable_text_attributes.init_value,
                                                                               elem_pos,
                                                                               elem_size,
                                                                               wxTE_PROCESS_ENTER);
    gui_elements_[editable_text->getId()] = static_cast<GuiElement*>(editable_text);
    Bind(wxEVT_TEXT, &MainWindow::textCtrlCallback, this, editable_text->getId());
    Bind(wxEVT_TEXT_ENTER, &MainWindow::textCtrlEnterCallback, this, editable_text->getId());
}

void MainWindow::setupDropDownMenu(const DropDownMenuAttributes& drop_down_menu_attributes,
                                   const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &drop_down_menu_attributes);

    MovableElement<wxComboBox>* drop_down_menu = new MovableElement<wxComboBox>(this,
                                                                                drop_down_menu_attributes.handle_string,
                                                                                elem_callback,
                                                                                GuiElementType::DropDownMenu,
                                                                                wxID_ANY,
                                                                                drop_down_menu_attributes.init_value,
                                                                                elem_pos,
                                                                                elem_size);
    gui_elements_[drop_down_menu->getId()] = static_cast<GuiElement*>(drop_down_menu);
    Bind(wxEVT_COMBOBOX, &MainWindow::comboBoxCallback, this, drop_down_menu->getId());
    return;
}

void MainWindow::setupListBox(const ListBoxAttributes& list_box_attributes, const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &list_box_attributes);

    MovableElement<wxListBox>* list_box = new MovableElement<wxListBox>(
        this, list_box_attributes.handle_string, elem_callback, GuiElementType::ListBox, wxID_ANY, elem_pos, elem_size);
    gui_elements_[list_box->getId()] = static_cast<GuiElement*>(list_box);
    Bind(wxEVT_LISTBOX, &MainWindow::listBoxCallback, this, list_box->getId());
}

void MainWindow::setupStaticText(const StaticTextAttributes& static_text_attributes,
                                 const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &static_text_attributes);

    MovableElement<wxStaticText>* static_text = new MovableElement<wxStaticText>(this,
                                                                                 static_text_attributes.handle_string,
                                                                                 elem_callback,
                                                                                 GuiElementType::TextLabel,
                                                                                 wxID_ANY,
                                                                                 static_text_attributes.label,
                                                                                 elem_pos,
                                                                                 elem_size);
    gui_elements_[static_text->getId()] = static_cast<GuiElement*>(static_text);
}

void MainWindow::setupRadioButton(const RadioButtonAttributes& radio_button_attributes,
                                  const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &radio_button_attributes);

    MovableElement<wxRadioButton>* radio_button =
        new MovableElement<wxRadioButton>(this,
                                          radio_button_attributes.handle_string,
                                          elem_callback,
                                          GuiElementType::RadioButton,
                                          wxID_ANY,
                                          radio_button_attributes.label,
                                          elem_pos,
                                          elem_size);
    gui_elements_[radio_button->getId()] = static_cast<GuiElement*>(radio_button);
    Bind(wxEVT_RADIOBUTTON, &MainWindow::radioButtonCallback, this, radio_button->getId());
}

void MainWindow::createGuiElements(const std::string& path_to_layout_file)
{
    std::ifstream input_file(path_to_layout_file);
    nlohmann::json json_data;
    input_file >> json_data;

    // TODO: No validation is possible to make sure that the assigned functions in "registerCallbacks" are
    // available from the json layout definition

    std::map<std::string, GuiElementCallback> callbacks;

    dynamic_module_.registerCallbacks(callbacks);

    for (size_t k = 0; k < json_data["windows"].size(); k++)
    {
        const nlohmann::json& wnd = json_data["windows"][k];

        for (size_t i = 0; i < wnd["tabs"].size(); i++)
        {
            const nlohmann::json& tb = wnd["tabs"][i];

            for (size_t j = 0; j < tb["elements"].size(); j++)
            {
                const nlohmann::json& elem = tb["elements"][j];
                const std::string element_type = elem["type"];

                const std::string handle_string = elem["handle_string"];

                GuiElementCallback elem_callback{nullptr};
                if (callbacks.find(handle_string) != callbacks.end())
                {
                    elem_callback = callbacks[handle_string];
                }

                GuiElement* gui_element{nullptr};

                if ("BUTTON" == element_type)
                {
                    setupButton(ButtonAttributes{elem}, elem_callback);
                }
                else if ("SLIDER" == element_type)
                {
                    setupSlider(SliderAttributes{elem}, elem_callback);
                }
                else if ("CHECK_BOX" == element_type)
                {
                    setupCheckBox(CheckBoxAttributes{elem}, elem_callback);
                }
                else if ("EDITABLE_TEXT" == element_type)
                {
                    setupEditableText(EditableTextAttributes{elem}, elem_callback);
                }
                else if ("DROP_DOWN_MENU" == element_type)
                {
                    setupDropDownMenu(DropDownMenuAttributes{elem}, elem_callback);
                }
                else if ("LIST_BOX" == element_type)
                {
                    setupListBox(ListBoxAttributes{elem}, elem_callback);
                }
                else if ("RADIO_BUTTON" == element_type)
                {
                    setupRadioButton(RadioButtonAttributes{elem}, elem_callback);
                }
                else if ("TEXT_LABEL" == element_type)
                {
                    if (elem_callback != nullptr)
                    {
                        DVS_LOG_WARNING()
                            << "TextLabel does not support having a callback function. No callback will be added";
                    }
                    setupStaticText(StaticTextAttributes{elem}, elem_callback);
                }
                else if ("RADIO_BUTTON_GROUP" == element_type)
                {
                    // setupRadioButtonGroup(RadioButtonGroupAttributes{elem});
                    // gui_elements_[gui_element->getId()] = gui_element;
                    // Bind(wxEVT_RADIOBUTTON, &MainWindow::radioButtonCallback, this, gui_element->getId());
                }
                else
                {
                    std::cout << "Unknown element type: " << element_type << std::endl;
                }
            }
        }
    }
}

/*
Gui layout gets setup first, and then after data is loaded, callbacks are registered.
Then the "std::map<std::string, GuiElementCallback>& callbacks" data structure is already
with elements (i.e. the proper name for all gui elements), but the callback functions
are nullptr. Then after callbacks are registered, the callbacks data structure is checked
that all callbacks are not nullptr.

The gui_elements data structure must be a map with id as key, since when the gui callback
functions are fired the id is available in the event object.
*/

// std::map<std::string, GuiElementCallback>& callbacks

/*

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

*/

MainWindow::MainWindow()
    : wxFrame(NULL, wxID_ANY, "", wxPoint(200, 30), wxSize(700, 700)),
      dynamic_module_{"/Users/danielpi/work/dvs/src/build/applications/module_application/libmodule-lib.dylib"}
{
    // registerCallbacksFunctionType register_callbacks_function =
    //     loadModule("/Users/danielpi/work/dvs/src/build/applications/module_application/libmodule-lib.dylib");
    createGuiElements("/Users/danielpi/work/dvs/src/applications/module_application/example_project.json");

    /*wxArrayString arr;
    arr.Add("Test1");
    arr.Add("Test2");
    arr.Add("Test3");
    me_list_box->InsertItems(arr, 0);

    me_combo_box->Append("combo Test1");
    me_combo_box->Append("combo Test2");
    me_combo_box->Append("combo Test3");*/

    /*
A button is clicked
A slider is dragged and gets a new value
A checkbox is checked or unchecked
An editable text field is edited
A drop down menu is changed
A list box is changed
A radio button is changed

-- Gui Element Input Types:
A slider has its range set
A text field has its text set
A drop down menu has its items set
A list box has its items set
*/
}

MainWindow::~MainWindow() {}

void MainWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
}
