#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "dvs/math/math.h"
#include "main_window.h"
#include "plot_objects/plot_object_base/plot_object_base.h"
#include "window_view.h"

using namespace dvs::internal;

/*bool isGuiElementFunction(const Function fcn)
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
           (fcn == Function::REAL_TIME_PLOT) || (fcn == Function::PROPERTIES_EXTENSION) ||
           (fcn == Function::DISABLE_AXES_FROM_MIN_MAX) || (fcn == Function::SET_AXES_BOX_SCALE_FACTOR) ||
           (fcn == Function::SET_OBJECT_TRANSFORM) || (fcn == Function::SHOW_LEGEND) ||
           (fcn == Function::FLUSH_ELEMENT) || (fcn == Function::WAIT_FOR_FLUSH);
}*/

void MainWindow::setCurrentElement(const ReceivedData& received_data)
{
    const CommunicationHeader& hdr = received_data.getCommunicationHeader();

    current_element_name_ = hdr.get(CommunicationHeaderObjectType::ELEMENT_NAME).as<properties::Name>().data;

    if (current_element_name_.length() == 0)
    {
        throw std::runtime_error("Name string had zero length!");
    }

    /*
    if (!currentGuiElementSet())
    {
        // TODO: Create new window with gui element or something
    }
    if (gui_elements_.count(element_name_str) > 0)
    {
        current_gui_element_ = gui_elements_[element_name_str];
        // current_gui_element_set_ = true;
    }
    else
    {
        // TODO: Handle this
        // newNamedElement(element_name_str);
    }
    */
}

void MainWindow::OnReceiveTimer(wxTimerEvent&)
{
    try
    {
        receiveData();
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Got runtime_error when receiving: " << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Got exception when receiving: " << e.what() << std::endl;
    }
}

std::unique_ptr<const ConvertedDataBase> convertPlotObjectData(const ReceivedData& received_data)
{
    const Function fcn = received_data.getFunction();

    std::unique_ptr<const ConvertedDataBase> converted_data;

    const PlotObjectAttributes attributes{received_data.getCommunicationHeader()};

    switch (fcn)
    {
        case Function::STAIRS:
            converted_data = Stairs::convertRawData(attributes, received_data.data());
            break;

        case Function::PLOT2:
            converted_data = Plot2D::convertRawData(attributes, received_data.data());
            break;

        case Function::PLOT3:
            converted_data = Plot3D::convertRawData(attributes, received_data.data());
            break;

        case Function::FAST_PLOT2:
            converted_data = FastPlot2D::convertRawData(attributes, received_data.data());
            break;

        case Function::FAST_PLOT3:
            converted_data = FastPlot3D::convertRawData(attributes, received_data.data());
            break;

        case Function::LINE_COLLECTION2:
            converted_data = LineCollection2D::convertRawData(attributes, received_data.data());
            break;

        case Function::LINE_COLLECTION3:
            converted_data = LineCollection3D::convertRawData(attributes, received_data.data());
            break;

        case Function::STEM:
            converted_data = Stem::convertRawData(attributes, received_data.data());
            break;

        case Function::SCATTER2:
            converted_data = Scatter2D::convertRawData(attributes, received_data.data());
            break;

        case Function::SCATTER3:
            converted_data = Scatter3D::convertRawData(attributes, received_data.data());
            break;

        case Function::SURF:
            converted_data = Surf::convertRawData(attributes, received_data.data());
            break;

        case Function::IM_SHOW:
            converted_data = ImShow::convertRawData(attributes, received_data.data());
            break;

        case Function::PLOT_COLLECTION2:
            converted_data = PlotCollection2D::convertRawData(attributes, received_data.data());
            break;

        case Function::PLOT_COLLECTION3:
            converted_data = PlotCollection3D::convertRawData(attributes, received_data.data());
            break;

        case Function::DRAW_MESH_SEPARATE_VECTORS:
        case Function::DRAW_MESH:
            converted_data = DrawMesh::convertRawData(attributes, received_data.data());
            break;

        case Function::REAL_TIME_PLOT:
            converted_data = ScrollingPlot2D::convertRawData(attributes, received_data.data());
            break;
        default:
            throw std::runtime_error("Invalid function!");
            break;
    }

    return converted_data;
}

void MainWindow::addActionToQueue(ReceivedData& received_data)
{
    const Function fcn = received_data.getFunction();

    if (fcn == Function::SET_CURRENT_ELEMENT)
    {
        setCurrentElement(received_data);
    }
    else if (fcn == Function::FLUSH_MULTIPLE_ELEMENTS)
    {
        mainWindowFlushMultipleElements(received_data);
    }
    else if (isPlotDataFunction(fcn))
    {
        std::unique_ptr<const ConvertedDataBase> converted_data = convertPlotObjectData(received_data);
        queued_data_[current_element_name_].push(std::make_unique<InputData>(received_data, converted_data));
    }
    else
    {
        queued_data_[current_element_name_].push(std::make_unique<InputData>(received_data));
    }
}

void MainWindow::receiveThreadFunction()
{
    while (1)
    {
        ReceivedData received_data = udp_server_->receiveAndGetData();

        const Function fcn = received_data.getFunction();

        {
            const std::lock_guard<std::mutex> lg(reveive_mtx_);

            if (fcn == Function::CREATE_NEW_ELEMENT)
            {
                // createNewElement(hdr);
                // TODO
                std::cout << "Not implemented yet!" << std::endl;
            }
            else if (fcn == Function::OPEN_PROJECT_FILE)
            {
                const CommunicationHeader& hdr = received_data.getCommunicationHeader();
                queued_project_file_name_ =
                    hdr.get(CommunicationHeaderObjectType::PROJECT_FILE_NAME).as<properties::Name>();
                open_project_file_queued_ = true;
            }
            else
            {
                addActionToQueue(received_data);
            }
        }
    }
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

bool MainWindow::currentGuiElementSet() const
{
    return current_gui_element_ != nullptr;
}

void MainWindow::queryUdpThreadFunction()
{
    std::cout << "Starting query server " << std::endl;
    char send_data[256];

    while (1)
    {
        std::cout << "Starting receive " << std::endl;
        query_udp_server_->receive();
        std::unique_ptr<const ReceivedData> received_data = query_udp_server_->getReceivedData();

        if (received_data)
        {
            size_t num_objects_in_buf2;
            {
                const std::lock_guard<std::mutex> lg(udp_mtx_);
                num_objects_in_buf2 = udp_server_->numObjectsInReceiveBuffer();
            }

            const size_t num_objects_in_buf = 82342;

            std::cout << "Data had something! " << std::endl;
            std::memcpy(send_data, &num_objects_in_buf, sizeof(size_t));

            for (size_t k = 0; k < 50; k++)
            {
                usleep(1000 * 50);

                query_udp_server_->sendData(send_data, sizeof(size_t));
                query_udp_server_->sendData(send_data, sizeof(size_t));
                query_udp_server_->sendData(send_data, sizeof(size_t));
                query_udp_server_->sendData(send_data, sizeof(size_t));
            }
        }
        else
        {
            std::cout << "Data contained nothing... " << std::endl;
        }
    }
}

void MainWindow::mainWindowFlushMultipleElements(const ReceivedData& received_data)
{
    const internal::CommunicationHeader& hdr = received_data.getCommunicationHeader();

    const uint8_t num_names = hdr.get(CommunicationHeaderObjectType::NUM_NAMES).as<uint8_t>();

    const VectorConstView<uint8_t> name_lengths{received_data.data(), static_cast<size_t>(num_names)};

    std::vector<std::string> names;

    size_t idx = num_names;

    for (size_t k = 0; k < num_names; k++)
    {
        names.push_back("");
        std::string& current_elem = names.back();

        const uint8_t current_element_length = name_lengths(k);

        for (size_t i = 0; i < current_element_length; i++)
        {
            current_elem += received_data.data()[idx];
            idx++;
        }
    }

    // Have to create a 'fake' header and 'fake' received data
    internal::CommunicationHeader faked_hdr{internal::Function::FLUSH_ELEMENT};

    const uint64_t num_bytes_hdr = faked_hdr.numBytes();

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_hdr + 1 + 2 * sizeof(uint64_t);

    FillableUInt8Array fillable_array{num_bytes};

    fillable_array.fillWithStaticType(isBigEndian());
    fillable_array.fillWithStaticType(kMagicNumber);
    fillable_array.fillWithStaticType(num_bytes);

    faked_hdr.fillBufferWithData(fillable_array);

    const UInt8ArrayView array_view{fillable_array.data(), fillable_array.size()};

    for (size_t k = 0; k < names.size(); k++)
    {
        ReceivedData fake_received_data{array_view};
        queued_data_[names[k]].push(std::make_unique<InputData>(fake_received_data));
    }
}

void MainWindow::openFileFromClient(const internal::CommunicationHeader& hdr)
{
    const properties::Name file_path = hdr.get(CommunicationHeaderObjectType::PROJECT_FILE_NAME).as<properties::Name>();
    openExistingFile(file_path.data);
}

void MainWindow::receiveData()
{
    if (open_project_file_queued_)
    {
        open_project_file_queued_ = false;
        openExistingFile(queued_project_file_name_.data);
    }

    {
        const std::lock_guard<std::mutex> lg(reveive_mtx_);

        for (auto& qa : queued_data_)
        {
            if (!qa.second.empty())
            {
                const std::string element_name = qa.first;

                GuiElement* gui_element = gui_elements_[element_name];
                gui_element->pushQueue(qa.second);
            }
        }
    }

    for (auto& ge : gui_elements_)
    {
        ge.second->update();
    }
}
