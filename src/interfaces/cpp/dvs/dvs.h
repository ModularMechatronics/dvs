#ifndef DVS_DVS_H_
#define DVS_DVS_H_

#include <stdlib.h>

#include <functional>
#include <map>

#include "dvs/gui_api.h"
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

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT) ||
        hdr.hasPropertyFlag(internal::PropertyFlag::APPENDABLE))
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

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT) ||
        hdr.hasPropertyFlag(internal::PropertyFlag::APPENDABLE))
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

/*

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
}*/

template <typename T, typename... Us>
void plot3(
    const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Vector<RGB888>& color, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.extend(settings...);

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT))
    {
        DVS_LOG_WARNING() << "Property FAST_PLOT not available when using function \"plot3\" with color vector!";
    }

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, color);
}

template <typename T, typename... Us>
void plot3(const VectorConstView<T>& x,
           const VectorConstView<T>& y,
           const VectorConstView<T>& z,
           const VectorConstView<RGB888>& color,
           const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PLOT3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.extend(settings...);

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT))
    {
        DVS_LOG_WARNING() << "Property FAST_PLOT not available when using function \"plot3\" with color vector!";
    }

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, color);
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
void scatter(const Vector<T>& x, const Vector<T>& y, const Vector<T>& point_sizes, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_POINT_SIZES, internal::toUInt8(1));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, point_sizes);
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
void scatter(const Vector<T>& x,
             const Vector<T>& y,
             const Vector<T>& point_sizes,
             const Vector<RGB888>& color,
             const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_POINT_SIZES, internal::toUInt8(1));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, point_sizes, color);
}

template <typename T, typename... Us>
void scatter(const Vector<T>& x,
             const Vector<T>& y,
             const Vector<RGB888>& color,
             const Vector<T>& point_sizes,
             const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER2};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_POINT_SIZES, internal::toUInt8(1));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, point_sizes, color);
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

template <typename T, typename... Us>
void scatter3(const Vector<T>& x,
              const Vector<T>& y,
              const Vector<T>& z,
              const Vector<RGB888>& color,
              const Vector<T>& point_sizes,
              const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_POINT_SIZES, internal::toUInt8(1));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, point_sizes, color);
}

template <typename T, typename... Us>
void scatter3(const Vector<T>& x,
              const Vector<T>& y,
              const Vector<T>& z,
              const Vector<T>& point_sizes,
              const Vector<RGB888>& color,
              const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_COLOR, internal::toUInt8(1));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_POINT_SIZES, internal::toUInt8(1));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, point_sizes, color);
}

template <typename T, typename... Us>
void scatter3(
    const Vector<T>& x, const Vector<T>& y, const Vector<T>& z, const Vector<T>& point_sizes, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::SCATTER3};
    hdr.append(internal::CommunicationHeaderObjectType::DATA_TYPE, internal::typeToDataTypeEnum<T>());
    hdr.append(internal::CommunicationHeaderObjectType::NUM_ELEMENTS, internal::toUInt32(x.size()));
    hdr.append(internal::CommunicationHeaderObjectType::HAS_POINT_SIZES, internal::toUInt8(1));
    hdr.extend(settings...);

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, x, y, z, point_sizes);
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

template <typename T, typename... Us> void realTimePlot(const T dt, const T y, const ItemId id, const Us&... settings)
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

template <typename... Us> void setProperties(const ItemId id, const Us&... settings)
{
    internal::CommunicationHeader hdr{internal::Function::PROPERTIES_EXTENSION};
    hdr.append(internal::CommunicationHeaderObjectType::ITEM_ID, id);
    hdr.extend(settings...);

    if (hdr.hasPropertyFlag(internal::PropertyFlag::APPENDABLE))
    {
        DVS_LOG_WARNING() << "Flag APPENDABLE is not allowed for setProperties()! Set in function call instead, e.g. "
                             "scatter(x, y, properties::APPENDABLE).";
        return;
    }

    if (hdr.hasPropertyFlag(internal::PropertyFlag::FAST_PLOT))
    {
        DVS_LOG_WARNING() << "Flag FAST_PLOT is not allowed for setProperties()! Set in function call instead, e.g. "
                             "plot(x, y, properties::FAST_PLOT).";
        return;
    }

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

template <typename... Us> void setProperties(const std::vector<PropertySet>& property_sets)
{
    internal::CommunicationHeader hdr{internal::Function::PROPERTIES_EXTENSION_MULTIPLE};

    std::size_t total_size{0U};
    for (size_t k = 0; k < property_sets.size(); k++)
    {
        total_size += property_sets[k].getTotalSize();
    }

    // +1 for number of property sets
    Vector<std::uint8_t> data_vec(total_size + 1U);
    data_vec(0) = static_cast<std::uint8_t>(property_sets.size());

    std::size_t idx{1U};
    for (size_t k = 0; k < property_sets.size(); k++)
    {
        property_sets[k].fillBuffer(data_vec.data() + idx);

        idx += property_sets[k].getTotalSize();
    }

    internal::sendHeaderAndData(internal::getSendFunction(), hdr, data_vec);
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

inline void deletePlotObject(const ItemId id)
{
    internal::CommunicationHeader hdr{internal::Function::DELETE_PLOT_OBJECT};
    hdr.append(internal::CommunicationHeaderObjectType::ITEM_ID, id);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setCurrentElementToImageView()
{
    internal::CommunicationHeader hdr{internal::Function::CURRENT_ELEMENT_AS_IMAGE_VIEW};

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

inline void disableScaleOnRotation()
{
    internal::CommunicationHeader hdr{internal::Function::DISABLE_SCALE_ON_ROTATION};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void axesSquare()
{
    internal::CommunicationHeader hdr{internal::Function::AXES_SQUARE};

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setAxesBoxScaleFactor(const Vec3<double>& scale_vector)
{
    internal::CommunicationHeader hdr{internal::Function::SET_AXES_BOX_SCALE_FACTOR};
    hdr.append(internal::CommunicationHeaderObjectType::VEC3, scale_vector);

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setTitle(const std::string& title)
{
    if (title.length() == 0)
    {
        DVS_LOG_ERROR() << "Cannot set title with string length 0!";
        return;
    }

    internal::CommunicationHeader hdr{internal::Function::SET_TITLE};
    hdr.append(internal::CommunicationHeaderObjectType::TITLE_STRING, properties::Name(title.c_str()));

    internal::sendHeaderOnly(internal::getSendFunction(), hdr);
}

inline void setTransform(const ItemId id,
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

inline void setTransform(const ItemId id,
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

inline bool isDvsRunning()
{
    char path[1035];

    FILE* const fp = popen("ps -ef | grep dvs", "r");
    if (fp == NULL)
    {
        DVS_LOG_ERROR() << "Failed to run command";
        return false;
    }

    std::vector<std::string> lines;
    while (fgets(path, sizeof(path), fp) != NULL)
    {
        lines.push_back(path);
    }

    bool dvs_running = false;

    for (const std::string& line : lines)
    {
        if ((line.length() > 0U) && (line.find("grep") == std::string::npos))
        {
            dvs_running = true;
        }
    }

    pclose(fp);

    return dvs_running;
}

inline void spawn()
{
    if (!isDvsRunning())
    {
        DVS_LOG_INFO() << "Starting DVS";
        system("./main_application/dvs &");
    }
}

using GuiCallbackFunction = std::function<void(std::string)>;

namespace internal
{
inline std::map<std::string, GuiCallbackFunction>& getGuiCallbacks()
{
    static std::map<std::string, GuiCallbackFunction> gui_callbacks;

    return gui_callbacks;
}
}  // namespace internal

inline void registerGuiCallback(const std::string& handle_string, GuiCallbackFunction callback_function)
{
    std::map<std::string, GuiCallbackFunction>& gui_callbacks = internal::getGuiCallbacks();

    if (gui_callbacks.find(handle_string) != gui_callbacks.end())
    {
        DVS_LOG_WARNING() << "Gui callback with name " << handle_string << " already exists!";
        return;
    }

    gui_callbacks[handle_string] = callback_function;
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
    int tcp_sockfd, tcp_connfd;
    socklen_t tcp_len;
    struct sockaddr_in tcp_servaddr;
    struct sockaddr_in tcp_cli;

    tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set reuse address that's already in use (probably by exited dvs instance)
    int true_val = 1;
    setsockopt(tcp_sockfd, SOL_SOCKET, SO_REUSEADDR, &true_val, sizeof(int));

    bzero(&tcp_servaddr, sizeof(tcp_servaddr));

    tcp_servaddr.sin_family = AF_INET;
    tcp_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_servaddr.sin_port = htons(9758);

    if ((bind(tcp_sockfd, (struct sockaddr*)&tcp_servaddr, sizeof(tcp_servaddr))) != 0)
    {
        throw std::runtime_error("Socket bind failed...");
    }

    tcp_len = sizeof(tcp_cli);

    if ((listen(tcp_sockfd, 5)) != 0)
    {
        throw std::runtime_error("Socket listen failed...");
    }

    ////// Read
    tcp_connfd = accept(tcp_sockfd, (struct sockaddr*)&tcp_cli, &tcp_len);
    if (tcp_connfd < 0)
    {
        throw std::runtime_error("Server accept failed...");
    }

    size_t num_expected_bytes;
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
    close(tcp_sockfd);

    return std::move(received_data);
}

class ParsedGuiData
{
private:
    std::string handle_string_;

public:
    ParsedGuiData() = default;
    ParsedGuiData(const ParsedGuiData& other) = delete;
    ParsedGuiData(const ReceivedGuiData& received_gui_data)
    {
        std::uint16_t num_data_bytes;

        const std::uint8_t* const raw_data = received_gui_data.data();
        const std::uint8_t handle_string_length = raw_data[0];

        size_t idx{1U};
        for (std::uint8_t k = 0; k < handle_string_length; k++)
        {
            handle_string_.push_back(raw_data[idx]);
            idx++;
        }
    }

    std::string getHandleString() const
    {
        return handle_string_;
    }
};

inline void callGuiCallbackFunction(const std::string& handle_string)
{
    std::map<std::string, GuiCallbackFunction>& gui_callbacks = internal::getGuiCallbacks();

    if (gui_callbacks.find(handle_string) == gui_callbacks.end())
    {
        DVS_LOG_WARNING() << "Gui callback with name " << handle_string << " does not exists!";
        return;
    }

    gui_callbacks[handle_string]("Some data!");
}

inline void startGuiReceiveThread()
{
    std::thread gui_receive_thread([]() {
        while (true)
        {
            // receiveGuiData is a blocking method
            const ReceivedGuiData received_data{receiveGuiData()};

            std::cout << " ############################################################" << std::endl;
            std::cout << " ##################### Gui thread print #####################" << std::endl;
            std::cout << " ############################################################" << std::endl;

            ParsedGuiData parsed_gui_data{received_data};

            std::cout << "Handle string: \"" << parsed_gui_data.getHandleString() << "\"" << std::endl;
            callGuiCallbackFunction(parsed_gui_data.getHandleString());
        }
    });

    gui_receive_thread.detach();

    std::thread dvs_application_heart_beat_monitor_threadd([]() {
        while (true)
        {
            usleep(1000U * 1000U);
            if (isDvsRunning())
            {
                DVS_LOG_INFO() << "DVS is running!";
            }
            else
            {
                DVS_LOG_ERROR() << "DVS is not running!";
            }
        }
    });

    dvs_application_heart_beat_monitor_threadd.detach();
}

// const float radio_value = dvs::getValue<float>("slider0")

/*
Alternative solution:
 Whenever a gui element is updated on the application side, this new updated state is sent to the
 client application, which stores all the values in a map. Whenever the user calls getValue, the
 value is simply read from the map.
Functions should be named properly so that the name refects that it's the current state, and
not a state which is continuously updated.

Or could it be continously updated? If a reference/pointer is given to the returned GUI object,
then it could be updated continously.

Can a function be called at startup of client application to poll dvs for all
gui element values?

*/

/*
Prototyping:
const float f = dvs::getSlider("slider0").getValue<float>();
const float f = dvs::getGuiElement("slider0").asSlider().getValue<float>();

*/

inline float getValue(const std::string& handle_string)
{
    float ret_value;
    std::thread receive_thread([&ret_value]() {
        // TODO: Implement timout timer for this thread
        while (true)
        {
            // receiveGuiData is a blocking method
            const ReceivedGuiData received_data{receiveGuiData()};
            std::memcpy(&ret_value, received_data.data(), sizeof(float));
        }
    });

    receive_thread.join();

    std::cout << "ret_value: " << ret_value << std::endl;
}

namespace not_ready
{
inline size_t numObjectsInReceiveBuffer()
{
    internal::CommunicationHeader hdr{internal::Function::IS_BUSY_RENDERING};

    // internal::sendHeaderOnly(internal::sendThroughQueryUdpInterface, hdr);

    usleep(1000 * 40);

    const size_t d = internal::receiveFromQueryUdpInterface();

    std::cout << d << std::endl;

    return 0;
}
}  // namespace not_ready

}  // namespace dvs

#endif  // DVS_DVS_H_
