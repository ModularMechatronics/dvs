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
    ElementAttributes() : handle_string(""), x(0.0f), y(0.0f), width(0.0f), height(0.0f) {}
    ElementAttributes(const nlohmann::json& j_data)
        : handle_string(j_data["handle_string"]),
          x(j_data["x"]),
          y(j_data["y"]),
          width(j_data["width"]),
          height(j_data["height"])
    {
    }
};

struct ButtonAttributes : public ElementAttributes
{
    std::string label;
    ButtonAttributes() : ElementAttributes(), label("") {}
    ButtonAttributes(const nlohmann::json& j_data) : ElementAttributes(j_data), label(j_data["attributes"]["label"]) {}
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
    void createGuiElements(const std::string& path_to_layout_file);

    void sliderHandler(wxCommandEvent& event);

    std::map<long, GuiElement*> gui_elements_;

public:
    MainWindow();
    MainWindow(const std::vector<std::string>& cmdl_args);
    ~MainWindow();
};

#endif  // MODULE_APPLICATION_MAIN_WINDOW_H_
