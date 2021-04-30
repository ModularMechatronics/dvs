#include "main_window.h"

#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "math/math.h"

using namespace dvs::internal;

void MainWindow::setupGui()
{
    tab_container = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(600, 600));
    tab_container->SetBackgroundColour(wxColor(203, 111, 33));

    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* big_sizer = new wxBoxSizer(wxVERTICAL);
    big_sizer->Add(button_sizer, 0, wxEXPAND);
    big_sizer->Add(tab_container, 1, wxEXPAND);

    this->SetSizer(big_sizer);

    tabs_view = new wxNotebook(tab_container, wxID_ANY, wxDefaultPosition, wxSize(500, 500), wxNB_MULTILINE);
    tabs_view->Layout();
    tabs_view->SetBackgroundColour(wxColor(1, 174, 254));

    wxBoxSizer* tabs_sizer_v = new wxBoxSizer(wxVERTICAL);
    tabs_sizer_v->Add(tabs_view, 1, wxEXPAND);

    // wxBoxSizer* tabs_sizer_h = new wxBoxSizer(wxHORIZONTAL);
    // tabs_sizer_h->Add(tabs_sizer_v, 1, wxEXPAND);

    tab_container->SetSizer(tabs_sizer_v);

    tabs_ = project_file_.getTabs();

    const wxSize s0 = tab_container->GetSize();

    for(size_t k = 0; k < tabs_.size(); k++)
    {
        const std::string tab_name = tabs_[k].getName();
        wxNotebookPage* tab_element = new wxNotebookPage(tabs_view, -1);
        tab_elements_.push_back(tab_element);
        tabs_view->AddPage(tab_element, tab_name);
        const wxSize tab_size = tab_element->GetSize();

        const std::vector<project_file::Element> elements = tabs_[k].getElements();

        const float tab_dx = static_cast<float>(tab_size.GetWidth()) / static_cast<float>(tabs_[k].num_cells_x);
        const float tab_dy = static_cast<float>(tab_size.GetHeight()) / static_cast<float>(tabs_[k].num_cells_y);

        for(auto e : elements)
        {
            float xpos = e.cell_idx_x * tab_dx;
            float ypos = e.cell_idx_y * tab_dy;

            wxSize gui_element_size(e.width * tab_dx, e.height * tab_dy);
            gui_elements_[e.name] = new GuiElement(tab_element, wxPoint(xpos, ypos), gui_element_size, e.name, e);
        }
    }

    // prototype_view_ = new PrototypeView(tab_container, wxPoint(0, 0), s0);
}

void MainWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    std::cout << "Size changed" << std::endl;
    const wxSize s0 = tab_container->GetSize();

    for(size_t k = 0; k < tabs_.size(); k++)
    {
        wxNotebookPage* tab_element = tab_elements_[k];
        const wxSize tab_size = tab_element->GetSize();
        // it.second->windowSizeChanged(s0, tab);

    }

    // prototype_view_->setSize(s0);

    /*const project_file::Tab tab = project_file_.getTabFromIdx(0);

    for(auto it : gui_elements_)
    {
        it.second->windowSizeChanged(s0, tab);
    }*/
}

void MainWindow::numCellsXInc(wxCommandEvent& event)
{
    (void)event;
    prototype_view_->changeNumCellsX(1);
}

void MainWindow::numCellsXDec(wxCommandEvent& event)
{
    (void)event;
    prototype_view_->changeNumCellsX(-1);
}

void MainWindow::numCellsYInc(wxCommandEvent& event)
{
    (void)event;
    prototype_view_->changeNumCellsY(1);
}

void MainWindow::numCellsYDec(wxCommandEvent& event)
{
    (void)event;
    prototype_view_->changeNumCellsY(-1);
}

void MainWindow::newElement(wxCommandEvent& event)
{
    (void)event;
    prototype_view_->newElement();
}

void MainWindow::deleteElement(wxCommandEvent& event)
{
    (void)event;
    prototype_view_->deleteElement();
}
