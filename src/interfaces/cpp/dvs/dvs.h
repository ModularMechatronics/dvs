#ifndef DVS_DVS_H_
#define DVS_DVS_H_

#include "dvs/internal.h"
#include "dvs/math/math.h"

namespace dvs
{
template <typename T, typename... Us> void plot(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void plot(const Vector<T>& x, const Vector<T>& y, const Vector<RGB888>& color, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, color);
}

template <typename T, typename... Us> void plot(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void fastPlot(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::FAST_PLOT2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void fastPlot(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::FAST_PLOT2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void lineCollection(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::LINE_COLLECTION2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void lineCollection(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::LINE_COLLECTION2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void lineCollection3(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::LINE_COLLECTION3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void lineCollection3(const VectorView<T>& x, const VectorView<T>& y, const VectorView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::LINE_COLLECTION3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void plotCollection(const std::vector<Vector<T>>& x, const std::vector<Vector<T>>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT_COLLECTION2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_OBJECTS, internal::toUInt32(x.size()));

    DVS_ASSERT(x.size() == y.size());

    uint32_t num_elements = 0;
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
void plotCollection(const std::vector<VectorView<T>>& x, const std::vector<VectorView<T>>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT_COLLECTION2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_OBJECTS, internal::toUInt32(x.size()));

    DVS_ASSERT(x.size() == y.size());

    uint32_t num_elements = 0;
    Vector<uint16_t> vector_lengths(x.size());

    for (size_t k = 0; k < x.size(); k++)
    {
        const VectorView<T> x_view = x[k];
        const VectorView<T> y_view = y[k];
        DVS_ASSERT(x_view.size() == y_view.size());
        vector_lengths(k) = x_view.size();
        num_elements += x_view.size();
    }

    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, num_elements);
    hdr.extend(settings...);

    const size_t num_bytes_to_send = sizeof(T) * 2 * num_elements;

    internal::sendHeaderAndVectorCollection(internal::getSendFunction(), hdr, vector_lengths, num_bytes_to_send, x, y);
}

template <typename T, typename... Us>
void plotCollection3(const std::vector<Vector<T>>& x,
                     const std::vector<Vector<T>>& y,
                     const std::vector<Vector<T>>& z,
                     const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT_COLLECTION3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_OBJECTS, internal::toUInt32(x.size()));

    DVS_ASSERT(x.size() == y.size());
    DVS_ASSERT(x.size() == z.size());

    uint32_t num_elements = 0;
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

    internal::sendHeaderAndVectorCollection(
        internal::getSendFunction(), hdr, vector_lengths, num_bytes_to_send, x, y, z);
}

template <typename T, typename... Us>
void plotCollection3(const std::vector<VectorView<T>>& x,
                     const std::vector<VectorView<T>>& y,
                     const std::vector<VectorView<T>>& z,
                     const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT_COLLECTION3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_OBJECTS, internal::toUInt32(x.size()));

    DVS_ASSERT(x.size() == y.size());
    DVS_ASSERT(x.size() == z.size());

    uint32_t num_elements = 0;
    Vector<uint16_t> vector_lengths(x.size());

    for (size_t k = 0; k < x.size(); k++)
    {
        const VectorView<T> x_view = x[k];
        const VectorView<T> y_view = y[k];
        const VectorView<T> z_view = z[k];
        DVS_ASSERT(x_view.size() == y_view.size());
        DVS_ASSERT(x_view.size() == z_view.size());
        vector_lengths(k) = x_view.size();
        num_elements += x_view.size();
    }

    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, num_elements);
    hdr.extend(settings...);

    const size_t num_bytes_to_send = sizeof(T) * 3 * num_elements;

    internal::sendHeaderAndVectorCollection(
        internal::getSendFunction(), hdr, vector_lengths, num_bytes_to_send, x, y, z);
}

template <typename T, typename... Us> void stairs(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::STAIRS};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void stairs(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::STAIRS};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void plot3(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void plot3(const VectorView<T>& x, const VectorView<T>& y, const VectorView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void fastPlot3(const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::FAST_PLOT3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void fastPlot3(const VectorView<T>& x, const VectorView<T>& y, const VectorView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::FAST_PLOT3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us> void stem(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::STEM};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void stem(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::STEM};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void scatter(const Vector<T>& x, const Vector<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void scatter(const Vector<T>& x, const Vector<T>& y, const Vector<RGB888>& color, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, color);
}

template <typename T, typename... Us>
void scatter(const VectorView<T>& x, const VectorView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us> void drawPoint(const Point2<T>& p, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER2};
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
    internal::CommunicationHeader hdr{internal::Function::SCATTER3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void scatter3(const VectorView<T>& x, const VectorView<T>& y, const VectorView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void scatter3(
    const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Vector<RGB888>& color, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, color);
}

template <typename T, typename... Us> void drawPoint(const Point3<T>& p, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER3};
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
    internal::CommunicationHeader hdr{internal::Function::SURF};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(x.numRows(), x.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void surf(
    const Matrix<T>& x, const Matrix<T>& y, const Matrix<T>& z, const Matrix<RGB888>& color, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SURF};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(x.numRows(), x.numCols()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, color);
}

template <typename T, typename... Us>
void surf(const MatrixView<T>& x, const MatrixView<T>& y, const MatrixView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SURF};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(x.numRows(), x.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us> void imShow(const ImageC1<T>& img, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::IM_SHOW};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_CHANNELS, internal::toUInt8(1));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(img.numElements()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D,
               internal::Dimension2D(img.numRows(), img.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename T, typename... Us> void imShow(const ImageC3<T>& img, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::IM_SHOW};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_CHANNELS, internal::toUInt8(3));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(img.numRows() * img.numCols()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D,
               internal::Dimension2D(img.numRows(), img.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename T, typename... Us> void imShow(const ImageC4<T>& img, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::IM_SHOW};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_CHANNELS, internal::toUInt8(4));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(img.numRows() * img.numCols()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D,
               internal::Dimension2D(img.numRows(), img.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename T, typename... Us> void imShow(const ImageC1View<T>& img, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::IM_SHOW};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_CHANNELS, internal::toUInt8(1));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(img.numElements()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D,
               internal::Dimension2D(img.numRows(), img.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename T, typename... Us> void imShow(const ImageC3View<T>& img, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::IM_SHOW};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_CHANNELS, internal::toUInt8(3));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(img.numRows() * img.numCols()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D,
               internal::Dimension2D(img.numRows(), img.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename T, typename... Us> void imShow(const ImageC4View<T>& img, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::IM_SHOW};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_CHANNELS, internal::toUInt8(4));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(img.numRows() * img.numCols()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D,
               internal::Dimension2D(img.numRows(), img.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename T, typename... Us>
void drawMesh(const Vector<Point3<T>>& vertices, const Vector<IndexTriplet>& indices, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::DRAW_MESH};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_VERTICES, internal::toUInt32(vertices.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_INDICES, internal::toUInt32(indices.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(indices.size()));  // Dummy, otherwise it fails

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, vertices, indices);
}

template <typename T, typename... Us>
void drawMesh(const Vector<T>& x,
              const Vector<T>& y,
              const Vector<T>& z,
              const Vector<IndexTriplet>& indices,
              const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::DRAW_MESH_SEPARATE_VECTORS};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_VERTICES, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_INDICES, internal::toUInt32(indices.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(indices.size()));  // Dummy, otherwise it fails

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, indices);
}

template <typename T, typename... Us>
void drawMesh(const VectorView<T>& x,
              const VectorView<T>& y,
              const VectorView<T>& z,
              const VectorView<IndexTriplet>& indices,
              const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::DRAW_MESH_SEPARATE_VECTORS};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_VERTICES, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_INDICES, internal::toUInt32(indices.size()));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(indices.size()));  // Dummy, otherwise it fails

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, indices);
}

template <typename... Us>
void drawLine(const Line3D<double>& line, const double t0, const double t1, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::DataType::DOUBLE);
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(2));
    const Point3<double> p0 = line.eval(t0);
    const Point3<double> p1 = line.eval(t1);
    const Vector<double> x{VectorInitializer{p0.x, p1.x}};
    const Vector<double> y{VectorInitializer{p0.y, p1.y}};

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void realTimePlot(const T dt, const T y, const internal::PlotSlot slot, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::REAL_TIME_PLOT};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(1U));
    hdr.append(internal::CommunicationHeaderObjectType::SLOT, slot);
    hdr.extend(settings...);

    const Vector<T> data{VectorInitializer<T>{dt, y}};

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, data);
}

template <typename... Us> void setProperties(const internal::PlotSlot slot, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PROPERTIES_EXTENSION};
    hdr.append(internal::CommunicationHeaderObjectType::SLOT, slot);
    hdr.extend(settings...);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setCurrentElement(const std::string& name)
{
    internal::CommunicationHeader hdr{internal::Function::SET_CURRENT_ELEMENT};
    hdr.append(internal::CommunicationHeaderObjectType::ELEMENT_NAME, properties::Name(name.c_str()));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void waitForFlush()
{
    internal::CommunicationHeader hdr{internal::Function::WAIT_FOR_FLUSH};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void flushElement()
{
    internal::CommunicationHeader hdr{internal::Function::FLUSH_ELEMENT};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void view(const float azimuth_deg, const float elevation_deg)
{
    internal::CommunicationHeader hdr{internal::Function::VIEW};
    hdr.append(internal::CommunicationHeaderObjectType::AZIMUTH, azimuth_deg);
    hdr.append(internal::CommunicationHeaderObjectType::ELEVATION, elevation_deg);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void axis(const Vec3<double>& min_bound, const Vec3<double>& max_bound)
{
    internal::CommunicationHeader hdr{internal::Function::AXES_3D};
    hdr.append(internal::CommunicationHeaderObjectType::AXIS_MIN_MAX_VEC,
               std::pair<Vec3<double>, Vec3<double>>(min_bound, max_bound));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void axis(const Vec2<double>& min_bound, const Vec2<double>& max_bound)
{
    const Vec3<double> min_bound_3d(min_bound.x, min_bound.y, -1.0);
    const Vec3<double> max_bound_3d(max_bound.x, max_bound.y, 1.0);

    internal::CommunicationHeader hdr{internal::Function::AXES_2D};
    hdr.append(internal::CommunicationHeaderObjectType::AXIS_MIN_MAX_VEC,
               std::pair<Vec3<double>, Vec3<double>>(min_bound_3d, max_bound_3d));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void globalIllumination(const Vec3<double>& light_position)
{
    internal::CommunicationHeader hdr{internal::Function::GLOBAL_ILLUMINATION};
    hdr.append(internal::CommunicationHeaderObjectType::VEC3, light_position);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void showLegend()
{
    internal::CommunicationHeader hdr{internal::Function::SHOW_LEGEND};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void holdOn()
{
    internal::CommunicationHeader hdr{internal::Function::HOLD_ON};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void softClearView()
{
    internal::CommunicationHeader hdr{internal::Function::SOFT_CLEAR};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void clearView()
{
    internal::CommunicationHeader hdr{internal::Function::CLEAR};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void disableAutomaticAxesAdjustment()
{
    internal::CommunicationHeader hdr{internal::Function::DISABLE_AXES_FROM_MIN_MAX};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setAxesBoxScaleFactor(const Vec3<double>& scale_vector)
{
    internal::CommunicationHeader hdr{internal::Function::SET_AXES_BOX_SCALE_FACTOR};
    hdr.append(internal::CommunicationHeaderObjectType::VEC3, scale_vector);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setTransform(const internal::PlotSlot slot,
                         const Vec3<double>& scale,
                         const Matrix<double>& rotation,
                         const Vec3<double>& translation)
{
    DVS_ASSERT(rotation.numRows() == 3) << "Number of rows should be 3!";
    DVS_ASSERT(rotation.numCols() == 3) << "Number of columns should be 3!";

    MatrixFixed<double, 3, 3> r_mat;

    for (size_t r = 0; r < 3; r++)
    {
        for (size_t c = 0; c < 3; c++)
        {
            r_mat(r, c) = rotation(r, c);
        }
    }

    internal::CommunicationHeader hdr{internal::Function::SET_OBJECT_TRANSFORM};
    hdr.append(internal::CommunicationHeaderObjectType::ROTATION_MATRIX, r_mat);
    hdr.append(internal::CommunicationHeaderObjectType::TRANSLATION_VECTOR, translation);
    hdr.append(internal::CommunicationHeaderObjectType::SCALE_VECTOR, scale);
    hdr.append(internal::CommunicationHeaderObjectType::SLOT, slot);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setTransform(const internal::PlotSlot slot,
                         const Vec3<double>& scale,
                         const MatrixFixed<double, 3, 3>& rotation,
                         const Vec3<double>& translation)
{
    internal::CommunicationHeader hdr{internal::Function::SET_OBJECT_TRANSFORM};
    hdr.append(internal::CommunicationHeaderObjectType::ROTATION_MATRIX, rotation);
    hdr.append(internal::CommunicationHeaderObjectType::TRANSLATION_VECTOR, translation);
    hdr.append(internal::CommunicationHeaderObjectType::SCALE_VECTOR, scale);
    hdr.append(internal::CommunicationHeaderObjectType::SLOT, slot);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

}  // namespace dvs

#endif  // DVS_DVS_H_
