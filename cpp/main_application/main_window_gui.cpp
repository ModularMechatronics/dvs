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
    tabs_view->AddPage( new wxNotebookPage(tabs_view, -1), L"TEST 4");
}

void MainWindow::setupGui()
{

    wxPanel* tab_container = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(600, 600));
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

    tabs_view = new wxNotebook(tab_container, wxID_ANY, wxDefaultPosition, wxSize(500, 500), wxNB_MULTILINE);
    tabs_view->Layout();
    tabs_view->SetBackgroundColour(wxColor(1, 174, 254));

    wxBoxSizer* tabs_sizer_v = new wxBoxSizer(wxVERTICAL);
    tabs_sizer_v->Add(tabs_view, 1, wxEXPAND);

    wxBoxSizer* tabs_sizer_h = new wxBoxSizer(wxHORIZONTAL);
    tabs_sizer_h->Add(tabs_sizer_v, 1, wxEXPAND);

    tab_container->SetSizer(tabs_sizer_h);

    const int nx = 13;
    const int ny = 9;
    int dx = initial_width_ / nx;
    int dy = initial_height_ / ny; 
    int xpos = 0;
    int ypos = 0;

    // x: 1500, y: 700
    /*for(int x = 0; x < nx; x++)
    {
        ypos = 0;
        for(int y = 0; y < ny; y++)
        {
            const wxPoint pos(xpos, ypos);
            const wxSize size(dx, dy);
            
            ResizablePanel* panel = new ResizablePanel(this, pos, size);
            panel->setBackgroundColour(wxColor(((10 * y) % 255), ((10 * x) % 255), (((x + y) * 10) * x) % 255));
            panels_.push_back(panel);
            ypos += dy;
        }
        xpos += dx;
    }*/
    /*std::vector<int> x = {0, 350, 2 * 350, 3 * 350};
    int idx = 0;
    for(auto e : project_file_.getElements())
    {
        const std::string name = e->getName();
        gui_elements_[name] = new GuiElement(this, wxPoint(x[idx], 0), wxSize(350, 350), name);
        idx++;
    }*/
}
