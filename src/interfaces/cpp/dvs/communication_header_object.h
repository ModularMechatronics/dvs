#ifndef DVS_COMMUNICATION_HEADER_OBJECT_H
#define DVS_COMMUNICATION_HEADER_OBJECT_H

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <stdexcept>
#include <vector>

#include "dvs/base_types.h"
#include "dvs/constants.h"
#include "dvs/enumerations.h"
#include "dvs/fillable_uint8_array.h"
#include "dvs/logging.h"
#include "dvs/math/math.h"
#include "dvs/plot_properties.h"
#include "dvs/utils.h"

namespace dvs
{
namespace internal
{

struct CommunicationHeaderObject
{
    CommunicationHeaderObjectType type;
    uint8_t size;
    uint8_t data[kCommunicationHeaderObjectDataSize];

    CommunicationHeaderObject() : type{CommunicationHeaderObjectType::UNKNOWN}, size{0U} {}
    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type) : type{input_type}, size{0U} {}

    template <typename U>
    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type, const U& input_data)
        : type{input_type}, size{sizeof(U)}
    {
        static_assert(std::is_same<U, std::int8_t>::value || std::is_same<U, std::int16_t>::value ||
                      std::is_same<U, std::int32_t>::value || std::is_same<U, std::int64_t>::value ||
                      std::is_same<U, std::uint8_t>::value || std::is_same<U, std::uint16_t>::value ||
                      std::is_same<U, std::uint32_t>::value || std::is_same<U, std::uint64_t>::value ||
                      std::is_same<U, float>::value || std::is_same<U, double>::value);
        static_assert(sizeof(U) <= kCommunicationHeaderObjectDataSize, "Object too big!");
        std::memcpy(data, reinterpret_cast<const uint8_t* const>(&input_data), size);
    }

    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type, const internal::Dimension2D& input_data)
        : type{input_type}, size{sizeof(Dimension2D)}
    {
        std::memcpy(data, reinterpret_cast<const uint8_t* const>(&input_data), size);
    }

    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type,
                              const std::pair<Vec3<double>, Vec3<double>>& input_data)
        : type{input_type}, size{sizeof(std::pair<Vec3<double>, Vec3<double>>)}
    {
        std::memcpy(data, reinterpret_cast<const uint8_t* const>(&input_data), size);
    }

    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type, const internal::DataType& input_data)
        : type{input_type}, size{sizeof(internal::DataType)}
    {
        std::memcpy(data, reinterpret_cast<const uint8_t* const>(&input_data), size);
    }

    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type, const Vec3<double>& input_data)
        : type{input_type}, size{sizeof(Vec3<double>)}
    {
        std::memcpy(data, reinterpret_cast<const uint8_t* const>(&input_data), size);
    }

    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type,
                              const MatrixFixed<double, 3, 3>& input_data)
        : type{input_type}, size{sizeof(MatrixFixed<double, 3, 3>)}
    {
        std::memcpy(data, reinterpret_cast<const uint8_t* const>(&input_data), size);
    }

    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type, const ItemId& input_data)
        : type{input_type}, size{sizeof(ItemId)}
    {
        std::memcpy(data, reinterpret_cast<const uint8_t* const>(&input_data), size);
    }

    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type, const properties::Name& input_data)
        : type{input_type}, size{sizeof(properties::Name)}
    {
        std::memcpy(data, reinterpret_cast<const uint8_t* const>(&input_data), size);
    }

    template <typename T> T as() const
    {
        static_assert(std::is_same<T, std::int8_t>::value || std::is_same<T, std::int16_t>::value ||
                      std::is_same<T, std::int32_t>::value || std::is_same<T, std::int64_t>::value ||
                      std::is_same<T, std::uint8_t>::value || std::is_same<T, std::uint16_t>::value ||
                      std::is_same<T, std::uint32_t>::value || std::is_same<T, std::uint64_t>::value ||
                      std::is_same<T, float>::value || std::is_same<T, double>::value);

        T out_val;
        std::memcpy(reinterpret_cast<uint8_t*>(&out_val), data, sizeof(T));

        return out_val;
    }

    template <> internal::Dimension2D as() const
    {
        return *reinterpret_cast<const internal::Dimension2D* const>(data);
    }

    template <> std::pair<Vec3<double>, Vec3<double>> as() const
    {
        return *reinterpret_cast<const std::pair<Vec3<double>, Vec3<double>>* const>(data);
    }

    template <> internal::DataType as() const
    {
        return *reinterpret_cast<const internal::DataType* const>(data);
    }

    template <> Vec3<double> as() const
    {
        return *reinterpret_cast<const Vec3<double>* const>(data);
    }

    template <> MatrixFixed<double, 3, 3> as() const
    {
        return *reinterpret_cast<const MatrixFixed<double, 3, 3>* const>(data);
    }

    template <> ItemId as() const
    {
        return *reinterpret_cast<const ItemId* const>(data);
    }

    template <> properties::Name as() const
    {
        return *reinterpret_cast<const properties::Name* const>(data);
    }

    template <> internal::PropertyBase as() const
    {
        return *reinterpret_cast<const internal::PropertyBase* const>(data);
    }

    template <> properties::Alpha as() const
    {
        return *reinterpret_cast<const properties::Alpha* const>(data);
    }

    template <> properties::LineWidth as() const
    {
        return *reinterpret_cast<const properties::LineWidth* const>(data);
    }

    template <> properties::Color as() const
    {
        return *reinterpret_cast<const properties::Color* const>(data);
    }

    template <> properties::EdgeColor as() const
    {
        return *reinterpret_cast<const properties::EdgeColor* const>(data);
    }

    template <> properties::FaceColor as() const
    {
        return *reinterpret_cast<const properties::FaceColor* const>(data);
    }

    template <> properties::ScatterStyle as() const
    {
        return *reinterpret_cast<const properties::ScatterStyle* const>(data);
    }

    template <> properties::ColorMap as() const
    {
        return *reinterpret_cast<const properties::ColorMap* const>(data);
    }

    template <> properties::PointSize as() const
    {
        return *reinterpret_cast<const properties::PointSize* const>(data);
    }

    template <> properties::BufferSize as() const
    {
        return *reinterpret_cast<const properties::BufferSize* const>(data);
    }

    template <> properties::DistanceFrom as() const
    {
        return *reinterpret_cast<const properties::DistanceFrom* const>(data);
    }

    template <> properties::ZOffset as() const
    {
        return *reinterpret_cast<const properties::ZOffset* const>(data);
    }

    template <> properties::Transform as() const
    {
        return *reinterpret_cast<const properties::Transform* const>(data);
    }

    template <> internal::LineStyleContainer as() const
    {
        return *reinterpret_cast<const internal::LineStyleContainer* const>(data);
    }

    template <> internal::ScatterStyleContainer as() const
    {
        return *reinterpret_cast<const internal::ScatterStyleContainer* const>(data);
    }

    template <> internal::ColorMapContainer as() const
    {
        return *reinterpret_cast<const internal::ColorMapContainer* const>(data);
    }
};

}  // namespace internal
}  // namespace dvs

#endif  // DVS_COMMUNICATION_HEADER_OBJECT_H
