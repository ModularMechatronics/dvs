#include "main_window.h"

#include <dlfcn.h>
#include <errno.h>
#include <mach-o/dyld.h>
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

GuiElement* MainWindow::setupButton(const ButtonAttributes& button_attributes, const GuiElementCallback& elem_callback)
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

    return static_cast<GuiElement*>(btn);
}

GuiElement* MainWindow::setupCheckBox(const CheckBoxAttributes& check_box_attributes,
                                      const GuiElementCallback& elem_callback)
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

    return static_cast<GuiElement*>(cb);
}

GuiElement* MainWindow::setupSlider(const SliderAttributes& slider_attributes, const GuiElementCallback& elem_callback)
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

    return static_cast<GuiElement*>(slider);
}

GuiElement* MainWindow::setupEditableText(const EditableTextAttributes& editable_text_attributes,
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

    return static_cast<GuiElement*>(editable_text);
}

GuiElement* MainWindow::setupDropDownMenu(const DropDownMenuAttributes& drop_down_menu_attributes,
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

    return static_cast<GuiElement*>(drop_down_menu);
}

GuiElement* MainWindow::setupListBox(const ListBoxAttributes& list_box_attributes,
                                     const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &list_box_attributes);

    MovableElement<wxListBox>* list_box = new MovableElement<wxListBox>(
        this, list_box_attributes.handle_string, elem_callback, GuiElementType::ListBox, wxID_ANY, elem_pos, elem_size);

    return list_box;
}

GuiElement* MainWindow::setupStaticText(const StaticTextAttributes& static_text_attributes,
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

    return static_cast<GuiElement*>(static_text);
}

GuiElement* MainWindow::setupRadioButton(const RadioButtonAttributes& radio_button_attributes,
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

    return static_cast<GuiElement*>(radio_button);
}

// typedef int (*some_func)(void);
// typedef void (*simpleFunctionType)(const std::string& msg);
// typedef void (*simpleFunctionType)(const char* const message);

typedef void (*registerCallbacksFunctionType)(std::map<std::string, GuiElementCallback>& callbacks);

registerCallbacksFunctionType loadModule(const std::string& path_to_module)
{
    void* lib_ptr = dlopen(path_to_module.c_str(), RTLD_NOW);  // RTLD_GLOBAL, RTLD_NOW

    if ((!lib_ptr) || (lib_ptr == NULL) || (lib_ptr == 0) || (lib_ptr == nullptr))
    {
        DVS_LOG_ERROR() << "Library not loaded!";
    }

    char* error_str = dlerror();

    if (error_str != NULL)
    {
        DVS_LOG_ERROR() << "Error: " << error_str;
    }
    else
    {
        std::cout << "Module loaded successfully!" << std::endl;
    }

    registerCallbacksFunctionType register_callbacks_function =
        reinterpret_cast<registerCallbacksFunctionType>(dlsym(lib_ptr, "registerCallbacks"));

    if ((!register_callbacks_function) || (register_callbacks_function == NULL) || (register_callbacks_function == 0) ||
        (register_callbacks_function == nullptr))
    {
        DVS_LOG_ERROR() << "Function not loaded!";
    }

    // dlclose(lib_ptr);

    return register_callbacks_function;
}

void MainWindow::createGuiElements(const std::string& path_to_layout_file)
{
    std::ifstream input_file(path_to_layout_file);
    nlohmann::json json_data;
    input_file >> json_data;

    // TODO: No validation is possible to make sure that the assigned functions in "registerCallbacks" are
    // available from the json layout definition

    registerCallbacksFunctionType register_callbacks_function =
        loadModule("/Users/danielpi/work/dvs/src/build/applications/module_application/libmodule-lib.dylib");

    std::map<std::string, GuiElementCallback> callbacks;

    register_callbacks_function(callbacks);

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

                const GuiElementCallback& elem_callback = callbacks[handle_string];

                GuiElement* gui_element{nullptr};

                if ("BUTTON" == element_type)
                {
                    gui_element = setupButton(ButtonAttributes{elem}, elem_callback);
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_BUTTON, &MainWindow::buttonCallback, this, gui_element->getId());
                }
                else if ("SLIDER" == element_type)
                {
                    gui_element = setupSlider(SliderAttributes{elem}, elem_callback);
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_SLIDER, &MainWindow::sliderCallback, this, gui_element->getId());
                }
                else if ("CHECK_BOX" == element_type)
                {
                    gui_element = setupCheckBox(CheckBoxAttributes{elem}, elem_callback);
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_CHECKBOX, &MainWindow::checkBoxCallback, this, gui_element->getId());
                }
                else if ("EDITABLE_TEXT" == element_type)
                {
                    gui_element = setupEditableText(EditableTextAttributes{elem}, elem_callback);
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_TEXT, &MainWindow::textCtrlCallback, this, gui_element->getId());
                    Bind(wxEVT_TEXT_ENTER, &MainWindow::textCtrlEnterCallback, this, gui_element->getId());
                }
                else if ("DROP_DOWN_MENU" == element_type)
                {
                    gui_element = setupDropDownMenu(DropDownMenuAttributes{elem}, elem_callback);
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_COMBOBOX, &MainWindow::comboBoxCallback, this, gui_element->getId());
                }
                else if ("LIST_BOX" == element_type)
                {
                    gui_element = setupListBox(ListBoxAttributes{elem}, elem_callback);
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_LISTBOX, &MainWindow::listBoxCallback, this, gui_element->getId());
                }
                else if ("RADIO_BUTTON" == element_type)
                {
                    gui_element = setupRadioButton(RadioButtonAttributes{elem}, elem_callback);
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_RADIOBUTTON, &MainWindow::radioButtonCallback, this, gui_element->getId());
                }
                else if ("TEXT_LABEL" == element_type)
                {
                    gui_element = setupStaticText(StaticTextAttributes{elem}, elem_callback);
                    gui_elements_[gui_element->getId()] = gui_element;
                }
                else if ("RADIO_BUTTON_GROUP" == element_type)
                {
                    // gui_element = setupRadioButtonGroup(RadioButtonGroupAttributes{elem});
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

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, "", wxPoint(200, 30), wxSize(700, 700))
{
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
