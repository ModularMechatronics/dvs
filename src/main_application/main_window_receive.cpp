#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "dvs/math/math.h"
#include "main_window.h"
#include "window_view.h"

using namespace dvs::internal;

void MainWindow::setCurrentElement(const CommunicationHeader& hdr)
{
    const CommunicationHeaderObject elem_obj = hdr.get(CommunicationHeaderObjectType::ELEMENT_NAME);
    const properties::Name elem_name = elem_obj.as<properties::Name>();
    const std::string element_name_str = elem_name.data;

    if (element_name_str.length() == 0)
    {
        throw std::runtime_error("Name string had zero length!");
    }

    for (auto we : windows_)
    {
        current_gui_element_ = we->getGuiElement(element_name_str);
        if (current_gui_element_ != nullptr)
        {
            break;
        }
    }

    if (!currentGuiElementSet())
    {
        // TODO: Create new window with gui element or something
    }
    /*if (gui_elements_.count(element_name_str) > 0)
    {
        current_gui_element_ = gui_elements_[element_name_str];
        // current_gui_element_set_ = true;
    }
    else
    {
        // TODO: Handle this
        // newNamedElement(element_name_str);
    }*/
}

void MainWindow::setWaitForFlush()
{
    if (currentGuiElementSet())
    {
        current_gui_element_->waitForFlush();
    }
}

void MainWindow::createNewElement(const CommunicationHeader& hdr)
{
    /*const CommunicationHeaderObject elem_obj = hdr.get(CommunicationHeaderObjectType::ELEMENT_NAME);
    const properties::Name elem_name = elem_obj.as<properties::Name>();
    const std::string element_name_str = elem_name.data;

    if (gui_elements_.count(element_name_str) > 0)
    {
        current_gui_element_ = gui_elements_[element_name_str];
        // current_gui_element_set_ = true;
    }
    else
    {
        const CommunicationHeaderObject parent_name_obj = hdr.get(CommunicationHeaderObjectType::PARENT_NAME);
        std::string parent_name = parent_name_obj.as<properties::Name>().data;

        const CommunicationHeaderObject parent_type_obj = hdr.get(CommunicationHeaderObjectType::PARENT_TYPE);
        const dvs::ElementParent parent_type = parent_type_obj.as<dvs::ElementParent>();

        if (parent_name == "#DEFAULTNAME#")
        {
            parent_name = "New figure " + std::to_string(current_window_num_);
            current_window_num_++;
        }

        // if (parent_type == ElementParent::TAB)
        // {
        //     if (!hasTabWithName(parent_name))
        //     {
        //         addNewTab(parent_name);
        //     }
        //     main_window_last_in_focus_ = true;
        // }
        // else if (parent_type == ElementParent::WINDOW)
        // {
        //     if (!hasWindowWithName(parent_name))
        //     {
        //         addNewWindow(parent_name);
        //     }
        //     else
        //     {
        //         // TODO: Set focus of the window with this name
        //         // TODO2: No, don't set focus of window, will be annoying
        //     }
        // }

        newNamedElement(element_name_str);
    }*/
}

bool isGuiElementFunction(const Function fcn)
{
    return (fcn == Function::PLANE_XY) || (fcn == Function::PLANE_XZ) || (fcn == Function::PLANE_YZ) ||
           (fcn == Function::GRID_ON) || (fcn == Function::GRID_OFF) || (fcn == Function::PLOT2) ||
           (fcn == Function::PLOT3) || (fcn == Function::DRAW_TRIANGLES_3D) || (fcn == Function::SCATTER2) ||
           (fcn == Function::SCATTER3) || (fcn == Function::DRAW_LINE_BETWEEN_POINTS_3D) ||
           (fcn == Function::POLYGON_FROM_4_POINTS) || (fcn == Function::DRAW_MESH) ||
           (fcn == Function::DRAW_MESH_SEPARATE_VECTORS) || (fcn == Function::DRAW_LINE3D) ||
           (fcn == Function::CLEAR) || (fcn == Function::HOLD_ON) || (fcn == Function::HOLD_OFF) ||
           (fcn == Function::POSITION) || (fcn == Function::SURF) || (fcn == Function::IM_SHOW) ||
           (fcn == Function::AXES_2D) || (fcn == Function::AXES_3D) || (fcn == Function::VIEW) ||
           (fcn == Function::SOFT_CLEAR) || (fcn == Function::STAIRS) || (fcn == Function::DRAW_TILES) ||
           (fcn == Function::DRAW_ARROW) || (fcn == Function::PLOT_COLLECTION2) || (fcn == Function::QUIVER) ||
           (fcn == Function::STEM) || (fcn == Function::FAST_PLOT2) || (fcn == Function::FAST_PLOT3) ||
           (fcn == Function::LINE_COLLECTION2) || (fcn == Function::LINE_COLLECTION3) ||
           (fcn == Function::PLOT_COLLECTION3) || (fcn == Function::GLOBAL_ILLUMINATION) ||
           (fcn == Function::REAL_TIME_PLOT) || (fcn == Function::HEADER_EXTENSION);
}

bool MainWindow::currentGuiElementSet() const
{
    return current_gui_element_ != nullptr;
}

void MainWindow::receiveData()
{
    std::unique_ptr<const ReceivedData> received_data = udp_server_->getReceivedData();
    if (received_data)
    {
        const CommunicationHeader& hdr = received_data->getCommunicationHeader();

        const Function fcn = hdr.getFunction();

        if (isGuiElementFunction(fcn))
        {
            if (currentGuiElementSet())
            {
                current_gui_element_->addData(std::move(received_data), hdr);
            }
            else
            {
                std::cout << "No element set!" << std::endl;
            }
        }
        else
        {
            switch (fcn)
            {
                case Function::SET_CURRENT_ELEMENT:
                    setCurrentElement(hdr);
                    break;

                case Function::WAIT_FOR_FLUSH:
                    setWaitForFlush();
                    break;

                case Function::FLUSH_ELEMENT:
                    if (currentGuiElementSet())
                    {
                        current_gui_element_->refresh();
                    }
                    break;

                case Function::CREATE_NEW_ELEMENT:
                    // createNewElement(hdr);
                    // TODO
                    std::cout << "Not implemented yet!" << std::endl;
                    break;

                case Function::SHOW_LEGEND:
                    if (currentGuiElementSet())
                    {
                        current_gui_element_->showLegend(true);
                    }
                    else
                    {
                        std::cout << "No element set!" << std::endl;
                    }

                    break;
                default:
                    std::cout << "Got default" << std::endl;
            }
        }
    }
}
