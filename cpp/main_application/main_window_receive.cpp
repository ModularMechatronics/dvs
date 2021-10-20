#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "dvs/math/math.h"
#include "main_window.h"

using namespace dvs::internal;

void MainWindow::setCurrentElement(const FunctionHeader& hdr)
{
    const FunctionHeaderObject elem_obj = hdr.get(FunctionHeaderObjectType::ELEMENT_NAME);
    const properties::Name elem_name = elem_obj.as<properties::Name>();
    const std::string element_name_str = elem_name.data;

    if (element_name_str.length() == 0)
    {
        // TODO: Throw exception
        std::cout << "Name string had zero length!" << std::endl;
        return;
    }

    if (gui_elements_.count(element_name_str) > 0)
    {
        current_gui_element_ = gui_elements_[element_name_str];
        current_gui_element_set_ = true;
    }
    else
    {
        if (tabs_.size() == 0)
        {
            const std::string tab_name = "new-tab-" + std::to_string(current_tab_num_);
            current_tab_num_++;
            addNewTab(tab_name);
        }

        newNamedElement(element_name_str);
    }
}

void MainWindow::createNewElement(const FunctionHeader& hdr)
{
    const FunctionHeaderObject elem_obj = hdr.get(FunctionHeaderObjectType::ELEMENT_NAME);
    const properties::Name elem_name = elem_obj.as<properties::Name>();
    const std::string element_name_str = elem_name.data;

    if (gui_elements_.count(element_name_str) > 0)
    {
        current_gui_element_ = gui_elements_[element_name_str];
        current_gui_element_set_ = true;
    }
    else
    {
        const FunctionHeaderObject parent_name_obj = hdr.get(FunctionHeaderObjectType::PARENT_NAME);
        std::string parent_name = parent_name_obj.as<properties::Name>().data;

        const FunctionHeaderObject parent_type_obj = hdr.get(FunctionHeaderObjectType::PARENT_TYPE);
        const dvs::ElementParent parent_type = parent_type_obj.as<dvs::ElementParent>();

        if (parent_name == "#DEFAULTNAME#")
        {
            parent_name = "New figure " + std::to_string(current_tab_num_);
            current_tab_num_++;
        }

        if (parent_type == ElementParent::TAB)
        {
            if (!hasTabWithName(parent_name))
            {
                addNewTab(parent_name);
            }
            main_window_last_in_focus_ = true;
        }
        else if (parent_type == ElementParent::WINDOW)
        {
            if (!hasWindowWithName(parent_name))
            {
                addNewWindow(parent_name);
            }
            else
            {
                // TODO: Set focus of the window with this name
                // TODO2: No, don't set focus of window, will be annoying
            }
        }

        newNamedElement(element_name_str);
    }
}

bool isGuiElementFunction(const Function fcn)
{
    return (fcn == Function::PLANE_XY) || (fcn == Function::PLANE_XZ) || (fcn == Function::PLANE_YZ) ||
           (fcn == Function::GRID_ON) || (fcn == Function::GRID_OFF) || (fcn == Function::PLOT2) ||
           (fcn == Function::PLOT3) || (fcn == Function::DRAW_TRIANGLES_3D) || (fcn == Function::SCATTER2) ||
           (fcn == Function::SCATTER3) || (fcn == Function::DRAW_LINE_BETWEEN_POINTS_3D) ||
           (fcn == Function::POLYGON_FROM_4_POINTS) || (fcn == Function::DRAW_MESH) || (fcn == Function::DRAW_LINE3D) ||
           (fcn == Function::CLEAR) || (fcn == Function::HOLD_ON) || (fcn == Function::HOLD_OFF) ||
           (fcn == Function::POSITION) || (fcn == Function::SURF) || (fcn == Function::IM_SHOW) ||
           (fcn == Function::AXES_2D) || (fcn == Function::AXES_3D) || (fcn == Function::VIEW) ||
           (fcn == Function::SOFT_CLEAR) || (fcn == Function::STAIRS) || (fcn == Function::DRAW_TILES) ||
           (fcn == Function::DRAW_ARROW) || (fcn == Function::PLOT2_COLLECTION);
}

void MainWindow::receiveData()
{
    std::unique_ptr<const ReceivedData> received_data = udp_server_->getReceivedData();
    if (received_data)
    {
        const FunctionHeader hdr = received_data->getFunctionHeader();

        const FunctionHeaderObject fcn_obj = hdr.getObjectAtIdx(0);
        if (fcn_obj.type != FunctionHeaderObjectType::FUNCTION)
        {
            throw std::runtime_error("Function object was not at element 0 in function header!");
        }

        const Function fcn = fcn_obj.as<Function>();

        if (isGuiElementFunction(fcn))
        {
            if (current_gui_element_set_)
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
                case Function::CREATE_NEW_ELEMENT:
                    createNewElement(hdr);

                    break;
                default:
                    std::cout << "Got default" << std::endl;
            }
        }
    }
}
