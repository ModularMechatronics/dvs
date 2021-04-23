#ifndef DVS_H_
#define DVS_H_

#include <functional>

#include "low_level_com.h"
#include "math/math.h"
#include "function_header.h"

namespace dvs
{
namespace internal
{
constexpr uint64_t magic_num = 0xdeadbeefcafebabe;
using SendFunctionType = std::function<void(const uint8_t* const data_blob, const uint64_t num_bytes)>;

/*
In the header, there will be a series of bytes specifying different attributes
[attribute_type|num_bytes|<data bytes>] [attribute_type|num_bytes|<data bytes>]

*/

inline void printData(const uint8_t* const data)
{
    for(size_t k = 0; k < 100; k++)
    {
        uint8_t s0 = data[k] > 31 ? data[k] : 'x';
        s0 = s0 < 127 ? s0 : 'u';
        std::cout << static_cast<int>(data[k]) << ", '" << s0 << "'" << std::endl;
    }
}

inline uint8_t isBigEndian()
{
    const uint32_t x = 1;
    const uint8_t* const ptr = reinterpret_cast<const uint8_t* const>(&x);

    if(ptr[0] == '\x01')
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

inline void sendThroughInterface(const uint8_t* const data_blob, const uint64_t num_bytes)
{
    if(num_bytes > 1380)
    {
        std::cout << "Tried to send too many bytes: " << num_bytes << "! MTU is 1380..." << std::endl;
        exit(-1);
    }
    UdpClient udp_client(9752);
    udp_client.sendData(data_blob, num_bytes);
}

inline SendFunctionType getSendFunction()
{
    return sendThroughInterface;
}

template <typename U>
void countNumBytes(uint64_t& num_bytes, const U& data_to_be_sent)
{
    num_bytes += data_to_be_sent.numBytes();
}

template <typename U, typename... Us>
void countNumBytes(uint64_t& num_bytes, const U& data_to_be_sent, const Us&... datas)
{
    num_bytes += data_to_be_sent.numBytes();
    countNumBytes(num_bytes, datas...);
}

template <typename... Us>
uint64_t countNumBytes(const Us&... datas)
{
    uint64_t num_bytes = 0;
    countNumBytes(num_bytes, datas...);
    return num_bytes;
}

template <typename U>
void fillBuffer(uint8_t* const data_blob, const U& data_to_be_sent)
{
    data_to_be_sent.fillBufferWithData(data_blob);
}

template <typename U, typename... Us>
void fillBuffer(uint8_t* const data_blob, const U& data_to_be_sent, const Us&... other_elements)
{
    const uint64_t num_bytes = data_to_be_sent.numBytes();

    data_to_be_sent.fillBufferWithData(data_blob);
    fillBuffer(&(data_blob[num_bytes]), other_elements...);
}

template <typename U>
void sendHeaderAndData(const SendFunctionType& send_function,
                       const FunctionHeader& hdr,
                       const U& first_element)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();
    const uint64_t num_bytes_first = first_element.numBytes();

    const uint64_t num_bytes_from_object = num_bytes_hdr + num_bytes_first;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = new uint8_t[num_bytes];

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    std::memcpy(&(data_blob[idx]), &magic_num, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    std::memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    hdr.fillBufferWithData(&(data_blob[idx]));
    idx += num_bytes_hdr;

    first_element.fillBufferWithData(&(data_blob[idx]));

    send_function(data_blob, num_bytes);

    delete[] data_blob;
}

template <typename U, typename... Us>
void sendHeaderAndData(const SendFunctionType& send_function,
                       const FunctionHeader& hdr,
                       const U& first_element, const Us&... other_elements)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();
    const uint64_t num_bytes_first = first_element.numBytes();

    const uint64_t num_bytes_from_object = countNumBytes(other_elements...) + num_bytes_hdr + num_bytes_first;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = new uint8_t[num_bytes];

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    std::memcpy(&(data_blob[idx]), &magic_num, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    std::memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    hdr.fillBufferWithData(&(data_blob[idx]));
    idx += num_bytes_hdr;

    first_element.fillBufferWithData(&(data_blob[idx]));
    idx += num_bytes_first;

    fillBuffer(&(data_blob[idx]), other_elements...);

    send_function(data_blob, num_bytes);

    delete[] data_blob;
}

inline void sendHeaderOnly(const SendFunctionType& send_function, const FunctionHeader& hdr)
{
    const uint64_t num_bytes_hdr = hdr.numBytes();

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_hdr + 1 + 2 * sizeof(uint64_t);

    uint8_t* const data_blob = new uint8_t[num_bytes];

    uint64_t idx = 0;
    data_blob[idx] = isBigEndian();
    idx += 1;

    std::memcpy(&(data_blob[idx]), &magic_num, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    std::memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));
    idx += sizeof(uint64_t);

    hdr.fillBufferWithData(&(data_blob[idx]));

    send_function(data_blob, num_bytes);

    delete[] data_blob;
}

}

template <typename T, typename... Us>
void plot(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::PLOT2);
    hdr.append(internal::FunctionHeaderObjectType::DATA_STRUCTURE, internal::DataStructure::VECTOR);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);

    /*sendData(x, y, hdr);
    sendData(x, y, z, hdr);
    sendData(values, hdr);
    sendData(text_data, hdr);

    use pushToCore instead of sendData?*/
    

}

/*
TODO:
Create another class called something like HeaderPayload, that separates values (angle, axis bounds etc)
from flags?
*/

inline void newElement(const GuiElementType element_type, const std::string& name)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::NEW_ELEMENT);
    hdr.append(internal::FunctionHeaderObjectType::GUI_ELEMENT_TYPE, element_type);
    hdr.append(internal::FunctionHeaderObjectType::ELEMENT_NAME, name);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setCurrentElement(const std::string& name)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::SET_CURRENT_ELEMENT);
    hdr.appendProperty(internal::FunctionHeaderObjectType::ELEMENT_NAME, properties::Name(name.c_str()));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void view(const float azimuth, const float elevation)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::VIEW);
    hdr.append(internal::FunctionHeaderObjectType::AZIMUTH, azimuth);
    hdr.append(internal::FunctionHeaderObjectType::ELEVATION, elevation);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void axis(const Bound3D& min_bound, const Bound3D& max_bound)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::AXES);
    hdr.append(internal::FunctionHeaderObjectType::NUM_AXES, static_cast<uint8_t>(3));
    hdr.append(internal::FunctionHeaderObjectType::AXIS_MIN_MAX_VEC, AxesBounds(min_bound, max_bound));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

/*inline void axis(const Bound2D& min_bound, const Bound2D& max_bound)
{
    assert(false && "Function not supported yet...");
    const Bound3D min_bound_3d(min_bound.x, min_bound.y, -1.0);
    const Bound3D max_bound_3d(max_bound.x, max_bound.y, 1.0);

    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, Function::AXES);
    hdr.append(internal::FunctionHeaderObjectType::NUM_AXES, static_cast<uint8_t>(2));
    hdr.append(internal::FunctionHeaderObjectType::AXIS_MIN_MAX_VEC,
                   std::pair<Bound3D, Bound3D>(min_bound_3d, max_bound_3d));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}*/

/*inline void figure(const std::string figure_name)
{
    TxList tx_list;
    tx_list.append(Command::FUNCTION, Function::FIGURE_NAME);
    tx_list.append(Command::HAS_PAYLOAD, false);
    // tx_list.append(Command::FIGURE_NAME, static_cast<char>(figure_number));

    sendTxList(tx_list);
}*/


/*template <typename... Ts>
void sendDataInternal(const Ts&... data_to_be_sent)
{

}

template <typename... Ts>
void sendData(const Ts&... data_to_be_sent)
{

}*/

// template <typename T, typename... Us>
// void rtPlot(const T timestamp, const T value, const uint8_t plot_id, const uint8_t line_id, const Us&... settings)

/*template <typename T, typename... Us>
void rtPlot(const T value, const uint8_t plot_id, const uint8_t line_id, const Us&... settings)
{

    // std::vector<Attribute> attributes;
    // CommandType command_type = CommandType::RT_PLOT;
    (void)value;
    (void)plot_id;
    (void)line_id;

}*/

}


#endif
