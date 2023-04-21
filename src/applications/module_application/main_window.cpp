#include "main_window.h"

#include <mach-o/dyld.h>
#include <unistd.h>
#include <wx/wfstream.h>
#include <wx/wxprec.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

void MainWindow::buttonCallback(wxCommandEvent& event)
{
    std::cout << "buttonCallback" << std::endl;
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

template <class C> class MovableElement : public C
{
public:
    /*static MovableElement* createSlider(wxFrame* parent,
                                        const wxWindowID id,
                                        const int init_value,
                                        const int min_value,
                                        const int max_value,
                                        const wxPoint& pos,
                                        const wxSize& size)
    {
        return new MovableElement(parent, id, init_value, min_value, max_value, pos, size);
    }

    static MovableElement* createButton(wxFrame* parent,
                                        const wxWindowID id,
                                        const std::string& label,
                                        const wxPoint& pos,
                                        const wxSize& size)
    {
        return new MovableElement(parent, id, label, pos, size);
    }*/

    // Slider
    MovableElement(wxFrame* parent,
                   const wxWindowID id,
                   const int init_value,
                   const int min_value,
                   const int max_value,
                   const wxPoint& pos,
                   const wxSize& size)
        : C(parent, id, init_value, min_value, max_value, pos, size)
    {
        control_pressed_at_mouse_down = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // Button, Checkbox, RadioButton, ComboBox
    MovableElement(
        wxFrame* parent, const wxWindowID id, const std::string& label, const wxPoint& pos, const wxSize& size)
        : C(parent, id, label, pos, size)
    {
        control_pressed_at_mouse_down = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // ListBox
    MovableElement(wxFrame* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size)
        : C(parent, id, pos, size)
    {
        control_pressed_at_mouse_down = false;

        this->Bind(wxEVT_MOTION, &MovableElement<C>::mouseMovedOverItem, this);
        this->Bind(wxEVT_LEFT_DOWN, &MovableElement<C>::mouseLeftPressed, this);
        this->Bind(wxEVT_LEFT_UP, &MovableElement<C>::mouseLeftReleased, this);
    }

    // EditableText
    MovableElement(wxFrame* parent,
                   const wxWindowID id,
                   const std::string& initial_text,
                   const wxPoint& pos,
                   const wxSize& size,
                   const long style)
        : C(parent, id, initial_text, pos, size, style)
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
            pos_at_press_ = this->GetPosition();
            mouse_pos_at_press_ = event.GetPosition() + pos_at_press_;
            previous_mouse_pos_ = event.GetPosition() + pos_at_press_;
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

private:
    bool control_pressed_at_mouse_down;
    wxPoint pos_at_press_;
    wxPoint mouse_pos_at_press_;
    wxPoint previous_mouse_pos_;
};

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, "", wxPoint(200, 30), wxSize(700, 700))
{
    MovableElement<wxButton>* me_btn =
        new MovableElement<wxButton>(this, wxID_ANY, "Test", wxPoint(200, 0), wxSize(100, 100));
    MovableElement<wxSlider>* me_slider =
        new MovableElement<wxSlider>(this, wxID_ANY, 13, 0, 100, wxPoint(200, 100), wxSize(100, 30));
    MovableElement<wxRadioButton>* me_radiobutton =
        new MovableElement<wxRadioButton>(this, wxID_ANY, "Test", wxPoint(200, 200), wxSize(100, 30));
    MovableElement<wxCheckBox>* me_checkbox =
        new MovableElement<wxCheckBox>(this, wxID_ANY, "Test", wxPoint(200, 300), wxSize(100, 30));
    MovableElement<wxComboBox>* me_combo_box =
        new MovableElement<wxComboBox>(this, wxID_ANY, "Test", wxPoint(200, 400), wxSize(100, 30));
    MovableElement<wxListBox>* me_list_box =
        new MovableElement<wxListBox>(this, wxID_ANY, wxPoint(200, 500), wxSize(100, 30));
    MovableElement<wxTextCtrl>* me_text_ctrl =
        new MovableElement<wxTextCtrl>(this, wxID_ANY, "Test", wxPoint(200, 600), wxSize(100, 30), wxTE_PROCESS_ENTER);

    wxArrayString arr;
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
    Bind(wxEVT_TEXT_ENTER, &MainWindow::textCtrlEnterCallback, this, me_text_ctrl->GetId());

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
