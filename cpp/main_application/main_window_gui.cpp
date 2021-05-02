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

    wxBoxSizer* tabs_sizer_v = new wxBoxSizer(wxVERTICAL);
    tabs_sizer_v->Add(tabs_view, 1, wxEXPAND);

    tab_container->SetSizer(tabs_sizer_v);

    for(const Tab tab : project_file_.getTabs())
    {
        const std::string tab_name = tab.getName();

        TabView* tab_element = new TabView(tabs_view, tab);
        tab_elements_.push_back(tab_element);

        tabs_view->AddPage(dynamic_cast<wxNotebookPage*>(tab_element), tab_name);
    }
}

void MainWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);

    const wxSize new_size = tab_container->GetSize();

    for(size_t k = 0; k < tab_elements_.size(); k++)
    {
        tab_elements_[k]->setSize(new_size);
    }
}

void MainWindow::numCellsXInc(wxCommandEvent& event)
{
    (void)event;
    // prototype_view_->changeNumCellsX(1);
}

void MainWindow::numCellsXDec(wxCommandEvent& event)
{
    (void)event;
    // prototype_view_->changeNumCellsX(-1);
}

void MainWindow::numCellsYInc(wxCommandEvent& event)
{
    (void)event;
    // prototype_view_->changeNumCellsY(1);
}

void MainWindow::numCellsYDec(wxCommandEvent& event)
{
    (void)event;
    // prototype_view_->changeNumCellsY(-1);
}

void MainWindow::newElement(wxCommandEvent& event)
{
    (void)event;
    // prototype_view_->newElement();
}

void MainWindow::deleteElement(wxCommandEvent& event)
{
    (void)event;
    // prototype_view_->deleteElement();
}
