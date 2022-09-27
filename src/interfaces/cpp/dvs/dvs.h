#ifndef DVS_DVS_H_
#define DVS_DVS_H_

#include "dvs/internal.h"
#include "dvs/math/math.h"

namespace dvs
{
template <typename T, typename... Us> void plot(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLOT2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void plot(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLOT2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void fastPlot(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::FAST_PLOT2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void fastPlot(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::FAST_PLOT2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void lineCollection(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::LINE_COLLECTION2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void lineCollection(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::LINE_COLLECTION2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void lineCollection3(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::LINE_COLLECTION3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void lineCollection3(const VectorView<T>& x, const VectorView<T>& y, const VectorView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::LINE_COLLECTION3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void plotCollection(const std::vector<Vector<T>>& x, const std::vector<Vector<T>>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLOT_COLLECTION2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_OBJECTS, internal::toUInt32(x.size()));

    uint32_t num_elements = 0;

    DVS_ASSERT(x.size() == y.size());

    Vector<uint16_t> vector_lengths(x.size());

    for (size_t k = 0; k < x.size(); k++)
    {
        DVS_ASSERT(x[k].size() == y[k].size());
        vector_lengths(k) = x[k].size();
        num_elements += x[k].size();
    }

    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, num_elements);
    hdr.extend(settings...);

    const size_t num_bytes_to_send = sizeof(T) * 2 * num_elements;

    internal::sendHeaderAndVectorCollection(internal::getSendFunction(), hdr, vector_lengths, num_bytes_to_send, x, y);
}

template <typename T, typename... Us>
void plotCollection(const std::vector<std::reference_wrapper<Vector<T>>>& x, const std::vector<std::reference_wrapper<Vector<T>>>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLOT_COLLECTION2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_OBJECTS, internal::toUInt32(x.size()));

    uint32_t num_elements = 0;

    DVS_ASSERT(x.size() == y.size());

    Vector<uint16_t> vector_lengths(x.size());

    for (size_t k = 0; k < x.size(); k++)
    {
        const Vector<T>& x_ref = x[k];
        const Vector<T>& y_ref = y[k];
        DVS_ASSERT(x_ref.size() == y_ref.size());
        vector_lengths(k) = x_ref.size();
        num_elements += x_ref.size();
    }

    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, num_elements);
    hdr.extend(settings...);

    const size_t num_bytes_to_send = sizeof(T) * 2 * num_elements;

    internal::sendHeaderAndRefVectorCollection(internal::getSendFunction(), hdr, vector_lengths, num_bytes_to_send, x, y);
}

template <typename T, typename... Us>
void plotCollection3(const std::vector<Vector<T>>& x, const std::vector<Vector<T>>& y, const std::vector<Vector<T>>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLOT_COLLECTION3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_OBJECTS, internal::toUInt32(x.size()));

    uint32_t num_elements = 0;

    DVS_ASSERT(x.size() == y.size());
    DVS_ASSERT(x.size() == z.size());

    Vector<uint16_t> vector_lengths(x.size());

    for (size_t k = 0; k < x.size(); k++)
    {
        DVS_ASSERT(x[k].size() == y[k].size());
        DVS_ASSERT(x[k].size() == z[k].size());
        vector_lengths(k) = x[k].size();
        num_elements += x[k].size();
    }

    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, num_elements);
    hdr.extend(settings...);

    const size_t num_bytes_to_send = sizeof(T) * 3 * num_elements;

    internal::sendHeaderAndVectorCollection(internal::getSendFunction(), hdr, vector_lengths, num_bytes_to_send, x, y, z);
}

template <typename T, typename... Us>
void plotCollection3(const std::vector<std::reference_wrapper<Vector<T>>>& x, const std::vector<std::reference_wrapper<Vector<T>>>& y, const std::vector<std::reference_wrapper<Vector<T>>>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLOT_COLLECTION3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_OBJECTS, internal::toUInt32(x.size()));

    uint32_t num_elements = 0;

    DVS_ASSERT(x.size() == y.size());
    DVS_ASSERT(x.size() == z.size());

    Vector<uint16_t> vector_lengths(x.size());

    for (size_t k = 0; k < x.size(); k++)
    {
        const Vector<T>& x_ref = x[k];
        const Vector<T>& y_ref = y[k];
        const Vector<T>& z_ref = z[k];
        DVS_ASSERT(x_ref.size() == y_ref.size());
        DVS_ASSERT(x_ref.size() == z_ref.size());
        vector_lengths(k) = x_ref.size();
        num_elements += x_ref.size();
    }

    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, num_elements);
    hdr.extend(settings...);

    const size_t num_bytes_to_send = sizeof(T) * 3 * num_elements;

    internal::sendHeaderAndRefVectorCollection(internal::getSendFunction(), hdr, vector_lengths, num_bytes_to_send, x, y, z);
}

template <typename T, typename... Us> void stairs(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::STAIRS);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void stairs(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::STAIRS);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void plot3(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLOT3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void plot3(const VectorView<T>& x, const VectorView<T>& y, const VectorView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLOT3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void fastPlot3(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::FAST_PLOT3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void fastPlot3(const VectorView<T>& x, const VectorView<T>& y, const VectorView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::FAST_PLOT3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void stem(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::STEM);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void stem(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::STEM);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void scatter(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SCATTER2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void scatter(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SCATTER2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void quiver(const Matrix<T>& x, const Matrix<T>& y, const Matrix<T>& u, const Matrix<T>& v, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::QUIVER);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(x.numRows(), x.numCols()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, u, v);
}

template <typename T, typename... Us> void drawPoint(const Point2<T>& p, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SCATTER2);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(1));
    hdr.extend(settings...);

    Vector<T> x(1), y(1);
    x(0) = p.x;
    y(0) = p.y;

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void scatter3(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SCATTER3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void scatter3(const VectorView<T>& x, const VectorView<T>& y, const VectorView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SCATTER3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us> void drawPoint(const Point3<T>& p, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SCATTER3);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(1));
    hdr.extend(settings...);

    Vector<T> x(1), y(1), z(1);
    x(0) = p.x;
    y(0) = p.y;
    z(0) = p.z;

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void surf(const Matrix<T>& x, const Matrix<T>& y, const Matrix<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SURF);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(x.numRows(), x.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void surf(const MatrixView<T>& x, const MatrixView<T>& y, const MatrixView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SURF);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(x.numRows(), x.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us> void imShow(const ImageC1<T>& img, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::IM_SHOW);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_CHANNELS, internal::toUInt8(1));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(img.numElements()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(img.numRows(), img.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename T, typename... Us> void imShow(const ImageC3<T>& img, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::IM_SHOW);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_CHANNELS, internal::toUInt8(3));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(img.numRows() * img.numCols()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(img.numRows(), img.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename... Us>
void drawPolygonFrom4Points(const Point3<double>& p0,
                            const Point3<double>& p1,
                            const Point3<double>& p2,
                            const Point3<double>& p3,
                            const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::POLYGON_FROM_4_POINTS);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(4));
    Vector<Point3<double>> points{VectorInitializer{p0, p1, p2, p3}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename T, typename... Us> void drawTriangles(const Vector<Triangle3D<T>>& triangles, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_TRIANGLES_3D);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(triangles.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, triangles);
}

template <typename... Us> void drawTriangle(const Triangle3D<double>& triangle, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_TRIANGLES_3D);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(1));
    hdr.extend(settings...);

    const Vector<Triangle3D<double>> triangles{VectorInitializer{triangle}};

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, triangles);
}

template <typename T, typename... Us>
void drawTiles(const Matrix<T>& z, const Vec2<double>& tile_size, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_TILES);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(z.size()));
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(z.numRows(), z.numCols()));
    hdr.extend(settings...);

    Vector<double> v(2);
    v(0) = tile_size.x;
    v(1) = tile_size.y;

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, z, v);
}

template <typename T, typename... Us>
void drawMesh(const Vector<Point3<T>>& vertices, const Vector<IndexTriplet>& indices, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_MESH);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_VERTICES, internal::toUInt32(vertices.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_INDICES, internal::toUInt32(indices.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(indices.size()));  // Dummy, otherwise it fails

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, vertices, indices);
}

template <typename T, typename... Us>
void drawMesh(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Vector<IndexTriplet>& indices, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_MESH_SEPARATE_VECTORS);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_VERTICES, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_INDICES, internal::toUInt32(indices.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(indices.size()));  // Dummy, otherwise it fails

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, indices);
}

template <typename T, typename... Us>
void drawMesh(const VectorView<T>& x, const VectorView<T>& y, const VectorView<T>& z, const VectorView<IndexTriplet>& indices, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_MESH_SEPARATE_VECTORS);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_VERTICES, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_INDICES, internal::toUInt32(indices.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(indices.size()));  // Dummy, otherwise it fails

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, indices);
}

template <typename... Us>
void drawPlaneXY(const PointXY<double>& p0,
                 const PointXY<double>& p1,
                 const Plane<double>& plane,
                 const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLANE_XY);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(2));
    Vector<PointXY<double>> points{VectorInitializer{p0, p1}};
    Vector<Plane<double>> planes{VectorInitializer{plane}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points, planes);
}

template <typename... Us>
void drawPlaneXZ(const PointXZ<double>& p0,
                 const PointXZ<double>& p1,
                 const Plane<double>& plane,
                 const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLANE_XZ);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(2));
    Vector<PointXZ<double>> points{VectorInitializer{p0, p1}};
    Vector<Plane<double>> planes{VectorInitializer{plane}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points, planes);
}

template <typename... Us>
void drawPlaneYZ(const PointYZ<double>& p0,
                 const PointYZ<double>& p1,
                 const Plane<double>& plane,
                 const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::PLANE_YZ);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(2));
    Vector<PointYZ<double>> points{{p0, p1}};
    Vector<Plane<double>> planes{{plane}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points, planes);
}

template <typename... Us>
void drawLine(const Line3D<double>& line, const double t0, const double t1, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_LINE3D);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    const Point3<double> p0 = line.eval(t0);
    const Point3<double> p1 = line.eval(t1);
    Vector<Point3<double>> points{VectorInitializer{p0, p1}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us> void drawArrow(const Point2<double>& p, const Vec2<double> v, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_ARROW);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    Vector<Point2<double>> points{VectorInitializer{p, v}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us>
void drawLine2D(const ParametricLine2D<double>& line, const double t0, const double t1, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_LINE3D);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    const Point2<double> p0 = line.eval(t0);
    const Point2<double> p1 = line.eval(t1);
    Vector<Point3<double>> points{VectorInitializer{Point3<double>(p0.x, p0.y, 0.0), Point3<double>(p1.x, p1.y, 0.0)}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us>
void drawLine2DBetweenXValues(const HomogeneousLine2D<double>& line,
                              const double x0,
                              const double x1,
                              const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_LINE3D);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    const Point2<double> p0(x0, line.evalX(x0));
    const Point2<double> p1(x1, line.evalX(x1));
    Vector<Point3<double>> points{VectorInitializer{Point3<double>(p0.x, p0.y, 0.0), Point3<double>(p1.x, p1.y, 0.0)}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us>
void drawLine2DBetweenYValues(const HomogeneousLine2D<double>& line,
                              const double y0,
                              const double y1,
                              const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_LINE3D);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    const Point2<double> p0(line.evalX(y0), y0);
    const Point2<double> p1(line.evalX(y1), y1);
    Vector<Point3<double>> points{VectorInitializer{Point3<double>(p0.x, p0.y, 0.0), Point3<double>(p1.x, p1.y, 0.0)}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us>
void drawLineBetweenPoints(const Point3<double>& p0, const Point3<double>& p1, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_LINE_BETWEEN_POINTS_3D);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    Vector<Point3<double>> points{VectorInitializer{p0, p1}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

template <typename... Us>
void drawLineBetweenPoints(const Point2<double>& p0, const Point2<double>& p1, const Us&... settings)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DRAW_LINE_BETWEEN_POINTS_3D);
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(0));
    const Point3<double> p0_3d(p0.x, p0.y, 0.0), p1_3d(p1.x, p1.y, 0.0);
    Vector<Point3<double>> points{VectorInitializer{p0_3d, p1_3d}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, points);
}

inline void setCurrentElement(const std::string& name,
                              const ElementType element_type,
                              const std::string& parent_name = "#DEFAULTNAME#",
                              const ElementParent element_parent = ElementParent::TAB)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::CREATE_NEW_ELEMENT);
    hdr.append(internal::CommunicationHeaderObjectType::PARENT_NAME, properties::Name(parent_name.c_str()));
    hdr.append(internal::CommunicationHeaderObjectType::GUI_ELEMENT_TYPE, element_type);
    hdr.append(internal::CommunicationHeaderObjectType::PARENT_TYPE, element_parent);
    hdr.append(internal::CommunicationHeaderObjectType::ELEMENT_NAME, properties::Name(name.c_str()));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setCurrentElement(const std::string& name)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SET_CURRENT_ELEMENT);
    hdr.append(internal::CommunicationHeaderObjectType::ELEMENT_NAME, properties::Name(name.c_str()));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void waitForFlush()
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::WAIT_FOR_FLUSH);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void flushElement()
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::FLUSH_ELEMENT);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void view(const float azimuth, const float elevation)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::VIEW);
    hdr.append(internal::CommunicationHeaderObjectType::AZIMUTH, azimuth);
    hdr.append(internal::CommunicationHeaderObjectType::ELEVATION, elevation);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void axis(const Vec3<double>& min_bound, const Vec3<double>& max_bound)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::AXES_3D);
    hdr.append(internal::CommunicationHeaderObjectType::AXIS_MIN_MAX_VEC, std::pair<Vec3<double>, Vec3<double>>(min_bound, max_bound));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void axis(const Vec2<double>& min_bound, const Vec2<double>& max_bound)
{
    const Vec3<double> min_bound_3d(min_bound.x, min_bound.y, -1.0);
    const Vec3<double> max_bound_3d(max_bound.x, max_bound.y, 1.0);

    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::AXES_2D);
    hdr.append(internal::CommunicationHeaderObjectType::AXIS_MIN_MAX_VEC,
               std::pair<Vec3<double>, Vec3<double>>(min_bound_3d, max_bound_3d));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void diffuseLight(const Vec3<double>& light_position)
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::DIFFUSE_LIGHT);
    hdr.append(internal::CommunicationHeaderObjectType::VEC3, light_position);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void showLegend()
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SHOW_LEGEND);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void holdOn()
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::HOLD_ON);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void softClearView()
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::SOFT_CLEAR);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void clearView()
{
    internal::CommunicationHeader hdr;
    hdr.append(internal::CommunicationHeaderObjectType::FUNCTION, internal::Function::CLEAR);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

}  // namespace dvs

#endif // DVS_DVS_H_
