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

#include "module_api.h"

class GuiElement
{
public:
    GuiElement(const std::string& handle_string) : callback_function_{nullptr}, handle_string_{handle_string} {}
    virtual ~GuiElement() = default;

    virtual long getId() const = 0;

    void callback(const GuiElementEventData& event_data)
    {
        if (callback_function_)
        {
            callback_function_(event_data);
        }
        else
        {
            DVS_LOG_ERROR() << "No callback function set for GuiElement with handle string: " << getHandleString();
        }
    }

    std::string getHandleString() const
    {
        return handle_string_;
    }

protected:
    GuiElementCallback callback_function_;
    std::string handle_string_;

    float x_;
    float y_;
    float width_;
    float height_;
};

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
    void OnSize(wxSizeEvent& event);

    void buttonCallback(wxCommandEvent& evt);
    void sliderCallback(wxCommandEvent& evt);
    void radioButtonCallback(wxCommandEvent& evt);
    void checkBoxCallback(wxCommandEvent& evt);
    void comboBoxCallback(wxCommandEvent& evt);
    void listBoxCallback(wxCommandEvent& evt);
    void textCtrlCallback(wxCommandEvent& evt);
    void textCtrlEnterCallback(wxCommandEvent& evt);

    GuiElement* setupButton(const ButtonAttributes& element_data);
    GuiElement* setupSlider(const SliderAttributes& slider_attributes);
    GuiElement* setupCheckBox(const CheckBoxAttributes& check_box_attributes);
    GuiElement* setupEditableText(const EditableTextAttributes& editable_text_attributes);
    GuiElement* setupDropDownMenu(const DropDownMenuAttributes& drop_down_menu_attributes);
    GuiElement* setupListBox(const ListBoxAttributes& list_box_attributes);
    GuiElement* setupRadioButton(const RadioButtonAttributes& radio_button_attributes);
    GuiElement* setupStaticText(const StaticTextAttributes& static_text_attributes);

    void createGuiElements(const std::string& path_to_layout_file);

    void sliderHandler(wxCommandEvent& event);

    std::map<long, GuiElement*> gui_elements_;

public:
    MainWindow();
    MainWindow(const std::vector<std::string>& cmdl_args);
    ~MainWindow();
};

#endif  // MODULE_APPLICATION_MAIN_WINDOW_H_
