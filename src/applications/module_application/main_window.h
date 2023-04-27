#ifndef MODULE_APPLICATION_MAIN_WINDOW_H_
#define MODULE_APPLICATION_MAIN_WINDOW_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <map>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <thread>
#include <tuple>
#include <utility>
#include <vector>

#include "dynamic_module.h"
#include "module_api.h"

struct ElementAttributes
{
    std::string handle_string;
    float x;
    float y;
    float width;
    float height;
    ElementAttributes() : handle_string(""), x{0.0f}, y{0.0f}, width{0.0f}, height{0.0f} {}
    ElementAttributes(const nlohmann::json& j_data)
        : handle_string{j_data["handle_string"]},
          x{j_data["x"]},
          y{j_data["y"]},
          width{j_data["width"]},
          height{j_data["height"]}
    {
    }
};

struct ButtonAttributes : public ElementAttributes
{
    std::string label;
    ButtonAttributes() : ElementAttributes{}, label{""} {}
    ButtonAttributes(const nlohmann::json& j_data) : ElementAttributes{j_data}, label{j_data["attributes"]["label"]} {}
};

struct CheckBoxAttributes : public ElementAttributes
{
    std::string label;
    CheckBoxAttributes() : ElementAttributes{}, label{""} {}
    CheckBoxAttributes(const nlohmann::json& j_data) : ElementAttributes{j_data}, label{j_data["attributes"]["label"]}
    {
    }
};

struct EditableTextAttributes : public ElementAttributes
{
    std::string init_value;
    EditableTextAttributes() : ElementAttributes{}, init_value{""} {}
    EditableTextAttributes(const nlohmann::json& j_data)
        : ElementAttributes{j_data}, init_value{j_data["attributes"]["init_value"]}
    {
    }
};

struct DropDownMenuAttributes : public ElementAttributes
{
    std::string init_value;
    DropDownMenuAttributes() : ElementAttributes{}, init_value{} {}
    DropDownMenuAttributes(const nlohmann::json& j_data)
        : ElementAttributes{j_data}, init_value{j_data["attributes"]["init_value"]}
    {
    }
};

struct ListBoxAttributes : public ElementAttributes
{
    ListBoxAttributes() : ElementAttributes{} {}
    ListBoxAttributes(const nlohmann::json& j_data) : ElementAttributes{j_data} {}
};

struct RadioButtonAttributes : public ElementAttributes
{
    std::string label;
    RadioButtonAttributes() : ElementAttributes{}, label{""} {}
    RadioButtonAttributes(const nlohmann::json& j_data)
        : ElementAttributes{j_data}, label{j_data["attributes"]["label"]}
    {
    }
};

struct RadioButtonGroupAttributes : public ElementAttributes
{
    std::string label;
    RadioButtonGroupAttributes() : ElementAttributes{}, label{""} {}
    RadioButtonGroupAttributes(const nlohmann::json& j_data)
        : ElementAttributes{j_data}, label{j_data["attributes"]["label"]}
    {
    }
};

struct StaticTextAttributes : public ElementAttributes
{
    std::string label;
    StaticTextAttributes() : ElementAttributes{}, label{""} {}
    StaticTextAttributes(const nlohmann::json& j_data) : ElementAttributes{j_data}, label{j_data["attributes"]["label"]}
    {
    }
};

template <typename T> T getOptionalValue(const nlohmann::json& j_data, const std::string& key, const T& default_value)
{
    if (j_data.contains(key))
    {
        return j_data[key];
    }
    else
    {
        return default_value;
    }
}

struct SliderAttributes : public ElementAttributes
{
    int min_value;
    int max_value;
    int init_value;
    int step_size;
    bool is_horizontal;
    SliderAttributes()
        : ElementAttributes(), min_value{0}, max_value{100}, init_value{0}, step_size{1}, is_horizontal{true}
    {
    }

    SliderAttributes(const nlohmann::json& j_data) : ElementAttributes(j_data)
    {
        if (j_data.contains("attributes"))
        {
            min_value = getOptionalValue(j_data["attributes"], "min", 0);
            max_value = getOptionalValue(j_data["attributes"], "max", 100);
            init_value = getOptionalValue(j_data["attributes"], "init_value", 0);
            step_size = getOptionalValue(j_data["attributes"], "step_size", 1);
            if (j_data["attributes"].contains("style"))
            {
                is_horizontal = j_data["attributes"]["style"] == "horizontal";
            }
            else
            {
                is_horizontal = true;
            }
        }
        else
        {
            min_value = 0;
            max_value = 100;
            init_value = 0;
            step_size = 1;
            is_horizontal = true;
        }
    }
};

class MainWindow : public wxFrame
{
private:
    DynamicModule dynamic_module_;
    std::map<long, GuiElement*> gui_elements_;

    void OnSize(wxSizeEvent& event);

    void buttonCallback(wxCommandEvent& evt);
    void sliderCallback(wxCommandEvent& evt);
    void radioButtonCallback(wxCommandEvent& evt);
    void checkBoxCallback(wxCommandEvent& evt);
    void comboBoxCallback(wxCommandEvent& evt);
    void listBoxCallback(wxCommandEvent& evt);
    void textCtrlCallback(wxCommandEvent& evt);
    void textCtrlEnterCallback(wxCommandEvent& evt);

    void setupButton(const ButtonAttributes& element_data, const GuiElementCallback& elem_callback);
    void setupSlider(const SliderAttributes& slider_attributes, const GuiElementCallback& elem_callback);
    void setupCheckBox(const CheckBoxAttributes& check_box_attributes, const GuiElementCallback& elem_callback);
    void setupEditableText(const EditableTextAttributes& editable_text_attributes,
                           const GuiElementCallback& elem_callback);
    void setupDropDownMenu(const DropDownMenuAttributes& drop_down_menu_attributes,
                           const GuiElementCallback& elem_callback);
    void setupListBox(const ListBoxAttributes& list_box_attributes, const GuiElementCallback& elem_callback);
    void setupRadioButton(const RadioButtonAttributes& radio_button_attributes,
                          const GuiElementCallback& elem_callback);
    void setupStaticText(const StaticTextAttributes& static_text_attributes, const GuiElementCallback& elem_callback);

    void createGuiElements(const std::string& path_to_layout_file);

    void sliderHandler(wxCommandEvent& event);

public:
    MainWindow();
    MainWindow(const std::vector<std::string>& cmdl_args);
    ~MainWindow();
};

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

    long getValue() const override
    {
        const wxSlider* const slider = dynamic_cast<const wxSlider* const>(this);
        return slider->GetValue();
    }

    void setMax(const int new_max) override
    {
        wxSlider* const slider = dynamic_cast<wxSlider* const>(this);
        slider->SetMax(new_max);
    }

private:
    bool control_pressed_at_mouse_down;
    wxPoint mouse_pos_at_press_;
    wxPoint previous_mouse_pos_;
};

#endif  // MODULE_APPLICATION_MAIN_WINDOW_H_
