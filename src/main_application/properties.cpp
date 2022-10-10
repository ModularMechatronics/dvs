#include "properties.h"

#include <algorithm>

using namespace dvs;
using namespace dvs::internal;
using namespace dvs::properties;

Properties::Properties(const std::vector<CommunicationHeaderObject>& objects)
{
    for (size_t k = 0; k < objects.size(); k++)
    {
        const CommunicationHeaderObject& obj = objects[k];
        if (obj.type == CommunicationHeaderObjectType::PROPERTY)
        {
            const PropertyBase pb = obj.as<PropertyBase>();
            std::shared_ptr<PropertyBase> ptr;

            switch (pb.getPropertyType())
            {
                case PropertyType::ALPHA:
                    ptr = std::make_shared<Alpha>(obj.as<properties::Alpha>());
                    break;
                case PropertyType::NAME:
                    ptr = std::make_shared<properties::Name>(obj.as<properties::Name>());
                    break;
                case PropertyType::LINE_WIDTH:
                    ptr = std::make_shared<properties::LineWidth>(obj.as<properties::LineWidth>());
                    break;
                case PropertyType::LINE_STYLE:
                    ptr = std::make_shared<properties::LineStyle>(obj.as<properties::LineStyle>());
                    break;
                case PropertyType::COLOR:
                    ptr = std::make_shared<properties::Color>(obj.as<properties::Color>());
                    break;
                case PropertyType::EDGE_COLOR:
                    ptr = std::make_shared<properties::EdgeColor>(obj.as<properties::EdgeColor>());
                    break;
                case PropertyType::FACE_COLOR:
                    ptr = std::make_shared<properties::FaceColor>(obj.as<properties::FaceColor>());
                    break;
                case PropertyType::SCATTER_STYLE:
                    ptr = std::make_shared<properties::ScatterStyle>(obj.as<properties::ScatterStyle>());
                    break;
                case PropertyType::COLOR_MAP:
                    ptr = std::make_shared<properties::ColorMap>(obj.as<properties::ColorMap>());
                    break;
                case PropertyType::POINT_SIZE:
                    ptr = std::make_shared<properties::PointSize>(obj.as<properties::PointSize>());
                    break;
                case PropertyType::FLAG:
                    flags.push_back(obj.as<internal::PropertyFlagContainer>());
                    break;
                case PropertyType::UNKNOWN:
                    throw std::runtime_error("'UNKNOWN' type found!");
                    break;
                default:
                    throw std::runtime_error("No valid type found!");
            }

            if (pb.getPropertyType() != PropertyType::FLAG)  // TODO: Hack
            {
                props.push_back(ptr);
            }
        }
    }
}

bool Properties::hasFlag(const dvs::internal::PropertyFlag f) const
{
    return std::find_if(flags.begin(), flags.end(), [&f](const internal::PropertyFlagContainer& fc) -> bool {
               return fc.data == f;
           }) != flags.end();
}

bool Properties::hasProperty(const PropertyType tp) const
{
    return std::find_if(props.begin(), props.end(), [&tp](const std::shared_ptr<PropertyBase>& p) -> bool {
               return p->getPropertyType() == tp;
           }) != props.end();
}

std::vector<std::shared_ptr<PropertyBase>> Properties::getAllProperties() const
{
    return props;
}
