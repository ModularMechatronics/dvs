#ifndef MAIN_APPLICATION_PROPERTIES_H_
#define MAIN_APPLICATION_PROPERTIES_H_

#include "dvs/dvs.h"

template <typename T> dvs::internal::PropertyType templateToPropertyType()
{
    if (std::is_same<T, dvs::properties::Alpha>::value)
    {
        return dvs::internal::PropertyType::ALPHA;
    }
    else if (std::is_same<T, dvs::properties::Name>::value)
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
    else if (std::is_same<T, dvs::properties::Color>::value)
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
    else
    {
        throw std::runtime_error("Invalid property template!");
    }
}

class Properties
{
private:
    std::vector<std::shared_ptr<dvs::internal::PropertyBase>> props;
    std::vector<dvs::internal::PropertyFlagContainer> flags;

public:
    Properties() = default;
    Properties(const std::vector<dvs::internal::CommunicationHeaderObject>& objects);
    bool hasProperty(const dvs::internal::PropertyType tp) const;
    bool hasFlag(const dvs::internal::PropertyFlag f) const;
    template <typename T> T getProperty() const
    {
        // TODO: Refactor
        const dvs::internal::PropertyType tp = templateToPropertyType<T>();

        if (!hasProperty(tp))
        {
            throw std::runtime_error("Property not present!");
        }

        std::shared_ptr<T> t_ptr = std::make_shared<T>();

        for (size_t k = 0; k < props.size(); k++)
        {
            if (props[k]->getPropertyType() == tp)
            {
                t_ptr = std::dynamic_pointer_cast<T>(props[k]);
            }
        }

        return *t_ptr;
    }

    std::vector<std::shared_ptr<dvs::internal::PropertyBase>> getAllProperties() const;
};

#endif  // MAIN_APPLICATION_PROPERTIES_H_
