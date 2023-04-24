#include "main_window.h"

#include <mach-o/dyld.h>
#include <unistd.h>
#include <wx/wfstream.h>
#include <wx/wxprec.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

template <class C> class MovableElement : public C, public GuiElement
{
public:
    // Slider
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const wxWindowID id,
                   const int init_value,
                   const int min_value,
                   const int max_value,
                   const wxPoint& pos,
                   const wxSize& size,
                   const long style)
        : C(parent, id, init_value, min_value, max_value, pos, size, style), GuiElement(handle_string)
    {
        control_pressed_at_mouse_down = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // Button, Checkbox, RadioButton, ComboBox
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const wxWindowID id,
                   const std::string& label,
                   const wxPoint& pos,
                   const wxSize& size)
        : C(parent, id, label, pos, size), GuiElement(handle_string)
    {
        control_pressed_at_mouse_down = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // ListBox
    MovableElement(
        wxFrame* parent, const std::string& handle_string, const wxWindowID id, const wxPoint& pos, const wxSize& size)
        : C(parent, id, pos, size), GuiElement(handle_string)
    {
        control_pressed_at_mouse_down = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // EditableText
    MovableElement(wxFrame* parent,
                   const std::string& handle_string,
                   const wxWindowID id,
                   const std::string& initial_text,
                   const wxPoint& pos,
                   const wxSize& size,
                   const long style)
        : C(parent, id, initial_text, pos, size, style), GuiElement(handle_string)
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

GuiElement* MainWindow::setupButton(const ButtonAttributes& button_attributes)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &button_attributes);

    const std::string handle_string = button_attributes.handle_string;
    const std::string button_label = button_attributes.label;

    MovableElement<wxButton>* btn =
        new MovableElement<wxButton>(this, handle_string, wxID_ANY, button_label, elem_pos, elem_size);

    return static_cast<GuiElement*>(btn);
}

GuiElement* MainWindow::setupCheckBox(const CheckBoxAttributes& check_box_attributes)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &check_box_attributes);

    const std::string handle_string = check_box_attributes.handle_string;
    const std::string check_box_label = check_box_attributes.label;

    MovableElement<wxCheckBox>* cb =
        new MovableElement<wxCheckBox>(this, handle_string, wxID_ANY, check_box_label, elem_pos, elem_size);

    return static_cast<GuiElement*>(cb);
}

GuiElement* MainWindow::setupSlider(const SliderAttributes& slider_attributes)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &slider_attributes);

    const std::string handle_string = slider_attributes.handle_string;
    const int min_value = slider_attributes.min_value;
    const int max_value = slider_attributes.max_value;
    const int initial_value = slider_attributes.init_value;

    long style = slider_attributes.is_horizontal ? wxSL_HORIZONTAL : wxSL_VERTICAL;

    MovableElement<wxSlider>* slider = new MovableElement<wxSlider>(
        this, handle_string, wxID_ANY, initial_value, min_value, max_value, elem_pos, elem_size, style);

    return static_cast<GuiElement*>(slider);
}

GuiElement* MainWindow::setupEditableText(const EditableTextAttributes& editable_text_attributes)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &editable_text_attributes);

    const std::string handle_string = editable_text_attributes.handle_string;
    const std::string init_value = editable_text_attributes.init_value;

    MovableElement<wxTextCtrl>* editable_text = new MovableElement<wxTextCtrl>(
        this, handle_string, wxID_ANY, init_value, elem_pos, elem_size, wxTE_PROCESS_ENTER);

    return static_cast<GuiElement*>(editable_text);
}

GuiElement* MainWindow::setupDropDownMenu(const DropDownMenuAttributes& drop_down_menu_attributes)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &drop_down_menu_attributes);

    MovableElement<wxComboBox>* drop_down_menu = new MovableElement<wxComboBox>(this,
                                                                                drop_down_menu_attributes.handle_string,
                                                                                wxID_ANY,
                                                                                drop_down_menu_attributes.init_value,
                                                                                elem_pos,
                                                                                elem_size);

    return static_cast<GuiElement*>(drop_down_menu);
}

GuiElement* MainWindow::setupListBox(const ListBoxAttributes& list_box_attributes)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &list_box_attributes);

    MovableElement<wxListBox>* list_box =
        new MovableElement<wxListBox>(this, list_box_attributes.handle_string, wxID_ANY, elem_pos, elem_size);

    return static_cast<GuiElement*>(list_box);
}

GuiElement* MainWindow::setupStaticText(const StaticTextAttributes& static_text_attributes)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &static_text_attributes);

    MovableElement<wxStaticText>* static_text = new MovableElement<wxStaticText>(
        this, static_text_attributes.handle_string, wxID_ANY, static_text_attributes.label, elem_pos, elem_size);

    return static_cast<GuiElement*>(static_text);
}

GuiElement* MainWindow::setupRadioButton(const RadioButtonAttributes& radio_button_attributes)
{
    auto const [elem_pos, elem_size] = getPosAndSizeInPixelCoords(this->GetSize(), &radio_button_attributes);

    MovableElement<wxRadioButton>* radio_button = new MovableElement<wxRadioButton>(
        this, radio_button_attributes.handle_string, wxID_ANY, radio_button_attributes.label, elem_pos, elem_size);

    return static_cast<GuiElement*>(radio_button);
}

void MainWindow::createGuiElements(const std::string& path_to_layout_file)
{
    std::ifstream input_file(path_to_layout_file);
    nlohmann::json json_data;
    input_file >> json_data;

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

                GuiElement* gui_element{nullptr};

                /*
                Bind(wxEVT_BUTTON, &MainWindow::buttonCallback, this, me_btn->GetId());
                Bind(wxEVT_SLIDER, &MainWindow::sliderCallback, this, me_slider->GetId());
                Bind(wxEVT_RADIOBUTTON, &MainWindow::radioButtonCallback, this, me_radiobutton->GetId());
                Bind(wxEVT_CHECKBOX, &MainWindow::checkBoxCallback, this, me_checkbox->GetId());
                Bind(wxEVT_LISTBOX, &MainWindow::listBoxCallback, this, me_list_box->GetId());
                Bind(wxEVT_COMBOBOX, &MainWindow::comboBoxCallback, this, me_combo_box->GetId());
                Bind(wxEVT_TEXT, &MainWindow::textCtrlCallback, this, me_text_ctrl->GetId());
                Bind(wxEVT_TEXT_ENTER, &MainWindow::textCtrlEnterCallback, this, me_text_ctrl->GetId());*/

                // GuiElement* setupButton(const ButtonAttributes& element_data);
                // GuiElement* setupSlider(const SliderAttributes& slider_attributes);
                // GuiElement* setupCheckBox(const CheckBoxAttributes& check_box_attributes);
                // GuiElement* setupEditableText(const EditableTextAttributes& editable_text_attributes);
                // GuiElement* setupDropDownMenu(const DropDownMenuAttributes& drop_down_menu_attributes);
                // GuiElement* setupListBox(const ListBoxAttributes& list_box_attributes);
                // GuiElement* setupRadioButton(const RadioButtonAttributes& radio_button_attributes);
                // GuiElement* setupStaticText(const StaticTextAttributes& static_text_attributes);

                if ("BUTTON" == element_type)
                {
                    gui_element = setupButton(ButtonAttributes{elem});
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_BUTTON, &MainWindow::buttonCallback, this, gui_element->getId());
                }
                else if ("SLIDER" == element_type)
                {
                    gui_element = setupSlider(SliderAttributes{elem});
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_SLIDER, &MainWindow::sliderCallback, this, gui_element->getId());
                }
                else if ("CHECK_BOX" == element_type)
                {
                    gui_element = setupCheckBox(CheckBoxAttributes{elem});
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_CHECKBOX, &MainWindow::checkBoxCallback, this, gui_element->getId());
                }
                else if ("EDITABLE_TEXT" == element_type)
                {
                    gui_element = setupEditableText(EditableTextAttributes{elem});
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_TEXT, &MainWindow::textCtrlCallback, this, gui_element->getId());
                    Bind(wxEVT_TEXT_ENTER, &MainWindow::textCtrlEnterCallback, this, gui_element->getId());
                }
                else if ("DROP_DOWN_MENU" == element_type)
                {
                    gui_element = setupDropDownMenu(DropDownMenuAttributes{elem});
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_COMBOBOX, &MainWindow::comboBoxCallback, this, gui_element->getId());
                }
                else if ("LIST_BOX" == element_type)
                {
                    gui_element = setupListBox(ListBoxAttributes{elem});
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_LISTBOX, &MainWindow::listBoxCallback, this, gui_element->getId());
                }
                else if ("RADIO_BUTTON" == element_type)
                {
                    gui_element = setupRadioButton(RadioButtonAttributes{elem});
                    gui_elements_[gui_element->getId()] = gui_element;
                    Bind(wxEVT_RADIOBUTTON, &MainWindow::radioButtonCallback, this, gui_element->getId());
                }
                else if ("TEXT_LABEL" == element_type)
                {
                    gui_element = setupStaticText(StaticTextAttributes{elem});
                    gui_elements_[gui_element->getId()] = gui_element;
                }
                else
                {
                    std::cout << "Unknown element type: " << element_type << std::endl;
                }
            }
        }
    }
}

// void registerCallbacksPrototype(std::map<std::string, GuiElementCallback>& callbacks) {}

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

void MainWindow::buttonCallback(wxCommandEvent& event)
{
    const long id = event.GetId();

    if (gui_elements_.count(id) > 0)
    {
        std::cout << "There was a gui element with id: " << id
                  << " and handle string: " << gui_elements_[id]->getHandleString() << std::endl;
        // gui_elements_[id]->callback();
    }
    else
    {
        std::cout << "There was no gui element with id: " << id << std::endl;
    }
}

void MainWindow::sliderCallback(wxCommandEvent& event)
{
    std::cout << "sliderCallback" << std::endl;
}

void MainWindow::radioButtonCallback(wxCommandEvent& event)
{
    const bool is_checked = event.IsChecked();
    std::cout << "radioButtonCallback" << std::endl;
}

void MainWindow::checkBoxCallback(wxCommandEvent& event)
{
    const bool is_checked = event.IsChecked();
    std::cout << "checkBoxCallback" << std::endl;
}

void MainWindow::comboBoxCallback(wxCommandEvent& event)
{
    std::cout << "comboBoxCallback" << std::endl;
}

void MainWindow::listBoxCallback(wxCommandEvent& event)
{
    std::cout << "listBoxCallback" << std::endl;
}

void MainWindow::textCtrlCallback(wxCommandEvent& event)
{
    wxString t = event.GetString();
    std::cout << "textCtrlCallback: " << t << std::endl;
}

void MainWindow::textCtrlEnterCallback(wxCommandEvent& event)
{
    wxString t = event.GetString();
    std::cout << "textCtrlEnterCallback: " << t << std::endl;
}

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, "", wxPoint(200, 30), wxSize(700, 700))
{
    // MovableElement<wxButton>* me_btn =
    //     new MovableElement<wxButton>(this, wxID_ANY, "Test", wxPoint(200, 0), wxSize(100, 100));
    /*MovableElement<wxSlider>* me_slider =
        new MovableElement<wxSlider>(this, wxID_ANY, 13, 0, 100, wxPoint(200, 100), wxSize(100, 30), wxSL_HORIZONTAL);
    MovableElement<wxRadioButton>* me_radiobutton =
        new MovableElement<wxRadioButton>(this, wxID_ANY, "Test", wxPoint(200, 200), wxSize(100, 30));
    MovableElement<wxCheckBox>* me_checkbox =
        new MovableElement<wxCheckBox>(this, wxID_ANY, "Test", wxPoint(200, 300), wxSize(100, 30));
    MovableElement<wxComboBox>* me_combo_box =
        new MovableElement<wxComboBox>(this, wxID_ANY, "Test", wxPoint(200, 400), wxSize(100, 30));
    MovableElement<wxListBox>* me_list_box =
        new MovableElement<wxListBox>(this, wxID_ANY, wxPoint(200, 500), wxSize(100, 30));
    MovableElement<wxTextCtrl>* me_text_ctrl =
        new MovableElement<wxTextCtrl>(this, wxID_ANY, "Test", wxPoint(200, 600), wxSize(100, 30),
    wxTE_PROCESS_ENTER);*/

    createGuiElements("/Users/danielpi/work/dvs/src/applications/module_application/example_project.json");

    /*wxArrayString arr;
    arr.Add("Test1");
    arr.Add("Test2");
    arr.Add("Test3");
    me_list_box->InsertItems(arr, 0);

    me_combo_box->Append("combo Test1");
    me_combo_box->Append("combo Test2");
    me_combo_box->Append("combo Test3");

    Bind(wxEVT_BUTTON, &MainWindow::buttonCallback, this, me_btn->GetId());
    Bind(wxEVT_SLIDER, &MainWindow::sliderCallback, this, me_slider->GetId());
    Bind(wxEVT_RADIOBUTTON, &MainWindow::radioButtonCallback, this, me_radiobutton->GetId());
    Bind(wxEVT_CHECKBOX, &MainWindow::checkBoxCallback, this, me_checkbox->GetId());
    Bind(wxEVT_LISTBOX, &MainWindow::listBoxCallback, this, me_list_box->GetId());
    Bind(wxEVT_COMBOBOX, &MainWindow::comboBoxCallback, this, me_combo_box->GetId());
    Bind(wxEVT_TEXT, &MainWindow::textCtrlCallback, this, me_text_ctrl->GetId());
    Bind(wxEVT_TEXT_ENTER, &MainWindow::textCtrlEnterCallback, this, me_text_ctrl->GetId());*/

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
