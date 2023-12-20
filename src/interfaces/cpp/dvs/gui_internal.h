#ifndef DVS_GUI_INTERNAL_H
#define DVS_GUI_INTERNAL_H

#include <errno.h>
#include <stdlib.h>
#include <sys/resource.h>

#include <functional>
#include <map>
#include <utility>

#include "dvs/internal.h"
#include "dvs/math/math.h"

namespace dvs
{

namespace internal
{

class InternalGuiElementHandle
{
private:
    std::string handle_string_;
    dvs::GuiElementType type_;

public:
    InternalGuiElementHandle() {}
    InternalGuiElementHandle(const std::string& handle_string, const dvs::GuiElementType type)
        : handle_string_{handle_string}, type_{type}
    {
    }

    dvs::GuiElementType getType() const
    {
        return type_;
    }

    std::string getHandleString() const
    {
        return handle_string_;
    }

    void setLabel(const std::string& label) const
    {
        internal::CommunicationHeader hdr{internal::Function::SET_GUI_ELEMENT_LABEL};
        hdr.append(internal::CommunicationHeaderObjectType::LABEL, properties::Name(label.c_str()));
        hdr.append(internal::CommunicationHeaderObjectType::HANDLE_STRING, properties::Name(handle_string_.c_str()));

        internal::sendHeaderOnly(internal::getSendFunction(), hdr);
    }

    void setEnabled() const
    {
        internal::CommunicationHeader hdr{internal::Function::SET_GUI_ELEMENT_ENABLED};
        hdr.append(internal::CommunicationHeaderObjectType::HANDLE_STRING, properties::Name(handle_string_.c_str()));

        internal::sendHeaderOnly(internal::getSendFunction(), hdr);
    }

    void setDisabled() const
    {
        internal::CommunicationHeader hdr{internal::Function::SET_GUI_ELEMENT_DISABLED};
        hdr.append(internal::CommunicationHeaderObjectType::HANDLE_STRING, properties::Name(handle_string_.c_str()));

        internal::sendHeaderOnly(internal::getSendFunction(), hdr);
    }

    void setMinValue(const std::int32_t min_value) const
    {
        internal::CommunicationHeader hdr{internal::Function::SET_GUI_ELEMENT_MIN_VALUE};
        hdr.append(internal::CommunicationHeaderObjectType::HANDLE_STRING, properties::Name(handle_string_.c_str()));
        hdr.append(internal::CommunicationHeaderObjectType::INT32, toInt32(min_value));

        internal::sendHeaderOnly(internal::getSendFunction(), hdr);
    }

    void setMaxValue(const std::int32_t max_value) const
    {
        internal::CommunicationHeader hdr{internal::Function::SET_GUI_ELEMENT_MAX_VALUE};
        hdr.append(internal::CommunicationHeaderObjectType::HANDLE_STRING, properties::Name(handle_string_.c_str()));
        hdr.append(internal::CommunicationHeaderObjectType::INT32, toInt32(max_value));

        internal::sendHeaderOnly(internal::getSendFunction(), hdr);
    }

    void setValue(const std::int32_t value) const
    {
        internal::CommunicationHeader hdr{internal::Function::SET_GUI_ELEMENT_VALUE};
        hdr.append(internal::CommunicationHeaderObjectType::HANDLE_STRING, properties::Name(handle_string_.c_str()));
        hdr.append(internal::CommunicationHeaderObjectType::INT32, toInt32(value));

        internal::sendHeaderOnly(internal::getSendFunction(), hdr);
    }

    void setStepSize(const std::int32_t step_size) const
    {
        internal::CommunicationHeader hdr{internal::Function::SET_GUI_ELEMENT_STEP};
        hdr.append(internal::CommunicationHeaderObjectType::HANDLE_STRING, properties::Name(handle_string_.c_str()));
        hdr.append(internal::CommunicationHeaderObjectType::INT32, toInt32(step_size));

        internal::sendHeaderOnly(internal::getSendFunction(), hdr);
    }

    virtual void updateState(const UInt8ArrayView& data_view) = 0;
};

class SliderInternal final : public InternalGuiElementHandle
{
public:
    std::int32_t min_value_;
    std::int32_t max_value_;
    std::int32_t step_size_;
    std::int32_t value_;

public:
    SliderInternal(const std::string& handle_string, const UInt8ArrayView& data_view)
        : InternalGuiElementHandle{handle_string, dvs::GuiElementType::Slider}
    {
        updateState(data_view);
    }

    void updateState(const UInt8ArrayView& data_view) override
    {
        const std::uint8_t* const raw_gui_data = data_view.data();
        min_value_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data);
        max_value_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + sizeof(std::int32_t));
        step_size_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + 2U * sizeof(std::int32_t));
        value_ = *reinterpret_cast<const std::int32_t* const>(raw_gui_data + 3U * sizeof(std::int32_t));
    }
};

class ButtonInternal final : public InternalGuiElementHandle
{
public:
    bool is_pressed_;

public:
    ButtonInternal() {}
    ButtonInternal(const std::string& handle_string, const UInt8ArrayView& data_view)
        : InternalGuiElementHandle{handle_string, dvs::GuiElementType::Button}
    {
        updateState(data_view);
    }

    void updateState(const UInt8ArrayView& data_view) override
    {
        is_pressed_ = data_view.data()[0];
    }
};

class CheckboxInternal final : public InternalGuiElementHandle
{
public:
    bool is_checked_;

public:
    CheckboxInternal() {}
    CheckboxInternal(const std::string& handle_string, const UInt8ArrayView& data_view)
        : InternalGuiElementHandle{handle_string, dvs::GuiElementType::Checkbox}
    {
        updateState(data_view);
    }

    void updateState(const UInt8ArrayView& data_view) override
    {
        is_checked_ = data_view.data()[0];
    }
};

class TextLabelInternal final : public InternalGuiElementHandle
{
public:
    std::string label_;

public:
    TextLabelInternal() {}
    TextLabelInternal(const std::string& handle_string, const UInt8ArrayView& data_view)
        : InternalGuiElementHandle{handle_string, dvs::GuiElementType::TextLabel}
    {
        updateState(data_view);
    }

    void updateState(const UInt8ArrayView& data_view) override
    {
        const std::uint8_t label_length = data_view.data()[0];
        label_.resize(label_length);
        std::memcpy(label_.data(), data_view.data() + 1U, label_length);
    }
};

inline std::map<std::string, std::shared_ptr<InternalGuiElementHandle>>& getGuiElementHandles()
{
    static std::map<std::string, std::shared_ptr<InternalGuiElementHandle>> gui_element_handles;

    return gui_element_handles;
}

inline void throwExceptionIfPointerIsNotInitialized(const std::shared_ptr<SliderInternal>& internal_ptr)
{
    if (internal_ptr == nullptr)
    {
        throw std::runtime_error("Pointer not initialized!");
    }
}

inline int& getTcpSocket()
{
    static int tcp_sockfd;

    return tcp_sockfd;
}

inline void initTcpSocket()
{
    int& tcp_sockfd = getTcpSocket();
    struct sockaddr_in tcp_servaddr;

    tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set reuse address that's already in use (probably by exited dvs instance)
    int true_val = 1;
    setsockopt(tcp_sockfd, SOL_SOCKET, SO_REUSEADDR, &true_val, sizeof(int));

    bzero(&tcp_servaddr, sizeof(tcp_servaddr));

    tcp_servaddr.sin_family = AF_INET;
    tcp_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_servaddr.sin_port = htons(kGuiTcpPortNum);

    int ret_val = bind(tcp_sockfd, (struct sockaddr*)&tcp_servaddr, sizeof(tcp_servaddr));

    if (ret_val != 0)
    {
        throw std::runtime_error("Socket bind failed...");
    }

    if ((listen(tcp_sockfd, 5)) != 0)
    {
        throw std::runtime_error("Socket listen failed...");
    }
}

class ReceivedGuiData
{
private:
    uint8_t* data_;
    uint64_t num_data_bytes_;

public:
    ReceivedGuiData() : data_{nullptr}, num_data_bytes_{0U} {}
    ReceivedGuiData(const ReceivedGuiData& other) = delete;
    ReceivedGuiData(ReceivedGuiData&& other) : data_{other.data_}, num_data_bytes_{other.num_data_bytes_}
    {
        other.data_ = nullptr;
        other.num_data_bytes_ = 0U;
    }
    ReceivedGuiData& operator=(const ReceivedGuiData& other) = delete;
    ReceivedGuiData& operator=(ReceivedGuiData&& other)
    {
        if (data_ != nullptr)
        {
            delete[] data_;
        }

        data_ = other.data_;
        num_data_bytes_ = other.num_data_bytes_;

        other.data_ = nullptr;
        other.num_data_bytes_ = 0U;

        return *this;
    }

    ReceivedGuiData(const size_t size_to_allocate)
    {
        data_ = new uint8_t[size_to_allocate];
        num_data_bytes_ = size_to_allocate;
    }

    ~ReceivedGuiData()
    {
        if (data_ != nullptr)
        {
            delete[] data_;
        }
        num_data_bytes_ = 0U;
    }

    uint8_t* data() const
    {
        return data_;
    }

    uint64_t size() const
    {
        return num_data_bytes_;
    }
};

inline ReceivedGuiData receiveGuiData()
{
    const int tcp_sockfd = getTcpSocket();

    struct sockaddr_in tcp_cli;
    socklen_t tcp_len = sizeof(tcp_cli);

    const int tcp_connfd = accept(tcp_sockfd, (struct sockaddr*)&tcp_cli, &tcp_len);
    if (tcp_connfd < 0)
    {
        throw std::runtime_error("Server accept failed...");
    }

    uint64_t num_expected_bytes;
    read(tcp_connfd, &num_expected_bytes, sizeof(uint64_t));

    ReceivedGuiData received_data{num_expected_bytes};

    char* rec_buffer = reinterpret_cast<char*>(received_data.data());

    size_t total_num_received_bytes = 0U;
    size_t num_bytes_left = num_expected_bytes;

    while (true)
    {
        const ssize_t num_received_bytes = read(tcp_connfd, rec_buffer + total_num_received_bytes, num_bytes_left);

        total_num_received_bytes += num_received_bytes;
        num_bytes_left -= static_cast<size_t>(num_received_bytes);

        if (total_num_received_bytes >= num_expected_bytes)
        {
            break;
        }
    }

    close(tcp_connfd);

    return std::move(received_data);
}

inline void populateGuiElementWithData(const dvs::GuiElementType type,
                                       const std::string& handle_string,
                                       const UInt8ArrayView& data_view)
{
    std::map<std::string, std::shared_ptr<InternalGuiElementHandle>>& gui_element_handles = getGuiElementHandles();

    if (gui_element_handles.count(handle_string) > 0U)
    {
        if (gui_element_handles[handle_string]->getType() != type)
        {
            DVS_LOG_ERROR() << "Gui element with handle string " << handle_string
                            << " already exists, but has different type!";
            return;
        }
        gui_element_handles[handle_string]->updateState(data_view);
    }
    else
    {
        if (type == dvs::GuiElementType::Button)
        {
            gui_element_handles[handle_string] = std::make_shared<ButtonInternal>(handle_string, data_view);
        }
        else if (type == dvs::GuiElementType::Checkbox)
        {
            gui_element_handles[handle_string] = std::make_shared<CheckboxInternal>(handle_string, data_view);
        }
        else if (type == dvs::GuiElementType::Slider)
        {
            gui_element_handles[handle_string] = std::make_shared<SliderInternal>(handle_string, data_view);
        }
        else if (type == dvs::GuiElementType::TextLabel)
        {
            gui_element_handles[handle_string] = std::make_shared<TextLabelInternal>(handle_string, data_view);
        }
    }
}

inline void waitForSyncForAllGuiElements()
{
    DVS_LOG_INFO() << "Waiting for DVS application to send GUI state...";
    const ReceivedGuiData received_data{receiveGuiData()};

    DVS_LOG_INFO() << "GUI state received!";

    size_t idx{1U};

    const std::uint8_t* const raw_data = received_data.data();

    // Receive[0]: Number of gui objects (std::uint8_t)
    const std::size_t num_gui_objects = static_cast<std::size_t>(raw_data[0]);

    for (std::size_t k = 0; k < num_gui_objects; k++)
    {
        // Receive[1]: Gui element type (std::uint8_t)
        const dvs::GuiElementType type = static_cast<dvs::GuiElementType>(raw_data[idx]);
        idx += sizeof(std::uint8_t);

        // Receive[2]: Handle string length (std::uint8_t)
        const std::uint16_t handle_string_length = static_cast<std::uint16_t>(raw_data[idx]);
        idx += sizeof(std::uint8_t);

        std::string handle_string = "";

        // Receive[3]: Handle string (variable)
        for (std::uint16_t i = 0; i < handle_string_length; i++)
        {
            handle_string.push_back(raw_data[idx]);
            idx++;
        }

        std::uint16_t size_of_current_gui_element;

        // Receive[4]: Size of current gui element (std::uint16_t)
        std::memcpy(&size_of_current_gui_element, raw_data + idx, sizeof(std::uint16_t));
        idx += sizeof(std::uint32_t);

        // Receive[5]: Gui element data (variable)
        populateGuiElementWithData(type, handle_string, UInt8ArrayView{raw_data + idx, size_of_current_gui_element});

        idx += size_of_current_gui_element;
    }
}

inline void queryForSyncOfGuiData()
{
    CommunicationHeader hdr{Function::QUERY_FOR_SYNC_OF_GUI_DATA};

    sendHeaderOnly(getSendFunction(), hdr);
}

inline void updateGuiState(const ReceivedGuiData& received_gui_data)
{
    size_t idx = 0U;

    const std::uint8_t* const raw_data = received_gui_data.data();

    const dvs::GuiElementType type{raw_data[idx]};
    idx += sizeof(std::uint8_t);

    const std::uint8_t handle_string_length = raw_data[1];
    idx += sizeof(std::uint8_t);

    std::string handle_string = "";

    for (std::size_t k = 0; k < handle_string_length; k++)
    {
        handle_string.push_back(raw_data[idx]);
        idx += sizeof(std::uint8_t);
    }

    std::uint32_t payload_size;
    std::memcpy(&payload_size, raw_data + idx, sizeof(std::uint32_t));
    idx += sizeof(std::uint32_t);

    populateGuiElementWithData(type, handle_string, UInt8ArrayView{raw_data + idx, payload_size});
}

}  // namespace internal
}  // namespace dvs

#endif  // DVS_GUI_INTERNAL_H
