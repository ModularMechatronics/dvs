#include "main_window.h"

#include <unistd.h>
#include <wx/wfstream.h>
#include <wx/wxprec.h>

#include <csignal>
#include <fstream>
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

void MainWindow::guiElementCallback(wxCommandEvent& event)
{
    const long id = event.GetId();

    if (gui_elements_.count(id) > 0)
    {
        gui_elements_[id]->callback();
    }
    else
    {
        std::cout << "There was no gui element with id: " << id << std::endl;
    }
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

    gui_elements_[btn->getId()] = static_cast<api_internal::InternalGuiElement*>(btn);
    Bind(wxEVT_BUTTON, &MainWindow::guiElementCallback, this, btn->getId());
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

    gui_elements_[cb->getId()] = static_cast<api_internal::InternalGuiElement*>(cb);
    Bind(wxEVT_CHECKBOX, &MainWindow::guiElementCallback, this, cb->getId());
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

    gui_elements_[slider->getId()] = static_cast<api_internal::InternalGuiElement*>(slider);
    Bind(wxEVT_SLIDER, &MainWindow::guiElementCallback, this, slider->getId());
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
    gui_elements_[editable_text->getId()] = static_cast<api_internal::InternalGuiElement*>(editable_text);
    Bind(wxEVT_TEXT, &MainWindow::guiElementCallback, this, editable_text->getId());
    Bind(wxEVT_TEXT_ENTER, &MainWindow::guiElementCallback, this, editable_text->getId());
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
    gui_elements_[drop_down_menu->getId()] = static_cast<api_internal::InternalGuiElement*>(drop_down_menu);
    Bind(wxEVT_COMBOBOX, &MainWindow::guiElementCallback, this, drop_down_menu->getId());
    return;
}

void MainWindow::setupListBox(const ListBoxAttributes& list_box_attributes, const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &list_box_attributes);

    MovableElement<wxListBox>* list_box = new MovableElement<wxListBox>(
        this, list_box_attributes.handle_string, elem_callback, GuiElementType::ListBox, wxID_ANY, elem_pos, elem_size);
    gui_elements_[list_box->getId()] = static_cast<api_internal::InternalGuiElement*>(list_box);
    Bind(wxEVT_LISTBOX, &MainWindow::guiElementCallback, this, list_box->getId());
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
    gui_elements_[static_text->getId()] = static_cast<api_internal::InternalGuiElement*>(static_text);
}

void MainWindow::setupRadioButtonGroup(const RadioButtonGroupAttributes& radio_button_group_attributes,
                                       const GuiElementCallback& elem_callback)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &radio_button_group_attributes);

    // wxArrayString *selection = new wxArrayString;
    wxArrayString radio_buttons_to_add{};

    for (const auto& radio_button_attribute : radio_button_group_attributes.radio_buttons)
    {
        radio_buttons_to_add.Add(radio_button_attribute.label);
    }

    MovableElement<wxRadioBox>* radio_box = new MovableElement<wxRadioBox>(this,
                                                                           radio_button_group_attributes.handle_string,
                                                                           elem_callback,
                                                                           GuiElementType::RadioButtonGroup,
                                                                           wxID_ANY,
                                                                           radio_button_group_attributes.label,
                                                                           elem_pos,
                                                                           elem_size,
                                                                           radio_buttons_to_add,
                                                                           1,
                                                                           wxRA_SPECIFY_COLS);

    gui_elements_[radio_box->getId()] = static_cast<api_internal::InternalGuiElement*>(radio_box);
    Bind(wxEVT_RADIOBOX, &MainWindow::guiElementCallback, this, radio_box->getId());
}

void MainWindow::createGuiElements(const std::string& path_to_layout_file)
{
    std::ifstream input_file(path_to_layout_file);
    nlohmann::json json_data;
    input_file >> json_data;

    // TODO: No validation is possible to make sure that the assigned functions in "registerCallbacks" are
    // available from the json layout definition

    get_gui_element_function_ = [this](const std::string& handle_string) -> GuiElement {
        auto q = std::find_if(gui_elements_.begin(), gui_elements_.end(), [&handle_string](const auto& elem) -> bool {
            return elem.second->getHandleString() == handle_string;
        });

        if (q == gui_elements_.end())
        {
            DVS_LOG_ERROR() << "Could not find gui element with handle string: " << handle_string
                            << ", returning empty object.";
            return GuiElement{};
        }
        else
        {
            return GuiElement{q->second};
        }
    };

    get_timer_function_ = [this](const std::string& handle_string) -> Timer {
        auto q = std::find_if(timers_.begin(), timers_.end(), [&handle_string](const auto& elem) -> bool {
            return elem.second->getHandleString() == handle_string;
        });

        if (q == timers_.end())
        {
            DVS_LOG_ERROR() << "Could not find timer with handle string: " << handle_string
                            << ", returning empty object.";
            return Timer{};
        }
        else
        {
            return Timer{q->second};
        }
    };

    std::map<std::string, GuiElementCallback> gui_element_callbacks;
    std::map<std::string, TimerCallback> timer_callbacks;

    dynamic_module_.registerCallbacks(
        gui_element_callbacks, timer_callbacks, get_gui_element_function_, get_timer_function_);

    for (const auto& elem : timer_callbacks)
    {
        timers_[elem.first] = new TimerImplementation(elem.first, elem.second);
    }

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
                if (gui_element_callbacks.find(handle_string) != gui_element_callbacks.end())
                {
                    elem_callback = gui_element_callbacks[handle_string];
                }

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
                    setupRadioButtonGroup(RadioButtonGroupAttributes{elem}, elem_callback);
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
    TextLabel
};

*/

MainWindow::MainWindow()
    : wxFrame(NULL, wxID_ANY, "", wxPoint(400, 30), wxSize(700, 700)),
      dynamic_module_{"/Users/danielpi/work/dvs/src/build/applications/module_application/libmodule-lib.dylib"}
{
    createGuiElements("/Users/danielpi/work/dvs/src/applications/module_application/example_project.json");

    {
        auto q = std::find_if(gui_elements_.begin(), gui_elements_.end(), [](auto& elem) {
            return elem.second->getHandleString() == "drop_down_menu0";
        });

        if (q != gui_elements_.end())
        {
            MovableElement<wxComboBox>* drop_down = static_cast<MovableElement<wxComboBox>*>(q->second);
            drop_down->Append("combo Test1");
            drop_down->Append("combo Test2");
            drop_down->Append("combo Test3");
        }
    }

    {
        auto q = std::find_if(gui_elements_.begin(), gui_elements_.end(), [](auto& elem) {
            return elem.second->getHandleString() == "list_box0";
        });

        if (q != gui_elements_.end())
        {
            MovableElement<wxListBox>* list_box = static_cast<MovableElement<wxListBox>*>(q->second);
            wxArrayString arr;
            arr.Add("Test1");
            arr.Add("Test2");
            arr.Add("Test3");
            list_box->InsertItems(arr, 0);
        }
    }

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
