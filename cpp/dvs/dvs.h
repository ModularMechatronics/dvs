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

inline bool checkAck(char data[256])
{
    const bool ar = data[0] == 'a' &&
                    data[1] == 'c' &&
                    data[2] == 'k' &&
                    data[3] == '#' &&
                    data[4] == '\0';
    return ar;
}

inline void sendThroughUdpInterface(const uint8_t* const data_blob, const uint64_t num_bytes)
{
    const size_t max_bytes_for_one_msg = 1380;
    UdpClient udp_client(9752);
    char data[256];

    if(num_bytes > max_bytes_for_one_msg)
    {
        size_t num_sent_bytes = 0;

        while(num_sent_bytes < num_bytes)
        {
            const size_t num_bytes_to_send = std::min(max_bytes_for_one_msg, static_cast<size_t>(num_bytes) - num_sent_bytes);

            udp_client.sendData(&(data_blob[num_sent_bytes]), num_bytes_to_send);
            num_sent_bytes += num_bytes_to_send;

            const int num_received_bytes = udp_client.receiveData(data);

            bool ack_received = checkAck(data);

            if(!ack_received)
            {
                throw std::runtime_error("No ack received!");
            }
            else if(num_received_bytes != 5)
            {
                throw std::runtime_error("Ack received but number of bytes was " + std::to_string(num_received_bytes));
            }
        }
    }
    else
    {
        udp_client.sendData(data_blob, num_bytes);

        const int num_received_bytes = udp_client.receiveData(data);

        bool ack_received = checkAck(data);

        if(!ack_received)
        {
            throw std::runtime_error("No ack received!");
        }
        else if(num_received_bytes != 5)
        {
            throw std::runtime_error("Ack received but number of bytes was " + std::to_string(num_received_bytes));
        }
    }
}

inline SendFunctionType getSendFunction()
{
    return sendThroughUdpInterface;
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
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void plot3(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::PLOT3);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void scatter(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::SCATTER2);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void scatter3(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::SCATTER3);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void surf(const Matrix<T>& x, const Matrix<T>& y, const Matrix<T>& z, const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::SURF);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size())); // TODO: Needed?
    hdr.append(internal::FunctionHeaderObjectType::DIMENSION_2D, internal::Dimension2D(x.rows(), x.cols()));
    
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void imShow(const Matrix<T>& img, const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::IM_SHOW);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(img.size())); // TODO: Needed?
    hdr.append(internal::FunctionHeaderObjectType::DIMENSION_2D, internal::Dimension2D(img.rows(), img.cols()));
    
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename T, typename... Us>
void drawPolygonFrom4Points(const Point3D<T>& p0,
                            const Point3D<T>& p1,
                            const Point3D<T>& p2,
                            const Point3D<T>& p3,
                            const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::POLYGON_FROM_4_POINTS);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(4));
    Vector<Point3D<T>> points = {p0, p1, p2, p3};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename T, typename... Us>
void drawTriangles(const Vector<Triangle3D<T>>& triangles,
                   const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::DRAW_TRIANGLES_3D);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(triangles.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, triangles);
}

template <typename T, typename... Us>
void drawTriangle(const Triangle3D<T>& triangle,
                  const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::DRAW_TRIANGLES_3D);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(1));
    hdr.extend(settings...);

    const Vector<Triangle3D<T>> triangles = {triangle};

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, triangles);
}

template <typename T, typename... Us>
void drawMesh(const Vector<Point3D<T>>& vertices,
              const Vector<IndexTriplet>& indices,
              const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::DRAW_MESH);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_VERTICES, internal::toUInt32(vertices.size()));
    hdr.append(internal::FunctionHeaderObjectType::NUM_INDICES, internal::toUInt32(indices.size()));
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(indices.size())); // Dummy, otherwise it fails

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, vertices, indices);
}

template <typename T, typename... Us>
void drawPlaneXY(const PointXY<T>& p0,
                 const PointXY<T>& p1,
                 const Plane<T>& plane,
                 const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::PLANE_XY);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(2));
    Vector<PointXY<T>> points = {p0, p1};
    Vector<Plane<T>> planes = {plane};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points, planes);
}

template <typename T, typename... Us>
void drawPlaneXZ(const PointXZ<T>& p0,
                 const PointXZ<T>& p1,
                 const Plane<T>& plane,
                 const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::PLANE_XZ);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(2));
    Vector<PointXZ<T>> points = {p0, p1};
    Vector<Plane<T>> planes = {plane};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points, planes);
}

template <typename T, typename... Us>
void drawPlaneYZ(const PointYZ<T>& p0,
                 const PointYZ<T>& p1,
                 const Plane<T>& plane,
                 const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::PLANE_YZ);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(2));
    Vector<PointYZ<T>> points = {p0, p1};
    Vector<Plane<T>> planes = {plane};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points, planes);
}

template <typename T, typename... Us>
void drawLine(const Line3D<T>& line, const T t0, const T t1, const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::DRAW_LINE3D);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    Vector<Line3D<T>> lines = {line};
    Vector<T> ts = {t0, t1};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, lines, ts);
}


template <typename T, typename... Us>
void drawLineBetweenPoints(const Point3D<T>& p0, const Point3D<T>& p1, const Us&... settings)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::DRAW_LINE_BETWEEN_POINTS_3D);
    hdr.append(internal::FunctionHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::FunctionHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    Vector<Point3D<T>> points = {p0, p1};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

inline void setCurrentElement(const std::string& name, const ElementType element_type, const std::string& parent_name="#DEFAULTNAME#", const ElementParent element_parent=ElementParent::TAB)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::CREATE_NEW_ELEMENT);
    hdr.append(internal::FunctionHeaderObjectType::PARENT_NAME, properties::Name(parent_name.c_str()));
    hdr.append(internal::FunctionHeaderObjectType::GUI_ELEMENT_TYPE, element_type);
    hdr.append(internal::FunctionHeaderObjectType::PARENT_TYPE, element_parent);
    hdr.append(internal::FunctionHeaderObjectType::ELEMENT_NAME, properties::Name(name.c_str()));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setCurrentElement(const std::string& name)
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::SET_CURRENT_ELEMENT);
    hdr.append(internal::FunctionHeaderObjectType::ELEMENT_NAME, properties::Name(name.c_str()));

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
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::AXES_3D);
    hdr.append(internal::FunctionHeaderObjectType::AXIS_MIN_MAX_VEC, AxesBounds(min_bound, max_bound));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void axis(const Bound2D& min_bound, const Bound2D& max_bound)
{
    const Bound3D min_bound_3d(min_bound.x, min_bound.y, -1.0);
    const Bound3D max_bound_3d(max_bound.x, max_bound.y, 1.0);

    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::AXES_2D);
    hdr.append(internal::FunctionHeaderObjectType::AXIS_MIN_MAX_VEC, AxesBounds(min_bound_3d, max_bound_3d));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void holdOn()
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::HOLD_ON);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void clearFigure()
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::SOFT_CLEAR);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void hardClearFigure()
{
    internal::FunctionHeader hdr;
    hdr.append(internal::FunctionHeaderObjectType::FUNCTION, internal::Function::CLEAR);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

/*

void drawVoxels(Vector x, Vector y, Vector z)
void drawVoxels(Vector<Point3D> points, size)

inline void setElementPositionAndSize(const int x, const int y)
inline void setWindowPositionAndSize(const int x, const int y)
{
    TxList tx_list;
    tx_list.append(Command::FUNCTION, Function::POSITION);
    tx_list.append(Command::HAS_PAYLOAD, false);
    tx_list.append(Command::POS2D, Pos2D(x, y));

    sendTxList(tx_list);
}
*/

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
