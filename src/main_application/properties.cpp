#include "properties.h"

#include <algorithm>

using namespace dvs;
using namespace dvs::internal;
using namespace dvs::properties;

Properties::Properties(const CommunicationHeader::PropertiesArray& props,
                       const dvs::internal::PropertyLookupTable& props_lut,
                       const CommunicationHeader::FlagsArray& flags)
    : props_{props}, props_lut_{props_lut}, flags_{flags}
{
}

bool Properties::hasFlag(const PropertyFlag f) const
{
    return flags_[static_cast<uint8_t>(f)];
}

bool Properties::hasProperty(const PropertyType tp) const
{
    return props_lut_.data[static_cast<uint8_t>(tp)] != 255;
}
