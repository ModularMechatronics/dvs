#ifndef DVS_PLOT_PROPERTIES_H_
#define DVS_PLOT_PROPERTIES_H_

#include "dvs/enumerations.h"
#include "dvs/function_header_object.h"

typedef struct S_AlphaStruct
{
    PropertyType property_type;

} AlphaStruct;

#define PROPERTY_TYPE_SERIALIZATION_TYPE uint8_t

FunctionHeaderObject Color(const uint8_t red, const uint8_t green, const uint8_t blue)
{
    FunctionHeaderObject hdr_obj;

    hdr_obj.type = FHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PropertyType) + 3;
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_COLOR;
    hdr_obj.data[1] = red;
    hdr_obj.data[1] = green;
    hdr_obj.data[1] = blue;

    return hdr_obj;
}

FunctionHeaderObject Alpha(const uint8_t alpha)
{
    FunctionHeaderObject hdr_obj;

    hdr_obj.type = FHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PropertyType) + 1;
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_ALPHA;
    hdr_obj.data[1] = alpha;

    return hdr_obj;
}

#endif
