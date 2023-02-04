#include "properties.h"

#include <algorithm>

using namespace dvs;
using namespace dvs::internal;
using namespace dvs::properties;

Properties::Properties()
{
    this->clear();
}

Properties::Properties(const CommunicationHeader& hdr)
    : props_{hdr.getPropertiesObjects()}, props_lut_{hdr.getPropertyLookupTable()}, flags_{hdr.getFlags()}
{
}

void Properties::appendAndOverwriteProperties(const Properties& other_props)
{
    for (size_t k = 0; k < other_props.props_.usedSize(); k++)
    {
        const PropertyType other_pt = other_props.props_[k].as<PropertyBase>().getPropertyType();

        if (hasProperty(other_pt))
        {
            const uint8_t idx = props_lut_.data[static_cast<uint8_t>(other_pt)];
            props_[idx] = other_props.props_[k];
        }
        else
        {
            props_lut_.appendPropertyIndex(other_pt, props_.usedSize());
            props_.pushBack(other_props.props_[k]);
        }
    }

    for (size_t k = 0; k < flags_.size(); k++)
    {
        flags_[k] = flags_[k] || other_props.flags_[k];
    }
}

void Properties::clear()
{
    for (size_t k = 0; k < props_.size(); k++)
    {
        CommunicationHeaderObject& obj = props_[k];
        PropertyBase* const pb = reinterpret_cast<PropertyBase*>(obj.data);
        pb->setPropertyType(PropertyType::UNKNOWN);
    }

    for (size_t k = 0; k < flags_.size(); k++)
    {
        flags_[k] = 0U;
    }

    props_lut_.clear();
    props_.clear();
}

bool Properties::hasFlag(const PropertyFlag f) const
{
    return flags_[static_cast<uint8_t>(f)];
}

bool Properties::hasProperty(const PropertyType tp) const
{
    return props_lut_.data[static_cast<uint8_t>(tp)] != 255;
}
