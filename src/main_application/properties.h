#ifndef MAIN_APPLICATION_PROPERTIES_H_
#define MAIN_APPLICATION_PROPERTIES_H_

#include "dvs/internal.h"
#include "dvs/math/math.h"

template <typename T> dvs::internal::PropertyType templateToPropertyType()
{
    if (std::is_same<T, dvs::properties::Alpha>::value)
    {
        return dvs::internal::PropertyType::ALPHA;
    }
    else if (std::is_same<T, dvs::properties::Label>::value)
    {
        return dvs::internal::PropertyType::NAME;
    }
    else if (std::is_same<T, dvs::properties::LineWidth>::value)
    {
        return dvs::internal::PropertyType::LINE_WIDTH;
    }
    else if (std::is_same<T, dvs::properties::LineStyle>::value)
    {
        return dvs::internal::PropertyType::LINE_STYLE;
    }
    else if (std::is_same<T, dvs::internal::ColorInternal>::value)
    {
        return dvs::internal::PropertyType::COLOR;
    }
    else if (std::is_same<T, dvs::properties::EdgeColor>::value)
    {
        return dvs::internal::PropertyType::EDGE_COLOR;
    }
    else if (std::is_same<T, dvs::properties::FaceColor>::value)
    {
        return dvs::internal::PropertyType::FACE_COLOR;
    }
    else if (std::is_same<T, dvs::properties::ColorMap>::value)
    {
        return dvs::internal::PropertyType::COLOR_MAP;
    }
    else if (std::is_same<T, dvs::properties::ScatterStyle>::value)
    {
        return dvs::internal::PropertyType::SCATTER_STYLE;
    }
    else if (std::is_same<T, dvs::properties::PointSize>::value)
    {
        return dvs::internal::PropertyType::POINT_SIZE;
    }
    else if (std::is_same<T, dvs::properties::BufferSize>::value)
    {
        return dvs::internal::PropertyType::BUFFER_SIZE;
    }
    else if (std::is_same<T, dvs::properties::DistanceFrom>::value)
    {
        return dvs::internal::PropertyType::DISTANCE_FROM;
    }
    else if (std::is_same<T, dvs::properties::ZOffset>::value)
    {
        return dvs::internal::PropertyType::Z_OFFSET;
    }
    else if (std::is_same<T, dvs::properties::Transform>::value)
    {
        return dvs::internal::PropertyType::TRANSFORM;
    }
    else
    {
        throw std::runtime_error("Invalid property template!");
    }
}

class Properties
{
private:
    dvs::internal::CommunicationHeader::PropertiesArray props_;
    dvs::internal::PropertyLookupTable props_lut_;
    dvs::internal::CommunicationHeader::FlagsArray flags_;

public:
    Properties();
    Properties(const dvs::internal::CommunicationHeader& hdr);
    bool hasProperty(const dvs::internal::PropertyType tp) const;
    bool hasFlag(const dvs::internal::PropertyFlag f) const;
    void appendAndOverwriteProperties(const Properties& other_props);

    template <typename T> T getProperty() const
    {
        const dvs::internal::PropertyType tp = templateToPropertyType<T>();

        const uint8_t idx = props_lut_.data[static_cast<uint8_t>(tp)];

        if (idx == 255U)
        {
            throw std::runtime_error("Property not present!");
        }

        return props_[idx].as<T>();
    }

    bool isEmpty() const
    {
        if (props_.usedSize() > 0)
        {
            return false;
        }

        for (size_t k = 0; k < flags_.size(); k++)
        {
            if (flags_[k] == static_cast<uint8_t>(1U))
            {
                return false;
            }
        }

        return true;
    }

    void clear();

    size_t numProperties() const
    {
        return props_.usedSize();
    }
};

#endif  // MAIN_APPLICATION_PROPERTIES_H_
