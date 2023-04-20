#include "main_window.h"

#include <mach-o/dyld.h>
#include <unistd.h>
#include <wx/wfstream.h>
#include <wx/wxprec.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

wxButton* btn;
wxSlider* slider;
wxRadioButton* radio;
wxCheckBox* checkbox;
wxComboBox* combo;
wxListBox* list;
wxTextCtrl* text;

void MainWindow::guiElementCallbackHandler(wxCommandEvent& event)
{
    std::cout << "Event: " << event.GetId() << std::endl;

    const int id = event.GetId();

    if (id == btn->GetId())
    {
        std::cout << "btn: " << event.GetEventType() << std::endl;
    }
    if (id == slider->GetId())
    {
        std::cout << "slider: " << event.GetEventType() << std::endl;
    }
    if (id == radio->GetId())
    {
        const bool is_checked = event.IsChecked();
        std::cout << "radio: " << is_checked << std::endl;
    }
    if (id == checkbox->GetId())
    {
        const bool is_checked = event.IsChecked();
        std::cout << "checkbox: " << is_checked << std::endl;
    }
    if (id == combo->GetId())
    {
        std::cout << "combo!" << std::endl;
    }
    if (id == list->GetId())
    {
        wxString t = event.GetString();

        std::cout << "list: " << t << ", idx: " << event.GetInt() << std::endl;
    }
    if (id == text->GetId())
    {
        wxString t = event.GetString();

        std::cout << "text: " << t << std::endl;
    }

    const wxEventType evt_type = event.GetEventType();

    if (evt_type == wxEVT_BUTTON)
    {
        std::cout << "wxEVT_BUTTON" << std::endl;
    }
    else if (evt_type == wxEVT_SLIDER)
    {
        std::cout << "wxEVT_SLIDER" << std::endl;
    }
    else if (evt_type == wxEVT_RADIOBUTTON)
    {
        std::cout << "wxEVT_RADIOBUTTON" << std::endl;
    }
    else if (evt_type == wxEVT_CHECKBOX)
    {
        std::cout << "wxEVT_CHECKBOX" << std::endl;
    }
    else if (evt_type == wxEVT_LISTBOX)
    {
        std::cout << "wxEVT_LISTBOX" << std::endl;
    }
    else if (evt_type == wxEVT_COMBOBOX)
    {
        std::cout << "wxEVT_COMBOBOX" << std::endl;
    }
    else if (evt_type == wxEVT_TEXT)
    {
        std::cout << "wxEVT_TEXT" << std::endl;
    }
    else
    {
        std::cout << "Unknown event type: " << evt_type << std::endl;
    }
}

void MainWindow::sliderHandler(wxCommandEvent& event)
{
    std::cout << "Scroll Event: " << event.GetId() << std::endl;
}

MainWindow::MainWindow() : wxFrame(NULL, wxID_ANY, "", wxPoint(200, 30), wxSize(700, 700))
{
    //
    btn = new wxButton(this, wxID_ANY, "Test", wxPoint(10, 0), wxSize(100, 100));
    slider = new wxSlider(this, wxID_ANY, 13, 0, 100, wxPoint(10, 100), wxSize(100, 30));
    radio = new wxRadioButton(this, wxID_ANY, "Test", wxPoint(10, 200), wxSize(100, 30));
    checkbox = new wxCheckBox(this, wxID_ANY, "Test", wxPoint(10, 300), wxSize(100, 30));
    combo = new wxComboBox(this, wxID_ANY, "Test", wxPoint(10, 400), wxSize(100, 30));
    list = new wxListBox(this, wxID_ANY, wxPoint(10, 500), wxSize(100, 30));
    text = new wxTextCtrl(this, wxID_ANY, "Test", wxPoint(10, 600), wxSize(100, 30));

    wxArrayString arr;
    arr.Add("Test1");
    arr.Add("Test2");
    arr.Add("Test3");
    list->InsertItems(arr, 0);

    combo->Append("combo Test1");
    combo->Append("combo Test2");
    combo->Append("combo Test3");

    Bind(wxEVT_BUTTON, &MainWindow::guiElementCallbackHandler, this, btn->GetId());
    Bind(wxEVT_SLIDER, &MainWindow::guiElementCallbackHandler, this, slider->GetId());
    Bind(wxEVT_RADIOBUTTON, &MainWindow::guiElementCallbackHandler, this, radio->GetId());
    Bind(wxEVT_CHECKBOX, &MainWindow::guiElementCallbackHandler, this, checkbox->GetId());
    Bind(wxEVT_LISTBOX, &MainWindow::guiElementCallbackHandler, this, list->GetId());
    Bind(wxEVT_COMBOBOX, &MainWindow::guiElementCallbackHandler, this, combo->GetId());
    Bind(wxEVT_TEXT, &MainWindow::guiElementCallbackHandler, this, text->GetId());
    // Bind(wxEVT_TEXT_ENTER, &MainWindow::guiElementCallbackHandler, this, text->GetId());

    // std::cout << "btn id: " << btn->GetId() << std::endl;
    // std::cout << "slider id: " << slider->GetId() << std::endl;

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
