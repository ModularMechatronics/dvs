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

    tabs_view = new wxNotebook(tab_container, wxID_ANY, wxDefaultPosition, wxSize(500, 500));
    tabs_view->Layout();

    Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &MainWindow::tabChanged, this);

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

    const int current_tab_idx = tabs_view->GetSelection();
    if(current_tab_idx != wxNOT_FOUND)
    {
        current_tab_name_ = tab_elements_.at(current_tab_idx)->getName();
    }
    layout_tools_window_->currentTabChanged(current_tab_name_);
}

void MainWindow::tabChanged(wxCommandEvent& event)
{
    const int current_tab_idx = tabs_view->GetSelection();
    if(current_tab_idx != wxNOT_FOUND)
    {
        current_tab_name_ = tab_elements_.at(current_tab_idx)->getName();
    }
    layout_tools_window_->currentTabChanged(current_tab_name_);
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

void MainWindow::changeCurrentElementName(wxCommandEvent& event)
{
    const wxString value = event.GetString();

    std::cout << std::string(value.mb_str()) << std::endl;
}

void MainWindow::changeCurrentTabName(wxCommandEvent& event)
{
    const wxString value = event.GetString();

    if(std::string(value.mb_str()) != "")
    {
        const int current_tab_idx = tabs_view->GetSelection();
        if(current_tab_idx != wxNOT_FOUND)
        {
            tabs_view->SetPageText(current_tab_idx, value);
            tab_elements_.at(current_tab_idx)->setName(std::string(value.mb_str()));

        }
    }
}

void MainWindow::newElement(wxCommandEvent& event)
{
    (void)event;
    const int current_tab_idx = tabs_view->GetSelection();
    if(current_tab_idx != wxNOT_FOUND)
    {
        tab_elements_.at(current_tab_idx)->newElement();
    }
}

void MainWindow::deleteElement(wxCommandEvent& event)
{
    (void)event;
    const int current_tab_idx = tabs_view->GetSelection();
    if(current_tab_idx != wxNOT_FOUND)
    {
        tab_elements_.at(current_tab_idx)->deleteSelectedElement();
    }
}
