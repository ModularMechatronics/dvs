#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "dvs/math/math.h"
#include "main_window.h"

using namespace dvs::internal;

void MainWindow::setupGui()
{
    /*tabs_view = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxSize(500, 500));
    tabs_view->Layout();

    tabs_sizer_v = new wxBoxSizer(wxHORIZONTAL);
    tabs_sizer_v->Add(tabs_view, 1, wxEXPAND);

    wxBoxSizer* big_sizer = new wxBoxSizer(wxVERTICAL);
    big_sizer->Add(tabs_sizer_v, 1, wxEXPAND);

    this->SetSizer(big_sizer);

    Bind(wxEVT_NOTEBOOK_PAGE_CHANGED, &MainWindow::tabChanged, this);

    setupTabs(save_manager_->getCurrentProjectSettings());*/
    setupWindows(save_manager_->getCurrentProjectSettings());
}

void MainWindow::setupTabs(const ProjectSettings& project_settings)
{
    /*gui_elements_ = std::map<std::string, GuiElement*>();
    for (const TabSettings& tab : project_settings.getTabs())
    {
        const std::string tab_name = tab.getName();

        TabView* tab_element = new TabView(
            tabs_view, tab, notification_from_gui_element_key_pressed_, notification_from_gui_element_key_released_);
        tabs_.push_back(tab_element);

        tabs_view->AddPage(dynamic_cast<wxNotebookPage*>(tab_element), tab_name);
        const std::map<std::string, GuiElement*> ges = tab_element->getGuiElements();
        gui_elements_.insert(ges.begin(), ges.end());
    }

    const int current_tab_idx = tabs_view->GetSelection();
    if (current_tab_idx != wxNOT_FOUND)
    {
        current_tab_name_ = tabs_.at(current_tab_idx)->getName();
        current_element_name_ = tabs_.at(current_tab_idx)->getSelectedElementName();
    }
    layout_tools_window_->setCurrentTabName(current_tab_name_);
    layout_tools_window_->setCurrentElementName(current_element_name_);*/
}

void MainWindow::fileModified()
{
    SetLabel(save_manager_->getCurrentFileName() + "*");
    save_manager_->setIsModified();
}

void MainWindow::tabChanged(wxCommandEvent& WXUNUSED(event))
{
    /*for (auto te : tabs_)
    {
        te->resetSelectionForAllChildren();
        te->setFirstElementSelected();
    }

    const int current_tab_idx = tabs_view->GetSelection();
    if (current_tab_idx != wxNOT_FOUND)
    {
        current_tab_name_ = tabs_.at(current_tab_idx)->getName();
        current_element_name_ = tabs_.at(current_tab_idx)->getSelectedElementName();
    }
    layout_tools_window_->setCurrentTabName(current_tab_name_);
    layout_tools_window_->setCurrentElementName(current_element_name_);*/
}

void MainWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);

    /*const wxSize new_size = tabs_view->GetSize();

    for (size_t k = 0; k < tabs_.size(); k++)
    {
        tabs_[k]->setSize(new_size);
    }*/
}

bool MainWindow::elementNameExists(const std::string& element_name) const
{
    bool name_exists = false;
    /*for (auto te : tabs_)
    {
        const std::vector<ElementSettings> element_settings_vec = te->getElementSettingsList();
        if (std::find_if(
                element_settings_vec.begin(), element_settings_vec.end(), [&](const ElementSettings& es) -> bool {
                    return es.name == element_name;
                }) != element_settings_vec.end())
        {
            name_exists = true;
            break;
        }
    }*/

    // if (!name_exists)
    // {
    //     for (auto we_inner : windows_)
    //     {
    //         const std::vector<ElementSettings> element_settings_vec = we_inner->getElementSettingsList();
    //         if (std::find_if(
    //                 element_settings_vec.begin(), element_settings_vec.end(), [&](const ElementSettings& es) -> bool
    //                 {
    //                     return es.name == element_name;
    //                 }) != element_settings_vec.end())
    //         {
    //             name_exists = true;
    //             break;
    //         }
    //     }
    // }

    return name_exists;
}

void MainWindow::changeCurrentElementName(const std::string new_element_name)
{
    // const bool name_exists = elementNameExists(new_element_name);

    if (main_window_last_in_focus_)
    {
        /*const int current_tab_idx = tabs_view->GetSelection();

        if (current_tab_idx != wxNOT_FOUND)
        {
            if (name_exists)
            {
                tabs_.at(current_tab_idx)->setSelectedElementName("_" + new_element_name);
                layout_tools_window_->setCurrentElementName("_" + new_element_name);
            }
            else
            {
                tabs_.at(current_tab_idx)->setSelectedElementName(new_element_name);
            }
        }*/
    }
    else
    {
        // for (auto we : windows_)
        // {
        //     if (we->getName() == current_tab_name_)
        //     {
        //         if (name_exists)
        //         {
        //             we->setSelectedElementName("_" + new_element_name);
        //             layout_tools_window_->setCurrentElementName("_" + new_element_name);
        //         }
        //         else
        //         {
        //             we->setSelectedElementName(new_element_name);
        //         }
        //     }
        // }
    }
    // fileModified();
}

void MainWindow::currentElementSelectionChanged(wxCommandEvent& event)
{
    // const wxString value = event.GetString();

    // layout_tools_window_->setCurrentElementName(std::string(value.mb_str()));
}

void MainWindow::changeCurrentTabName(const std::string& new_tab_name)
{
    if (new_tab_name != "")
    {
        /*if (main_window_last_in_focus_)
        {
            const int current_tab_idx = tabs_view->GetSelection();
            if (current_tab_idx != wxNOT_FOUND)
            {
                std::vector<std::string> existing_tab_names;
                for (size_t k = 0; k < tabs_view->GetPageCount(); k++)
                {
                    const std::string tn = std::string(tabs_view->GetPageText(k).mb_str());
                    existing_tab_names.push_back(tn);
                }

                if (std::find(existing_tab_names.begin(), existing_tab_names.end(), new_tab_name) !=
                    existing_tab_names.end())
                {
                    layout_tools_window_->setCurrentTabName("_" + new_tab_name);
                    tabs_view->SetPageText(current_tab_idx, "_" + new_tab_name);
                    tabs_.at(current_tab_idx)->setName("_" + new_tab_name);
                }
                else
                {
                    wxString value = new_tab_name;
                    tabs_view->SetPageText(current_tab_idx, value);
                    tabs_.at(current_tab_idx)->setName(new_tab_name);
                }
            }
        }
        else
        {*/
        // for (auto we : windows_)
        // {
        //     if (we->getName() == current_tab_name_)
        //     {
        //         we->setName(new_tab_name);
        //         current_tab_name_ = new_tab_name;
        //         layout_tools_window_->setCurrentTabName(new_tab_name);
        //         std::cout << "Modifying window: " << current_tab_name_ << " with new name " << new_tab_name
        //                   << std::endl;
        //     }
        // }
        // // }
        // fileModified();
    }
}

void MainWindow::newNamedElement(const std::string& element_name)
{
    if (main_window_last_in_focus_)
    {
        /*const int current_tab_idx = tabs_view->GetSelection();
        if (current_tab_idx != wxNOT_FOUND)
        {
            tabs_.at(current_tab_idx)->newElement(element_name);
            const std::map<std::string, GuiElement*> new_elements = tabs_.at(current_tab_idx)->getGuiElements();

            for (const auto& q : new_elements)
            {
                if (gui_elements_.count(q.first) == 0)
                {
                    gui_elements_[q.first] = q.second;
                }
            }
        }*/
    }
    else
    {
        // for (auto we : windows_)
        // {
        //     if (we->getName() == current_tab_name_)
        //     {
        //         we->newElement(element_name);
        //         /*const std::map<std::string, GuiElement*> new_elements = we->getGuiElements();

        //         for (const auto& q : new_elements)
        //         {
        //             if (gui_elements_.count(q.first) == 0)
        //             {
        //                 gui_elements_[q.first] = q.second;
        //             }
        //         }*/
        //     }
        // }
    }
    fileModified();
}

void MainWindow::newElement()
{
    if (main_window_last_in_focus_)
    {
        /*const int current_tab_idx = tabs_view->GetSelection();
        if (current_tab_idx != wxNOT_FOUND)
        {
            tabs_.at(current_tab_idx)->resetSelectionForAllChildren();
            tabs_.at(current_tab_idx)->newElement();
            const std::map<std::string, GuiElement*> new_elements = tabs_.at(current_tab_idx)->getGuiElements();

            for (const auto& q : new_elements)
            {
                if (gui_elements_.count(q.first) == 0)
                {
                    gui_elements_[q.first] = q.second;
                    q.second->setSelection();
                }
            }
        }*/
    }
    else
    {
        // for (auto we : windows_)
        // {
        //     if (we->getName() == current_tab_name_)
        //     {
        //         we->resetSelectionForAllChildren();
        //         we->newElement();
        //         /*const std::map<std::string, GuiElement*> new_elements = we->getGuiElements();

        //         for (const auto& q : new_elements)
        //         {
        //             if (gui_elements_.count(q.first) == 0)
        //             {
        //                 gui_elements_[q.first] = q.second;
        //                 q.second->setSelection();
        //             }
        //         }*/
        //     }
        // }
    }
    fileModified();
}

void MainWindow::deleteSelectedElement()
{
    /*if (main_window_last_in_focus_) {}
    else
    {
        for (auto we : windows_)
        {
            if (we->getName() == current_tab_name_)
            {
                const std::map<std::string, GuiElement*> all_elements = we->getGuiElements();

                we->deleteSelectedElement();

                const std::map<std::string, GuiElement*> elements_after_delete = we->getGuiElements();

                for (const auto& q : all_elements)
                {
                    if (elements_after_delete.count(q.first) == 0)
                    {
                        gui_elements_.erase(q.first);
                        break;
                    }
                }
            }
        }
    }
    fileModified();*/
}

bool MainWindow::hasWindowWithName(const std::string& window_name)
{
    for (const auto we : windows_)
    {
        if (we->getName() == window_name)
        {
            return true;
        }
    }
    return false;
}

WindowView* MainWindow::getWindowWithName(const std::string& window_name)
{
    for (const auto we : windows_)
    {
        if (we->getName() == window_name)
        {
            return we;
        }
    }
    return nullptr;
}
