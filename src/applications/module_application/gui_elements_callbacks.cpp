#include <mach-o/dyld.h>
#include <unistd.h>
#include <wx/wfstream.h>
#include <wx/wxprec.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "main_window.h"

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
