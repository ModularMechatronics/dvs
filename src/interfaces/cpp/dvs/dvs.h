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

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT))
    {
        hdr.setFunction(internal::Function::FAST_PLOT2);
    }

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y);
}

template <typename T, typename... Us>
void plot(const VectorConstView<T>& x, const VectorConstView<T>& y, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT))
    {
        hdr.setFunction(internal::Function::FAST_PLOT2);
    }

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

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT))
    {
        DVS_LOG_WARNING() << "Property FAST_PLOT not available when using function \"plot\" with color vector!";
    }

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, color);
}

template <typename T, typename... Us>
void plot(const VectorConstView<T>& x,
          const VectorConstView<T>& y,
          const VectorConstView<RGB888>& color,
          const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.extend(settings...);

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT))
    {
        DVS_LOG_WARNING() << "Property FAST_PLOT not available when using function \"plot\" with color vector!";
    }

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, color);
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
void lineCollection(const VectorConstView<T>& x, const VectorConstView<T>& y, const Us&... settings)
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
void lineCollection3(const VectorConstView<T>& x,
                     const VectorConstView<T>& y,
                     const VectorConstView<T>& z,
                     const Us&... settings)
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
void plotCollection(const std::vector<VectorConstView<T>>& x,
                    const std::vector<VectorConstView<T>>& y,
                    const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT_COLLECTION2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_OBJECTS, internal::toUInt32(x.size()));

    DVS_ASSERT(x.size() == y.size());

    uint32_t num_elements = 0;
    Vector<uint16_t> vector_lengths(x.size());

    for (size_t k = 0; k < x.size(); k++)
    {
        const VectorConstView<T> x_view = x[k];
        const VectorConstView<T> y_view = y[k];
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
void plotCollection3(const std::vector<VectorConstView<T>>& x,
                     const std::vector<VectorConstView<T>>& y,
                     const std::vector<VectorConstView<T>>& z,
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
        const VectorConstView<T> x_view = x[k];
        const VectorConstView<T> y_view = y[k];
        const VectorConstView<T> z_view = z[k];
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

template <typename T, typename... Us>
void stairs(const VectorConstView<T>& x, const VectorConstView<T>& y, const Us&... settings)
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

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT))
    {
        hdr.setFunction(internal::Function::FAST_PLOT3);
    }

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us>
void plot3(const VectorConstView<T>& x, const VectorConstView<T>& y, const VectorConstView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.extend(settings...);

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT))
    {
        hdr.setFunction(internal::Function::FAST_PLOT3);
    }

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

template <typename T, typename... Us>
void stem(const VectorConstView<T>& x, const VectorConstView<T>& y, const Us&... settings)
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
void scatter(const VectorConstView<T>& x,
             const VectorConstView<T>& y,
             const VectorConstView<RGB888>& color,
             const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, color);
}

template <typename T, typename... Us>
void scatter(const VectorConstView<T>& x, const VectorConstView<T>& y, const Us&... settings)
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
void scatter3(const VectorConstView<T>& x,
              const VectorConstView<T>& y,
              const VectorConstView<T>& z,
              const Us&... settings)
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
void surf(const MatrixConstView<T>& x, const MatrixConstView<T>& y, const MatrixConstView<T>& z, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SURF};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D, internal::Dimension2D(x.numRows(), x.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z);
}

template <typename T, typename... Us> void imShow(const ImageGray<T>& img, const Us&... settings)
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, uint8_t>::value || std::is_same<T, double>::value,
                  "Only float, double and uint8_t supported for imShow!");

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

template <typename T, typename... Us> void imShow(const ImageGrayAlpha<T>& img, const Us&... settings)
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, uint8_t>::value || std::is_same<T, double>::value,
                  "Only float, double and uint8_t supported for imShow!");

    internal::CommunicationHeader hdr{internal::Function::IM_SHOW};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_CHANNELS, internal::toUInt8(2));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(img.numElements()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D,
               internal::Dimension2D(img.numRows(), img.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename T, typename... Us> void imShow(const ImageRGB<T>& img, const Us&... settings)
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, uint8_t>::value || std::is_same<T, double>::value,
                  "Only float, double and uint8_t supported for imShow!");

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

template <typename T, typename... Us> void imShow(const ImageRGBA<T>& img, const Us&... settings)
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, uint8_t>::value || std::is_same<T, double>::value,
                  "Only float, double and uint8_t supported for imShow!");

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

template <typename T, typename... Us> void imShow(const ImageGrayConstView<T>& img, const Us&... settings)
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, uint8_t>::value || std::is_same<T, double>::value,
                  "Only float, double and uint8_t supported for imShow!");

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

template <typename T, typename... Us> void imShow(const ImageGrayAlphaConstView<T>& img, const Us&... settings)
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, uint8_t>::value || std::is_same<T, double>::value,
                  "Only float, double and uint8_t supported for imShow!");

    internal::CommunicationHeader hdr{internal::Function::IM_SHOW};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_CHANNELS, internal::toUInt8(2));
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS,
               internal::toUInt32(img.numElements()));  // TODO: Needed?
    hdr.append(internal::CommunicationHeaderObjectType::DIMENSION_2D,
               internal::Dimension2D(img.numRows(), img.numCols()));

    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, img);
}

template <typename T, typename... Us> void imShow(const ImageRGBConstView<T>& img, const Us&... settings)
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, uint8_t>::value || std::is_same<T, double>::value,
                  "Only float, double and uint8_t supported for imShow!");

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

template <typename T, typename... Us> void imShow(const ImageRGBAConstView<T>& img, const Us&... settings)
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, uint8_t>::value || std::is_same<T, double>::value,
                  "Only float, double and uint8_t supported for imShow!");

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
void drawMesh(const VectorConstView<Point3<T>>& vertices,
              const VectorConstView<IndexTriplet>& indices,
              const Us&... settings)
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
void drawMesh(const VectorConstView<T>& x,
              const VectorConstView<T>& y,
              const VectorConstView<T>& z,
              const VectorConstView<IndexTriplet>& indices,
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
void realTimePlot(const T dt, const T y, const internal::ItemId id, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::REAL_TIME_PLOT};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(1U));
    hdr.append(internal::CommunicationHeaderObjectType::ITEM_ID, id);
    hdr.extend(settings...);

    const Vector<T> data{VectorInitializer<T>{dt, y}};

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, data);
}

/*template <typename T, typename... Us> void drawCube(const Us&... settings)
{
    const Vector<T> x{VectorInitializer<T>{-1, 1, 1, -1, -1, 1, 1, -1}};
    const Vector<T> y{VectorInitializer<T>{-1, -1, 1, 1, -1, -1, 1, 1}};
    const Vector<T> z{VectorInitializer<T>{-1, -1, -1, -1, 1, 1, 1, 1}};
    Vector<IndexTriplet> indices;

    drawMesh(x, y, z, indices, settings...);
}*/

template <typename... Us> void setProperties(const internal::ItemId id, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PROPERTIES_EXTENSION};
    hdr.append(internal::CommunicationHeaderObjectType::ITEM_ID, id);
    hdr.extend(settings...);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setCurrentElement(const std::string& name)
{
    if (name.length() == 0)
    {
        DVS_LOG_ERROR() << "Cannot set element name with string length 0!";
        return;
    }

    internal::CommunicationHeader hdr{internal::Function::SET_CURRENT_ELEMENT};
    hdr.append(internal::CommunicationHeaderObjectType::ELEMENT_NAME, properties::Name(name.c_str()));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void deletePlotObject(const internal::ItemId id)
{
    internal::CommunicationHeader hdr{internal::Function::DELETE_PLOT_OBJECT};
    hdr.append(internal::CommunicationHeaderObjectType::ITEM_ID, id);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void waitForFlush()
{
    internal::CommunicationHeader hdr{internal::Function::WAIT_FOR_FLUSH};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void flushCurrentElement()
{
    internal::CommunicationHeader hdr{internal::Function::FLUSH_ELEMENT};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

namespace internal
{

inline void flushMultipleElementsInternal(std::vector<std::string>& extracted_elements, const std::string& elem)
{
    extracted_elements.push_back(elem);
}

template <typename... Us>
void flushMultipleElementsInternal(std::vector<std::string>& extracted_elements,
                                   const std::string& elem,
                                   const Us&... elements)
{
    extracted_elements.push_back(elem);

    internal::flushMultipleElementsInternal(extracted_elements, elements...);
}

}  // namespace internal

template <typename... Us> void flushMultipleElements(const Us&... elements)
{
    internal::CommunicationHeader hdr{internal::Function::FLUSH_MULTIPLE_ELEMENTS};

    std::vector<std::string> extracted_elements;

    internal::flushMultipleElementsInternal(extracted_elements, elements...);

    Vector<uint8_t> name_lengths{extracted_elements.size()};

    std::vector<char> concatenated_names;

    for (size_t k = 0; k < extracted_elements.size(); k++)
    {
        name_lengths(k) = static_cast<uint8_t>(extracted_elements[k].length());
        for (size_t i = 0; i < extracted_elements[k].length(); i++)
        {
            concatenated_names.push_back(extracted_elements[k][i]);
        }
    }

    Vector<char> concatenated_names_local = concatenated_names;

    hdr.append(internal::CommunicationHeaderObjectType::NUM_NAMES, internal::toUInt8(extracted_elements.size()));

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, name_lengths, concatenated_names_local);
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

inline void setTransform(const internal::ItemId id,
                         const Matrix<double>& scale,
                         const Matrix<double>& rotation,
                         const Vec3<double>& translation)
{
    DVS_ASSERT(rotation.numRows() == 3) << "Number of rows should be 3!";
    DVS_ASSERT(rotation.numCols() == 3) << "Number of columns should be 3!";

    DVS_ASSERT(scale.numRows() == 3) << "Number of rows should be 3!";
    DVS_ASSERT(scale.numCols() == 3) << "Number of columns should be 3!";

    MatrixFixed<double, 3, 3> r_mat, scale_mat;

    for (size_t r = 0; r < 3; r++)
    {
        for (size_t c = 0; c < 3; c++)
        {
            r_mat(r, c) = rotation(r, c);
            scale_mat(r, c) = scale(r, c);
        }
    }

    internal::CommunicationHeader hdr{internal::Function::SET_OBJECT_TRANSFORM};
    hdr.append(internal::CommunicationHeaderObjectType::ROTATION_MATRIX, r_mat);
    hdr.append(internal::CommunicationHeaderObjectType::TRANSLATION_VECTOR, translation);
    hdr.append(internal::CommunicationHeaderObjectType::SCALE_MATRIX, scale_mat);
    hdr.append(internal::CommunicationHeaderObjectType::ITEM_ID, id);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void openProjectFile(const std::string& file_path)
{
    internal::CommunicationHeader hdr{internal::Function::OPEN_PROJECT_FILE};
    hdr.append(internal::CommunicationHeaderObjectType::PROJECT_FILE_NAME, properties::Name(file_path.c_str()));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setTransform(const internal::ItemId id,
                         const MatrixFixed<double, 3, 3>& scale,
                         const MatrixFixed<double, 3, 3>& rotation,
                         const Vec3<double>& translation)
{
    internal::CommunicationHeader hdr{internal::Function::SET_OBJECT_TRANSFORM};
    hdr.append(internal::CommunicationHeaderObjectType::ROTATION_MATRIX, rotation);
    hdr.append(internal::CommunicationHeaderObjectType::TRANSLATION_VECTOR, translation);
    hdr.append(internal::CommunicationHeaderObjectType::SCALE_MATRIX, scale);
    hdr.append(internal::CommunicationHeaderObjectType::ITEM_ID, id);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

namespace not_ready
{
inline size_t numObjectsInReceiveBuffer()
{
    internal::CommunicationHeader hdr{internal::Function::IS_BUSY_RENDERING};

    internal::sendHeaderOnly(internal::sendThroughQueryUdpInterface, hdr);

    usleep(1000 * 40);

    const size_t d = internal::receiveFromQueryUdpInterface();

    std::cout << d << std::endl;

    return 0;
}
}  // namespace not_ready

}  // namespace dvs

#endif  // DVS_DVS_H_
