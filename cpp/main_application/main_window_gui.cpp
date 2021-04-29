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

    prototype_view_ = new PrototypeView(tab_container, wxPoint(0, 0), s0);

    /*std::vector<project_file::Element> elements = tab.getElements();

    for(auto e : elements)
    {
        xpos = tab.margin_left_right + e.col * inc_x + tab.spacing_cols;
        ypos = tab.margin_top_bottom + e.row * inc_y + tab.spacing_rows;
        wxSize sq(e.width * inc_x - tab.spacing_cols * 2, e.height * inc_y - tab.spacing_rows * 2);
        gui_elements_[e.name] = new GuiElement(tab_container, wxPoint(xpos, ypos), sq, e.name, e);
    }*/
}

void MainWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    std::cout << "Size changed" << std::endl;
    wxSize s0 = tab_container->GetSize();

    prototype_view_->setPosAndSize(wxPoint(0, 0), s0);

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
