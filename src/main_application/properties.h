#ifndef MAIN_APPLICATION_PROPERTIES_H_
#define MAIN_APPLICATION_PROPERTIES_H_

#include "duoplot/internal.h"
#include "duoplot/math/math.h"

template <typename T> duoplot::internal::PropertyType templateToPropertyType()
{
    if (std::is_same<T, duoplot::properties::Alpha>::value)
    {
        return duoplot::internal::PropertyType::ALPHA;
    }
    else if (std::is_same<T, duoplot::properties::Label>::value)
    {
        return duoplot::internal::PropertyType::NAME;
    }
    else if (std::is_same<T, duoplot::properties::LineWidth>::value)
    {
        return duoplot::internal::PropertyType::LINE_WIDTH;
    }
    else if (std::is_same<T, duoplot::properties::LineStyle>::value)
    {
        return duoplot::internal::PropertyType::LINE_STYLE;
    }
    else if (std::is_same<T, duoplot::internal::ColorInternal>::value)
    {
        return duoplot::internal::PropertyType::COLOR;
    }
    else if (std::is_same<T, duoplot::properties::EdgeColor>::value)
    {
        return duoplot::internal::PropertyType::EDGE_COLOR;
    }
    else if (std::is_same<T, duoplot::properties::FaceColor>::value)
    {
        return duoplot::internal::PropertyType::FACE_COLOR;
    }
    else if (std::is_same<T, duoplot::properties::ColorMap>::value)
    {
        return duoplot::internal::PropertyType::COLOR_MAP;
    }
    else if (std::is_same<T, duoplot::properties::ScatterStyle>::value)
    {
        return duoplot::internal::PropertyType::SCATTER_STYLE;
    }
    else if (std::is_same<T, duoplot::properties::PointSize>::value)
    {
        return duoplot::internal::PropertyType::POINT_SIZE;
    }
    else if (std::is_same<T, duoplot::properties::BufferSize>::value)
    {
        return duoplot::internal::PropertyType::BUFFER_SIZE;
    }
    else if (std::is_same<T, duoplot::properties::DistanceFrom>::value)
    {
        return duoplot::internal::PropertyType::DISTANCE_FROM;
    }
    else if (std::is_same<T, duoplot::properties::ZOffset>::value)
    {
        return duoplot::internal::PropertyType::Z_OFFSET;
    }
    else if (std::is_same<T, duoplot::properties::Transform>::value)
    {
        return duoplot::internal::PropertyType::TRANSFORM;
    }
    else if (std::is_same<T, duoplot::properties::Silhouette>::value)
    {
        return duoplot::internal::PropertyType::SILHOUETTE;
    }
    else
    {
        throw std::runtime_error("Invalid property template!");
    }
}

class Properties
{
private:
    duoplot::internal::CommunicationHeader::PropertiesArray props_;
    duoplot::internal::PropertyLookupTable props_lut_;
    duoplot::internal::CommunicationHeader::FlagsArray flags_;

public:
    Properties();
    Properties(const duoplot::internal::CommunicationHeader& hdr);
    bool hasProperty(const duoplot::internal::PropertyType tp) const;
    bool hasFlag(const duoplot::internal::PropertyFlag f) const;
    void appendAndOverwriteProperties(const Properties& other_props);

    template <typename T> T getProperty() const
    {
        const duoplot::internal::PropertyType tp = templateToPropertyType<T>();

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
