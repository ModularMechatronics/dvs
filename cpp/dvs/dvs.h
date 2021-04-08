#ifndef DVS_H_
#define DVS_H_

#include "low_level_com.h"
#include "math/math.h"
#include "function_header.h"

namespace dvs
{
namespace internal
{
constexpr uint64_t magic_num = 0xdeadbeefcafebabe;


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
    UdpClient udp_client(9547);
    printData(data_blob);
    udp_client.sendData(data_blob, num_bytes);
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
    // std::cout << data_to_be_sent.numBytes() << std::endl;
    
}

template <typename U>
void fillBuffer(uint8_t* data_blob, const U& data_to_be_sent)
{
    data_to_be_sent.fillBufferWithData(data_blob);
}

template <typename U, typename... Us>
void fillBuffer(uint8_t* data_blob, const U& data_to_be_sent, const Us&... datas)
{
    const uint64_t num_bytes = data_to_be_sent.numBytes();

    data_to_be_sent.fillBufferWithData(data_blob);
    fillBuffer(&(data_blob[num_bytes]), datas...);
}

template <typename U, typename... Us>
void sendData(const U& data_to_be_sent, const Us&... datas)
{
    const uint64_t num_bytes_of_first = data_to_be_sent.numBytes();

    const uint64_t num_bytes_from_object = countNumBytes(datas...) + num_bytes_of_first;

    // + 1 bytes for endianness byte
    // + 2 * sizeof(uint64_t) for magic number and number of bytes in transfer
    const uint64_t num_bytes = num_bytes_from_object + 1 + 2 * sizeof(uint64_t);
    uint8_t* data_blob = new uint8_t[num_bytes];
    data_blob[0] = isBigEndian();

    std::memcpy(&(data_blob[1]), &magic_num, sizeof(uint64_t));
    std::memcpy(&(data_blob[sizeof(uint64_t) + 1]), &num_bytes, sizeof(uint64_t));


    data_to_be_sent.fillBufferWithData(data_blob);
    // fillBuffer(&(data_blob[num_bytes_of_first]), datas...);

    sendThroughInterface(data_blob, num_bytes);

    delete[] data_blob;
}

}

template <typename Wx, typename Wy, typename... Us>
void plot(const Vector<Wx>& x, const Vector<Wy>& y, const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::PLOT2);
    hdr.append(internal::FunctionHeaderObjectType::DATA_STRUCTURE, internal::DataStructure::VECTOR);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<double>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_BUFFERS_REQUIRED, internal::toUInt8(2));
    hdr.extend(settings...);

    // TODO: change "sendData" to always take FunctionHeader as first argument
    internal::sendData(hdr, x, y);

    /*sendData(x, y, hdr);
    sendData(x, y, z, hdr);
    sendData(values, hdr);
    sendData(text_data, hdr);

    use pushToCore instead of sendData?*/
    

}

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
