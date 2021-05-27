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

    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* big_sizer = new wxBoxSizer(wxVERTICAL);
    big_sizer->Add(button_sizer, 0, wxEXPAND);
    big_sizer->Add(tab_container, 1, wxEXPAND);

    this->SetSizer(big_sizer);

    tabs_view = new wxNotebook(tab_container, wxID_ANY, wxDefaultPosition, wxSize(500, 500));
    tabs_view->Layout();

    Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &MainWindow::tabChanged, this);

    tabs_sizer_v = new wxBoxSizer(wxVERTICAL);
    tabs_sizer_v->Add(tabs_view, 1, wxEXPAND);

    tab_container->SetSizer(tabs_sizer_v);

    setupTabs(save_manager_->getCurrentProjectFile());
    setupWindows(save_manager_->getCurrentProjectFile());
}

void MainWindow::setupTabs(const ProjectFile& project_file)
{
    gui_elements_ = std::map<std::string, GuiElement*>();
    for(const TabSettings tab : project_file.getTabs())
    {
        const std::string tab_name = tab.getName();

        TabView* tab_element = new TabView(tabs_view, tab);
        tab_elements_.push_back(tab_element);

        tabs_view->AddPage(dynamic_cast<wxNotebookPage*>(tab_element), tab_name);
        const std::map<std::string, GuiElement*> ges = tab_element->getGuiElements();
        gui_elements_.insert(ges.begin(), ges.end());
    }

    const int current_tab_idx = tabs_view->GetSelection();
    if(current_tab_idx != wxNOT_FOUND)
    {
        current_tab_name_ = tab_elements_.at(current_tab_idx)->getName();
        current_element_name_ = tab_elements_.at(current_tab_idx)->getSelectedElementName();
    }
    layout_tools_window_->currentTabChanged(current_tab_name_);
    layout_tools_window_->currentElementSelectionChanged(current_element_name_);
}

void MainWindow::fileModified()
{
    SetLabel(save_manager_->getCurrentFileName() + "*");
    save_manager_->setIsModified();
}

void MainWindow::tabChanged(wxCommandEvent& event)
{
    for(auto te : tab_elements_)
    {
        te->resetSelectionForAllChildren();
        te->setFirstElementSelected();
    }

    const int current_tab_idx = tabs_view->GetSelection();
    if(current_tab_idx != wxNOT_FOUND)
    {
        current_tab_name_ = tab_elements_.at(current_tab_idx)->getName();
        current_element_name_ = tab_elements_.at(current_tab_idx)->getSelectedElementName();
    }
    layout_tools_window_->currentTabChanged(current_tab_name_);
    layout_tools_window_->currentElementSelectionChanged(current_element_name_);
}

void MainWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);

    const wxSize new_size = tab_container->GetSize();

    for(size_t k = 0; k < tab_elements_.size(); k++)
    {
        tab_elements_[k]->setSize(new_size);
    }

    background_panel_->resize(this->GetSize());
}

void MainWindow::changeCurrentElementName(wxCommandEvent& event)
{
    const wxString value = event.GetString();
    const int current_tab_idx = tabs_view->GetSelection();
    if(current_tab_idx != wxNOT_FOUND)
    {
        tab_elements_.at(current_tab_idx)->setSelectedElementName(std::string(value.mb_str()));
    }
    fileModified();
}

void MainWindow::currentElementSelectionChanged(wxCommandEvent& event)
{
    const wxString value = event.GetString();

    layout_tools_window_->currentElementSelectionChanged(std::string(value.mb_str()));
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
    fileModified();
}

void MainWindow::newElement(wxCommandEvent& event)
{
    (void)event;
    if(main_window_last_in_focus_)
    {
        const int current_tab_idx = tabs_view->GetSelection();
        if(current_tab_idx != wxNOT_FOUND)
        {
            tab_elements_.at(current_tab_idx)->newElement();
            const std::map<std::string, GuiElement*> new_elements = tab_elements_.at(current_tab_idx)->getGuiElements();

            for(const auto& q : new_elements)
            {
                if(gui_elements_.count(q.first) == 0)
                {
                    gui_elements_[q.first] = q.second;
                }
            }
        }
    }
    else
    {
        for(auto we : windows_)
        {
            if(we->getName() == current_tab_name_)
            {
                we->newElement();
                const std::map<std::string, GuiElement*> new_elements = we->getGuiElements();

                for(const auto& q : new_elements)
                {
                    if(gui_elements_.count(q.first) == 0)
                    {
                        gui_elements_[q.first] = q.second;
                    }
                }
            }
        }
    }
    fileModified();
}

void MainWindow::deleteElement(wxCommandEvent& event)
{
    (void)event;
    if(main_window_last_in_focus_)
    {
        const int current_tab_idx = tabs_view->GetSelection();
        if(current_tab_idx != wxNOT_FOUND)
        {
            const std::map<std::string, GuiElement*> all_elements = tab_elements_.at(current_tab_idx)->getGuiElements();

            tab_elements_.at(current_tab_idx)->deleteSelectedElement();

            const std::map<std::string, GuiElement*> elements_after_delete = tab_elements_.at(current_tab_idx)->getGuiElements();

            for(const auto& q : all_elements)
            {
                if(elements_after_delete.count(q.first) == 0)
                {
                    gui_elements_.erase(q.first);
                    break;
                }
            }
        }
    }
    else
    {
        for(auto we : windows_)
        {
            if(we->getName() == current_tab_name_)
            {
                const std::map<std::string, GuiElement*> all_elements = we->getGuiElements();

                we->deleteSelectedElement();

                const std::map<std::string, GuiElement*> elements_after_delete = we->getGuiElements();

                for(const auto& q : all_elements)
                {
                    if(elements_after_delete.count(q.first) == 0)
                    {
                        gui_elements_.erase(q.first);
                        break;
                    }
                }
            }
        }
    }
    fileModified();
}
