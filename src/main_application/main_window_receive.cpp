#include <unistd.h>

#include <csignal>
#include <iostream>
#include <stdexcept>

#include "duoplot/math/math.h"
#include "gui_window.h"
#include "main_window.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

using namespace duoplot::internal;

void MainWindow::setCurrentElement(const ReceivedData& received_data)
{
    const CommunicationHeader& hdr = received_data.getCommunicationHeader();

    const std::string current_element_name =
        hdr.get(CommunicationHeaderObjectType::ELEMENT_NAME).as<properties::Label>().data;

    if (current_element_name.length() == 0)
    {
        DUOPLOT_LOG_WARNING() << "Label string had zero length!";
        return;
    }

    current_element_name_ = current_element_name;

    if (plot_panes_.count(current_element_name_) == 0)
    {
        new_window_queued_ = true;
    }
}

void MainWindow::OnReceiveTimer(wxTimerEvent&)
{
    handleSerialData();

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

std::shared_ptr<const ConvertedDataBase> convertPlotObjectData(const CommunicationHeader& hdr,
                                                               const ReceivedData& received_data,
                                                               const PlotObjectAttributes& attributes,
                                                               const PropertiesData& properties_data)
{
    const Function fcn = received_data.getFunction();

    std::shared_ptr<const ConvertedDataBase> converted_data;

    switch (fcn)
    {
        case Function::STAIRS:
            converted_data = Stairs::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::PLOT2:
            converted_data = Plot2D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::PLOT3:
            converted_data = Plot3D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::FAST_PLOT2:
            converted_data = FastPlot2D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::FAST_PLOT3:
            converted_data = FastPlot3D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::LINE_COLLECTION2:
            converted_data =
                LineCollection2D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::LINE_COLLECTION3:
            converted_data =
                LineCollection3D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::STEM:
            converted_data = Stem::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::SCATTER2:
            converted_data = Scatter2D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::SCATTER3:
            converted_data = Scatter3D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::SURF:
            converted_data = Surf::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::IM_SHOW:
            converted_data = ImShow::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::PLOT_COLLECTION2:
            converted_data =
                PlotCollection2D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::PLOT_COLLECTION3:
            converted_data =
                PlotCollection3D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::DRAW_MESH_SEPARATE_VECTORS:
        case Function::DRAW_MESH:
            converted_data = DrawMesh::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
            break;

        case Function::REAL_TIME_PLOT:
            converted_data =
                ScrollingPlot2D::convertRawData(hdr, attributes, properties_data, received_data.payloadData());
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
        const CommunicationHeader& hdr{received_data.getCommunicationHeader()};
        const PlotObjectAttributes plot_object_attributes{hdr};
        const PropertiesData properties_data{hdr};

        std::shared_ptr<const ConvertedDataBase> converted_data =
            convertPlotObjectData(hdr, received_data, plot_object_attributes, properties_data);

        queued_data_[current_element_name_].push(
            std::make_unique<InputData>(received_data, converted_data, plot_object_attributes, properties_data));
    }
    else if (fcn == Function::PROPERTIES_EXTENSION)
    {
        const CommunicationHeader& hdr{received_data.getCommunicationHeader()};
        const PlotObjectAttributes plot_object_attributes{hdr};
        const PropertiesData properties_data{hdr};
        queued_data_[current_element_name_].push(
            std::make_unique<InputData>(received_data, plot_object_attributes, properties_data));
    }
    else if (fcn == Function::PROPERTIES_EXTENSION_MULTIPLE)
    {
        const CommunicationHeader& hdr{received_data.getCommunicationHeader()};
        const PlotObjectAttributes plot_object_attributes{hdr};
        const PropertiesData properties_data{hdr};
        queued_data_[current_element_name_].push(
            std::make_unique<InputData>(received_data, plot_object_attributes, properties_data));
    }
    else
    {
        queued_data_[current_element_name_].push(std::make_unique<InputData>(received_data));
    }
}

bool isGuiRelatedFunction(const Function fcn)
{
    return fcn == Function::SET_GUI_ELEMENT_LABEL;
}

void MainWindow::handleGuiManipulation(ReceivedData& received_data)
{
    const Function fcn = received_data.getFunction();

    const CommunicationHeader& hdr{received_data.getCommunicationHeader()};
    const std::string handle_string =
        hdr.get(CommunicationHeaderObjectType::HANDLE_STRING).as<properties::Label>().data;
    const std::string label = hdr.get(CommunicationHeaderObjectType::LABEL).as<properties::Label>().data;

    gui_elements_[handle_string]->setLabel(label);
}

void MainWindow::manageReceivedData(ReceivedData& received_data)
{
    const Function fcn = received_data.getFunction();

    {
        const std::lock_guard<std::mutex> lg(receive_mtx_);

        if (fcn == Function::OPEN_PROJECT_FILE)
        {
            const CommunicationHeader& hdr = received_data.getCommunicationHeader();
            queued_project_file_name_ =
                hdr.get(CommunicationHeaderObjectType::PROJECT_FILE_NAME).as<properties::Label>().data;
            open_project_file_queued_ = true;
        }
        else if (fcn == Function::SCREENSHOT)
        {
            const CommunicationHeader& hdr = received_data.getCommunicationHeader();
            std::string screenshot_base_path =
                hdr.get(CommunicationHeaderObjectType::SCREENSHOT_BASE_PATH).as<properties::Label>().data;
            performScreenshot(screenshot_base_path);
        }
        else if (fcn == Function::QUERY_FOR_SYNC_OF_GUI_DATA)
        {
            updateClientApplicationAboutGuiState();
        }
        else if (isGuiRelatedFunction(fcn))
        {
            handleGuiManipulation(received_data);
        }
        else
        {
            addActionToQueue(received_data);
        }
    }
}

void MainWindow::tcpReceiveThreadFunction()
{
    while (1)
    {
        ReceivedData received_data = data_receiver_.receiveAndGetDataFromTcp();
        if (received_data.rawData() == nullptr)
        {
            continue;
        }

        /*std::atomic<bool> received_data_has_been_moved = false;

        std::thread local_thread([this, &received_data, &received_data_has_been_moved] {
            // DUOPLOT_LOG_DEBUG() << "In thread!";
            ReceivedData local_received_data{std::move(received_data)};
            received_data_has_been_moved = true;
            manageReceivedData(local_received_data);
        });
        local_thread.detach();

        while (!received_data_has_been_moved)
            ;*/

        manageReceivedData(received_data);
    }
}

void MainWindow::pushNewDataToQueue(const TopicId topic_id, const std::shared_ptr<objects::BaseObject>& obj)
{
    if (objects_temporary_storage_.find(topic_id) != objects_temporary_storage_.end())
    {
        objects_temporary_storage_.at(topic_id).push_back(obj);
    }
}

void MainWindow::handleSerialData()
{
    std::vector<RawDataFrame> raw_data_frames{serial_interface_.extractRawDataFrames()};

    for (const RawDataFrame& raw_data_frame : raw_data_frames)
    {
        if (raw_data_frame.size() == 0U)
        {
            std::cout << "Error: Received data size is 0!" << std::endl;
            continue;
        }

        BufferedReader reader{raw_data_frame};

        size_t num_objects;
        reader.readByte(num_objects);

        for (size_t k = 0; k < num_objects; k++)
        {
            TopicId topic_id;
            reader.readUInt16(topic_id);

            ObjectType object_type;
            reader.readByte(object_type);

            std::shared_ptr<objects::BaseObject> obj;

            if (ObjectType::kNumber == object_type)
            {
                NumberDataType number_data_type;
                reader.readByte(number_data_type);

                if (number_data_type == NumberDataType::kFloat)
                {
                    obj = std::make_shared<objects::Float>(reader, topic_id);
                }
                else if (number_data_type == NumberDataType::kDouble)
                {
                    obj = std::make_shared<objects::Double>(reader, topic_id);
                }
                else if (number_data_type == NumberDataType::kInt8)
                {
                    obj = std::make_shared<objects::Int8>(reader, topic_id);
                }
                else if (number_data_type == NumberDataType::kInt16)
                {
                    obj = std::make_shared<objects::Int16>(reader, topic_id);
                }
                else if (number_data_type == NumberDataType::kInt32)
                {
                    obj = std::make_shared<objects::Int32>(reader, topic_id);
                }
                else if (number_data_type == NumberDataType::kInt64)
                {
                    obj = std::make_shared<objects::Int64>(reader, topic_id);
                }
                else if (number_data_type == NumberDataType::kUInt8)
                {
                    obj = std::make_shared<objects::UInt8>(reader, topic_id);
                }
                else if (number_data_type == NumberDataType::kUInt16)
                {
                    obj = std::make_shared<objects::UInt16>(reader, topic_id);
                }
                else if (number_data_type == NumberDataType::kUInt32)
                {
                    obj = std::make_shared<objects::UInt32>(reader, topic_id);
                }
                else if (number_data_type == NumberDataType::kUInt64)
                {
                    obj = std::make_shared<objects::UInt64>(reader, topic_id);
                }

                // objects::printObjectData(obj);
            }

            if (topic_id == 0xFFFEU)
            {
                const uint8_t value = static_cast<objects::UInt8*>(obj.get())->value();
                if (value == 0U && !serial_device_is_ready_for_new_data_)
                {
                    // TODO: Even if this is sort of only handled correctly here, there might be "old" data in the
                    // unread serial buffer that will push for a new update of the GUI, even though it was never
                    // requested by the device
                    // pushGuiDataToSerialInterface();
                    serial_device_is_ready_for_new_data_ = true;
                }
            }
            else
            {
                pushNewDataToQueue(topic_id, obj);
            }
        }

        if (!reader.hasReadToEnd())
        {
            for (size_t k = 0; k < raw_data_frame.size(); k++)
            {
                std::cout << "0x" << std::hex << static_cast<int>(raw_data_frame.data()[k]) << std::dec << " ";
            }
            std::cout << std::endl;
            std::cout << "Error: Did not read all data!" << std::endl;
        }
    }

    for (auto& [topic_id, objects] : objects_temporary_storage_)
    {
        if (plot_pane_subscriptions_.find(topic_id) == plot_pane_subscriptions_.end())
        {
            continue;
        }

        const std::vector<PlotPane*>& plot_panes_to_push_data_to = plot_pane_subscriptions_[topic_id];
        for (PlotPane* const plot_pane : plot_panes_to_push_data_to)
        {
            plot_pane->pushStreamData(topic_id, objects);
        }
        objects.clear();
    }
}

void MainWindow::mainWindowFlushMultipleElements(const ReceivedData& received_data)
{
    const internal::CommunicationHeader& hdr = received_data.getCommunicationHeader();

    const uint8_t num_names = hdr.get(CommunicationHeaderObjectType::NUM_NAMES).as<uint8_t>();

    const VectorConstView<uint8_t> name_lengths{received_data.payloadData(), static_cast<size_t>(num_names)};

    std::vector<std::string> names;

    size_t idx = num_names;

    for (size_t k = 0; k < num_names; k++)
    {
        names.push_back("");
        std::string& current_elem = names.back();

        const uint8_t current_element_length = name_lengths(k);

        for (size_t i = 0; i < current_element_length; i++)
        {
            current_elem += received_data.payloadData()[idx];
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
        ReceivedData fake_received_data{array_view.size()};
        std::memcpy(fake_received_data.rawData(), array_view.data(), array_view.size());
        fake_received_data.parseHeader();

        queued_data_[names[k]].push(std::make_unique<InputData>(fake_received_data));
    }
}

void MainWindow::receiveData()
{
    {
        const std::lock_guard<std::mutex> lg(receive_mtx_);

        if (open_project_file_queued_)
        {
            open_project_file_queued_ = false;
            openExistingFile(queued_project_file_name_);
        }

        if (new_window_queued_)
        {
            new_window_queued_ = false;
            newWindowWithoutFileModification(current_element_name_);
        }

        for (auto& qa : queued_data_)
        {
            if (!qa.second.empty())
            {
                const std::string element_handle_string = qa.first;

                if (plot_panes_.count(element_handle_string) > 0U)
                {
                    PlotPane* const plot_pane = dynamic_cast<PlotPane*>(plot_panes_[element_handle_string]);
                    plot_pane->pushQueue(qa.second);
                }
            }
        }
    }

    for (auto& ge : plot_panes_)
    {
        PlotPane* const plot_pane = dynamic_cast<PlotPane*>(ge.second);
        plot_pane->update();
    }
}
