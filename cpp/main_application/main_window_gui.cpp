#include "main_window.h"

#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "math/math.h"

using namespace dvs::internal;


void MainWindow::addNewTab(wxCommandEvent & event)
{
    std::cout << "Add new tab" << std::endl;
    // tabs_view->AddPage( new wxNotebookPage(tabs_view, -1), L"TEST 4");
}

void MainWindow::setupGui()
{
    tab_container = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(600, 600));
    tab_container->SetBackgroundColour(wxColor(203, 111, 33));

    wxButton* new_tab_button = new wxButton(this, wxID_ANY, "New tab", wxPoint(0, 0));
    wxButton* delete_tab_button = new wxButton(this, wxID_ANY, "Delete current tab", wxPoint(0, 0));
    wxButton* edit_tab_button = new wxButton(this, wxID_ANY, "Edit tab", wxPoint(0, 0));

    new_tab_button->Bind(wxEVT_BUTTON, &MainWindow::addNewTab, this);

    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);
    button_sizer->Add(edit_tab_button, 0, 0, 0);
    button_sizer->Add(delete_tab_button, 0, 0, 0);
    button_sizer->Add(new_tab_button, 0, 0, 0);

    wxBoxSizer* big_sizer = new wxBoxSizer(wxVERTICAL);
    big_sizer->Add(button_sizer, 0, wxEXPAND);
    big_sizer->Add(tab_container, 1, wxEXPAND);

    this->SetSizer(big_sizer);

    /*tabs_view = new wxNotebook(tab_container, wxID_ANY, wxDefaultPosition, wxSize(500, 500), wxNB_MULTILINE);
    tabs_view->Layout();
    tabs_view->SetBackgroundColour(wxColor(1, 174, 254));

    wxBoxSizer* tabs_sizer_v = new wxBoxSizer(wxVERTICAL);
    tabs_sizer_v->Add(tabs_view, 1, wxEXPAND);

    wxBoxSizer* tabs_sizer_h = new wxBoxSizer(wxHORIZONTAL);
    tabs_sizer_h->Add(tabs_sizer_v, 1, wxEXPAND);

    tab_container->SetSizer(tabs_sizer_h);*/

    project_file::Tab tab = project_file_.getTabFromIdx(0);

    wxSize s0 = tab_container->GetSize();

    const int nx = tab.num_cols;
    const int ny = tab.num_rows;

    float dx = static_cast<float>(s0.GetWidth() - tab.margin_left_right * 2) / static_cast<float>(nx) - tab.spacing_cols * 2;
    float dy = static_cast<float>(s0.GetHeight() - tab.margin_top_bottom * 2) / static_cast<float>(ny) - tab.spacing_rows * 2;
    float inc_x = static_cast<float>(s0.GetWidth() - tab.margin_left_right * 2) / static_cast<float>(nx);
    float inc_y = static_cast<float>(s0.GetHeight() - tab.margin_top_bottom * 2) / static_cast<float>(ny);
    float xpos = tab.margin_left_right + tab.spacing_cols;
    float ypos = tab.margin_top_bottom + tab.spacing_rows;

    const wxSize size(dx, dy);

    for(int x = 0; x < nx; x++)
    {
        for(int y = 0; y < ny; y++)
        {
            xpos = tab.margin_left_right + x * inc_x + tab.spacing_cols;
            ypos = tab.margin_top_bottom + y * inc_y + tab.spacing_rows;
            const wxPoint pos(xpos, ypos);
            
            // ResizablePanel* panel = new ResizablePanel(tab_container, pos, size);
            // panel->setBackgroundColour(wxColor(((10 * y) % 255), ((10 * x) % 255), (((x + y) * 10) * x) % 255));
            // panels_.push_back(panel);
        }
    }
    std::vector<project_file::Element> elements = tab.getElements();

    for(auto e : elements)
    {
        xpos = tab.margin_left_right + e.col * inc_x + tab.spacing_cols;
        ypos = tab.margin_top_bottom + e.row * inc_y + tab.spacing_rows;
        wxSize sq(e.width * inc_x - tab.spacing_cols * 2, e.height * inc_y - tab.spacing_rows * 2);
        gui_elements_[e.name] = new GuiElement(tab_container, wxPoint(xpos, ypos), sq, e.name, e);
    }
}

void MainWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    std::cout << "Size changed" << std::endl;
    wxSize s0 = tab_container->GetSize();

    std::cout << s0.GetWidth() << ", " << s0.GetHeight() << std::endl;
    const project_file::Tab tab = project_file_.getTabFromIdx(0);

    const int nx = tab.num_cols;
    const int ny = tab.num_rows;

    float dx = static_cast<float>(s0.GetWidth() - tab.margin_left_right * 2) / static_cast<float>(nx) - tab.spacing_cols * 2;
    float dy = static_cast<float>(s0.GetHeight() - tab.margin_top_bottom * 2) / static_cast<float>(ny) - tab.spacing_rows * 2;
    float inc_x = static_cast<float>(s0.GetWidth() - tab.margin_left_right * 2) / static_cast<float>(nx);
    float inc_y = static_cast<float>(s0.GetHeight() - tab.margin_top_bottom * 2) / static_cast<float>(ny);
    float xpos = tab.margin_left_right + tab.spacing_cols;
    float ypos = tab.margin_top_bottom + tab.spacing_rows;

    const wxSize size(dx, dy);

    for(int x = 0; x < nx; x++)
    {
        for(int y = 0; y < ny; y++)
        {
            xpos = tab.margin_left_right + x * inc_x + tab.spacing_cols;
            ypos = tab.margin_top_bottom + y * inc_y + tab.spacing_rows;
            const int idx = x + nx * y;
            const wxPoint pos(xpos, ypos);

            // panels_.at(idx)->setSize(pos, size);
        }
    }

    for(auto it : gui_elements_)
    {
        it.second->windowSizeChanged(s0, tab);
    }

    /*wxSize new_size = event.GetSize();
    const int nx = 13;
    const int ny = 9;
    float dx = static_cast<float>(new_size.GetWidth()) / static_cast<float>(nx);
    float dy = static_cast<float>(new_size.GetHeight() - 30) / static_cast<float>(ny);
    float xpos = 0;
    float ypos = 0;

    // x: 1500, y: 700
    for(int x = 0; x < nx; x++)
    {
        ypos = 0;
        for(int y = 0; y < ny; y++)
        {
            const int idx = x + nx * y;
            const wxPoint pos(std::round(xpos), std::round(ypos));
            const wxSize size(dx, dy);
            panels_.at(idx)->setSize(pos, size);
            
            // ResizablePanel* panel = new ResizablePanel(this, pos, size);
            // panel->setBackgroundColour(wxColor(100, 100, (((x + y) * 10) * x) % 255));
            // panels_.push_back(panel);
            ypos += dy;
        }
        xpos += dx;
    }*/
}
