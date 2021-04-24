#include "main_window.h"

#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "math/math.h"

using namespace dvs::internal;


void MainWindow::setCurrentElement(const FunctionHeader& hdr)
{
    const FunctionHeaderObject elem_obj = hdr.getObjectFromType(FunctionHeaderObjectType::ELEMENT_NAME);
    const properties::Name elem_name = elem_obj.getAs<properties::Name>();
    const std::string element_name_str = elem_name.data;

    if(gui_elements_.count(element_name_str) > 0)
    {
        std::cout << "Current element name: " << element_name_str << std::endl;
        current_gui_element_ = gui_elements_[element_name_str];
        current_gui_element_set_ = true;
    }
    else
    {
        std::cout << "I don't have name: " << element_name_str << std::endl;
    }
}

bool isGuiElementFunction(const Function fcn)
{
    return (fcn == Function::PLANE_XY) ||
           (fcn == Function::PLANE_XZ) ||
           (fcn == Function::PLANE_YZ) ||
           (fcn == Function::GRID_ON) ||
           (fcn == Function::GRID_OFF) ||
           (fcn == Function::PLOT2) ||
           (fcn == Function::PLOT3) ||
           (fcn == Function::SCATTER2) ||
           (fcn == Function::SCATTER3) ||
           (fcn == Function::LINE_BETWEEN_POINTS_3D) ||
           (fcn == Function::POLYGON_FROM_4_POINTS) ||
           (fcn == Function::CLEAR) ||
           (fcn == Function::HOLD_ON) ||
           (fcn == Function::HOLD_OFF) ||
           (fcn == Function::POSITION) ||
           (fcn == Function::SURF) ||
           (fcn == Function::IM_SHOW) ||
           (fcn == Function::AXES_2D) ||
           (fcn == Function::AXES_3D) ||
           (fcn == Function::VIEW) ||
           (fcn == Function::SOFT_CLEAR);
}

void MainWindow::receiveData()
{
    std::unique_ptr<const ReceivedData> received_data = udp_server_->getReceivedData();
    if(received_data)
    {
        const FunctionHeader hdr = received_data->getFunctionHeader();

        const FunctionHeaderObject fcn_obj = hdr.getObjectAtIdx(0);
        if(fcn_obj.type != FunctionHeaderObjectType::FUNCTION)
        {
            throw std::runtime_error("Function object was not at element 0 in function header!");
        }

        const Function fcn = fcn_obj.getAs<Function>();

        if(isGuiElementFunction(fcn))
        {
            if(current_gui_element_set_)
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
            switch(fcn)
            {
                case Function::SET_CURRENT_ELEMENT:
                    setCurrentElement(hdr);

                    break;
                default:
                    std::cout << "Got default" << std::endl;
            }
        }
    }
}
