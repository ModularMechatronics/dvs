#include "main_window.h"

#include <mach-o/dyld.h>
#include <unistd.h>
#include <wx/wfstream.h>
#include <wx/wxprec.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

void registerCallbacksPrototype(std::map<std::string, GuiElementCallback>& callbacks)
{
    callbacks["button0"] = [](const GuiElementEventData& gui_element_event_data,
                              const GuiElement* const origin_gui_element) -> void {
        std::cout << "User callback from " << origin_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["slider0"] = [](const GuiElementEventData& gui_element_event_data,
                              const GuiElement* const origin_gui_element) -> void {
        std::cout << "User callback from " << origin_gui_element->getHandleString() << std::endl;
        const SliderData slider_data = gui_element_event_data.getSliderData();

        std::cout << "Slider min: " << slider_data.min_value << std::endl;
        std::cout << "Slider max: " << slider_data.max_value << std::endl;
        std::cout << "Slider data: " << slider_data.value << std::endl;
    };

    callbacks["check_box0"] = [](const GuiElementEventData& gui_element_event_data,
                                 const GuiElement* const origin_gui_element) -> void {
        std::cout << "User callback from " << origin_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["editable_text0"] = [](const GuiElementEventData& gui_element_event_data,
                                     const GuiElement* const origin_gui_element) -> void {
        static_cast<void>(gui_element_event_data);

        const EditableTextData editable_text_data = gui_element_event_data.getEditableTextData();
        std::cout << "User callback from " << origin_gui_element->getHandleString()
                  << " with text data: " << editable_text_data.text_data << std::endl;
    };

    callbacks["drop_down_menu0"] = [](const GuiElementEventData& gui_element_event_data,
                                      const GuiElement* const origin_gui_element) -> void {
        std::cout << "User callback from " << origin_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["list_box0"] = [](const GuiElementEventData& gui_element_event_data,
                                const GuiElement* const origin_gui_element) -> void {
        std::cout << "User callback from " << origin_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["radio_button0x"] = [](const GuiElementEventData& gui_element_event_data,
                                     const GuiElement* const origin_gui_element) -> void {
        std::cout << "User callback from " << origin_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };

    callbacks["text_label0"] = [](const GuiElementEventData& gui_element_event_data,
                                  const GuiElement* const origin_gui_element) -> void {
        std::cout << "User callback from " << origin_gui_element->getHandleString() << std::endl;
        static_cast<void>(gui_element_event_data);
    };
}

template <class C> class MovableElement : public C, public GuiElement
{
public:
    // Slider
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const int init_value,
                   const int min_value,
                   const int max_value,
                   const wxPoint& pos,
                   const wxSize& size,
                   const long style)
        : C(parent, id, init_value, min_value, max_value, pos, size, style),
          GuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // Button, Checkbox, RadioButton, ComboBox
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const std::string& label,
                   const wxPoint& pos,
                   const wxSize& size)
        : C(parent, id, label, pos, size), GuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // ListBox
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const wxPoint& pos,
                   const wxSize& size)
        : C(parent, id, pos, size), GuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // EditableText
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const GuiElementCallback& elem_callback,
                   const GuiElementType gui_element_type,
                   const wxWindowID id,
                   const std::string& initial_text,
                   const wxPoint& pos,
                   const wxSize& size,
                   const long style)
        : C(parent, id, initial_text, pos, size, style), GuiElement(handle_string, elem_callback, gui_element_type)
    {
        control_pressed_at_mouse_down = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    void mouseLeftPressed(wxMouseEvent& event)
    {
        if (wxGetKeyState(WXK_CONTROL))
        {
            control_pressed_at_mouse_down = true;

            const wxPoint pos_at_press = this->GetPosition();

            mouse_pos_at_press_ = event.GetPosition() + pos_at_press;
            previous_mouse_pos_ = mouse_pos_at_press_;
        }
        else
        {
            event.Skip();
        }
    }

    void mouseLeftReleased(wxMouseEvent& event)
    {
        if (control_pressed_at_mouse_down)
        {
            control_pressed_at_mouse_down = false;
        }
        else
        {
            event.Skip();
        }
    }

    void mouseMovedOverItem(wxMouseEvent& event)
    {
        if (control_pressed_at_mouse_down && event.LeftIsDown())
        {
            const wxPoint current_mouse_position_local = event.GetPosition();
            const wxPoint current_mouse_position_global = current_mouse_position_local + this->GetPosition();
            const wxPoint delta = current_mouse_position_global - previous_mouse_pos_;
            this->SetPosition(this->GetPosition() + delta);

            previous_mouse_pos_ = current_mouse_position_global;
        }
        else
        {
            event.Skip();
        }
    }

    long getId() const override
    {
        return this->GetId();
    }

private:
    bool control_pressed_at_mouse_down;
    wxPoint mouse_pos_at_press_;
    wxPoint previous_mouse_pos_;
};

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

void MainWindow::createGuiElements(const std::string& path_to_layout_file)
{
    std::ifstream input_file(path_to_layout_file);
    nlohmann::json json_data;
    input_file >> json_data;

    // TODO: No validation is possible to make sure that the assigned functions in "registerCallbacksPrototype" are
    // available from the json layout definition

    std::map<std::string, GuiElementCallback> callbacks;
    registerCallbacksPrototype(callbacks);

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

void MainWindow::buttonCallback(wxCommandEvent& event)
{
    const long id = event.GetId();

    if (gui_elements_.count(id) > 0)
    {
        const VectorConstView<std::uint8_t> event_raw_data{nullptr, 0U};

        gui_elements_[id]->callback(event_raw_data);
    }
    else
    {
        std::cout << "There was no gui element with id: " << id << std::endl;
    }
}

void MainWindow::sliderCallback(wxCommandEvent& event)
{
    const long id = event.GetId();

    if (gui_elements_.count(id) > 0)
    {
        SliderData slider_data;

        const MovableElement<wxSlider>* const sldr =
            static_cast<const MovableElement<wxSlider>* const>(gui_elements_[id]);

        slider_data.max_value = sldr->GetMax();
        slider_data.min_value = sldr->GetMin();
        slider_data.value = event.GetInt();

        gui_elements_[id]->callback(
            VectorConstView<std::uint8_t>{reinterpret_cast<std::uint8_t*>(&slider_data), sizeof(SliderData)});
    }
    else
    {
        std::cout << "There was no gui element with id: " << id << std::endl;
    }
}

void MainWindow::radioButtonCallback(wxCommandEvent& event)
{
    const long id = event.GetId();

    const bool is_checked = event.IsChecked();
    if (gui_elements_.count(id) > 0)
    {
        const bool is_checked = event.IsChecked();

        std::array<std::uint8_t, 1> event_raw_data;
        event_raw_data[0] = is_checked ? 1U : 0U;

        gui_elements_[id]->callback(VectorConstView<std::uint8_t>{event_raw_data.data(), 1U});
    }
    else
    {
        std::cout << "There was no gui element with id: " << id << std::endl;
    }
}

void MainWindow::checkBoxCallback(wxCommandEvent& event)
{
    const long id = event.GetId();

    if (gui_elements_.count(id) > 0)
    {
        const bool is_checked = event.IsChecked();

        std::array<std::uint8_t, 1> event_raw_data;
        event_raw_data[0] = is_checked ? 1U : 0U;

        gui_elements_[id]->callback(VectorConstView<std::uint8_t>{event_raw_data.data(), 1U});
    }
    else
    {
        std::cout << "There was no gui element with id: " << id << std::endl;
    }
}

void MainWindow::comboBoxCallback(wxCommandEvent& event)
{
    const long id = event.GetId();

    if (gui_elements_.count(id) > 0)
    {
        const VectorConstView<std::uint8_t> event_raw_data{nullptr, 0U};

        gui_elements_[id]->callback(event_raw_data);
    }
    else
    {
        std::cout << "There was no gui element with id: " << id << std::endl;
    }
}

void MainWindow::listBoxCallback(wxCommandEvent& event)
{
    const long id = event.GetId();

    if (gui_elements_.count(id) > 0)
    {
        const VectorConstView<std::uint8_t> event_raw_data{nullptr, 0U};

        gui_elements_[id]->callback(event_raw_data);
    }
    else
    {
        std::cout << "There was no gui element with id: " << id << std::endl;
    }
}

void MainWindow::textCtrlCallback(wxCommandEvent& event)
{
    const long id = event.GetId();

    if (gui_elements_.count(id) > 0)
    {
        const wxString t = event.GetString();

        std::vector<std::uint8_t> event_raw_data;

        event_raw_data.resize(t.length() + sizeof(std::uint16_t) + 1U);

        event_raw_data[0] = 0U;  // Enter is not pressed

        std::uint16_t num_characters{static_cast<std::uint16_t>(t.length())};

        std::memcpy(event_raw_data.data() + 1U, &num_characters, sizeof(std::uint16_t));

        for (size_t k = 0; k < t.length(); ++k)
        {
            event_raw_data[k + sizeof(std::uint16_t) + 1U] = t[k];
        }

        gui_elements_[id]->callback(VectorConstView<std::uint8_t>{event_raw_data.data(), event_raw_data.size()});
    }
    else
    {
        std::cout << "There was no gui element with id: " << id << std::endl;
    }
}

void MainWindow::textCtrlEnterCallback(wxCommandEvent& event)
{
    const long id = event.GetId();
    wxString t = event.GetString();
    if (gui_elements_.count(id) > 0)
    {
        const VectorConstView<std::uint8_t> event_raw_data{nullptr, 0U};

        gui_elements_[id]->callback(event_raw_data);
    }
    else
    {
        std::cout << "There was no gui element with id: " << id << std::endl;
    }
}

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
