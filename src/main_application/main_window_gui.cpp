#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "dvs/math/math.h"
#include "main_window.h"

using namespace dvs::internal;

void MainWindow::setupGui()
{
    setupWindows(save_manager_->getCurrentProjectSettings());
}

void MainWindow::fileModified()
{
    save_manager_->setIsModified();
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

    /*if (main_window_last_in_focus_)
    {
        const int current_tab_idx = tabs_view->GetSelection();

        if (current_tab_idx != wxNOT_FOUND)
        {
            if (name_exists)
            {
                tabs_.at(current_tab_idx)->setSelectedElementName("_" + new_element_name);
            }
            else
            {
                tabs_.at(current_tab_idx)->setSelectedElementName(new_element_name);
            }
        }
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
        //         }
        //         else
        //         {
        //             we->setSelectedElementName(new_element_name);
        //         }
        //     }
        // }
    }*/
    // fileModified();
}

void MainWindow::newNamedElement(const std::string& element_name)
{
    /*if (main_window_last_in_focus_)
    {
        const int current_tab_idx = tabs_view->GetSelection();
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
        }
    }
    else
    {
        // for (auto we : windows_)
        // {
        //     if (we->getName() == current_tab_name_)
        //     {
        //         we->newElement(element_name);
        //         const std::map<std::string, GuiElement*> new_elements = we->getGuiElements();

        //         for (const auto& q : new_elements)
        //         {
        //             if (gui_elements_.count(q.first) == 0)
        //             {
        //                 gui_elements_[q.first] = q.second;
        //             }
        //         }
        //     }
        // }
    }
    fileModified();*/
}

void MainWindow::newElement()
{
    /*if (main_window_last_in_focus_)
    {
        const int current_tab_idx = tabs_view->GetSelection();
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
        }
    }
    else
    {
        // for (auto we : windows_)
        // {
        //     if (we->getName() == current_tab_name_)
        //     {
        //         we->resetSelectionForAllChildren();
        //         we->newElement();
        //         const std::map<std::string, GuiElement*> new_elements = we->getGuiElements();

        //         for (const auto& q : new_elements)
        //         {
        //             if (gui_elements_.count(q.first) == 0)
        //             {
        //                 gui_elements_[q.first] = q.second;
        //                 q.second->setSelection();
        //             }
        //         }
        //     }
        // }
    }
    fileModified();*/
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
