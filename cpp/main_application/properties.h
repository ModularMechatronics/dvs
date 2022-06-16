#ifndef MAIN_APPLICATION_PROPERTIES_H_
#define MAIN_APPLICATION_PROPERTIES_H_

#include "dvs/dvs.h"

template <typename T> PropertyType templateToPropType()
{
    if (std::is_same<T, properties::Alpha>::value)
    {
        return PropertyType::ALPHA;
    }
    else if (std::is_same<T, properties::Name>::value)
    {
        return PropertyType::NAME;
    }
    else if (std::is_same<T, properties::LineWidth>::value)
    {
        return PropertyType::LINE_WIDTH;
    }
    else if (std::is_same<T, properties::LineStyle>::value)
    {
        return PropertyType::LINE_STYLE;
    }
    else if (std::is_same<T, properties::Color>::value)
    {
        return PropertyType::COLOR;
    }
    else if (std::is_same<T, properties::EdgeColor>::value)
    {
        return PropertyType::EDGE_COLOR;
    }
    else if (std::is_same<T, properties::FaceColor>::value)
    {
        return PropertyType::FACE_COLOR;
    }
    else if (std::is_same<T, properties::ColorMap>::value)
    {
        return PropertyType::COLOR_MAP;
    }
    else if (std::is_same<T, properties::PointSize>::value)
    {
        return PropertyType::POINT_SIZE;
    }
    else
    {
        throw std::runtime_error("Invalid property template!");
    }
}

class Properties
{
private:
    std::vector<std::shared_ptr<PropertyBase>> props;

public:
    Properties() = default;
    Properties(const std::vector<FunctionHeaderObject>& values)
    {
        for (size_t k = 0; k < values.size(); k++)
        {
            if (values[k].type == FunctionHeaderObjectType::PROPERTY)
            {
                const PropertyBase pb = values[k].as<PropertyBase>();
                std::shared_ptr<PropertyBase> ptr;

                switch (pb.getPropertyType())
                {
                    case PropertyType::ALPHA:
                        ptr = std::make_shared<properties::Alpha>(values[k].as<properties::Alpha>());
                        break;
                    case PropertyType::NAME:
                        ptr = std::make_shared<properties::Name>(values[k].as<properties::Name>());
                        break;
                    case PropertyType::LINE_WIDTH:
                        ptr = std::make_shared<properties::LineWidth>(values[k].as<properties::LineWidth>());
                        break;
                    case PropertyType::LINE_STYLE:
                        ptr = std::make_shared<properties::LineStyle>(values[k].as<properties::LineStyle>());
                        break;
                    case PropertyType::COLOR:
                        ptr = std::make_shared<properties::Color>(values[k].as<properties::Color>());
                        break;
                    case PropertyType::EDGE_COLOR:
                        ptr = std::make_shared<properties::EdgeColor>(values[k].as<properties::EdgeColor>());
                        break;
                    case PropertyType::FACE_COLOR:
                        ptr = std::make_shared<properties::FaceColor>(values[k].as<properties::FaceColor>());
                        break;
                    case PropertyType::COLOR_MAP:
                        ptr = std::make_shared<properties::ColorMap>(values[k].as<properties::ColorMap>());
                        break;
                    case PropertyType::POINT_SIZE:
                        ptr = std::make_shared<properties::PointSize>(values[k].as<properties::PointSize>());
                        break;
                    case PropertyType::PERSISTENT:
                        ptr = std::make_shared<PropertyBase>();
                        ptr->setPropertyType(PropertyType::PERSISTENT);
                        break;
                    case PropertyType::LINE_STRIP:
                        ptr = std::make_shared<PropertyBase>();
                        ptr->setPropertyType(PropertyType::LINE_STRIP);
                        break;
                    case PropertyType::UNKNOWN:
                        throw std::runtime_error("'UNKNOWN' type found!");
                        break;
                    default:
                        throw std::runtime_error("No valid type found!");
                }

                props.push_back(ptr);
            }
        }
    }

    bool hasProperty(const PropertyType tp) const
    {
        for (size_t k = 0; k < props.size(); k++)
        {
            if (props[k]->getPropertyType() == tp)
            {
                return true;
            }
        }
        return false;
    }

    template <typename T> T getProperty() const
    {
        const PropertyType tp = templateToPropType<T>();

        if (!hasProperty(tp))
        {
            throw std::runtime_error("Property not present!");
        }

        std::shared_ptr<T> t_ptr = std::shared_ptr<T>(new T());

        for (size_t k = 0; k < props.size(); k++)
        {
            if (props[k]->getPropertyType() == tp)
            {
                t_ptr = std::dynamic_pointer_cast<T>(props[k]);
            }
        }

        return *t_ptr;
    }

    std::vector<std::shared_ptr<PropertyBase>> getAllProperties() const
    {
        return props;
    }
};

#endif // MAIN_APPLICATION_PROPERTIES_H_
