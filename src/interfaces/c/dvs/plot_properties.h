#ifndef DVS_PLOT_PROPERTIES_H_
#define DVS_PLOT_PROPERTIES_H_

#include "dvs/communication_header_object.h"
#include "dvs/enumerations.h"

#define PROPERTY_TYPE_SERIALIZATION_TYPE uint8_t

CommunicationHeaderObject Color(const uint8_t red, const uint8_t green, const uint8_t blue)
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = 20U;

    hdr_obj.data[0U] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_COLOR;
    hdr_obj.data[9U + 0U] = red;
    hdr_obj.data[9U + 1U] = green;
    hdr_obj.data[9U + 2U] = blue;

    return hdr_obj;
}

CommunicationHeaderObject FaceColor(const uint8_t red, const uint8_t green, const uint8_t blue)
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 20U;
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_FACE_COLOR;
    hdr_obj.data[9U + 0U] = red;
    hdr_obj.data[9U + 1U] = green;
    hdr_obj.data[9U + 2U] = blue;

    return hdr_obj;
}

CommunicationHeaderObject EdgeColor(const uint8_t red, const uint8_t green, const uint8_t blue)
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 20U;
    hdr_obj.data[0U] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_EDGE_COLOR;
    hdr_obj.data[9U + 0U] = red;
    hdr_obj.data[9U + 1U] = green;
    hdr_obj.data[9U + 2U] = blue;

    return hdr_obj;
}

CommunicationHeaderObject ColorMapJet()
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 20U;
    hdr_obj.data[0U] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_COLOR_MAP;
    hdr_obj.data[9U] = (uint8_t)CM_JET;

    return hdr_obj;
}

CommunicationHeaderObject ColorMapHsv()
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 20U;
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_COLOR_MAP;
    hdr_obj.data[9U] = (uint8_t)CM_HSV;

    return hdr_obj;
}

CommunicationHeaderObject ColorMapMagma()
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 20U;
    memset(hdr_obj.data, 0, kMaxNumFunctionHeaderBytes);

    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_COLOR_MAP;
    hdr_obj.data[9U] = (uint8_t)CM_MAGMA;

    return hdr_obj;
}

CommunicationHeaderObject ColorMapViridis()
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 20U;
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_COLOR_MAP;
    hdr_obj.data[9U] = (uint8_t)CM_VIRIDIS;

    return hdr_obj;
}

CommunicationHeaderObject ColorMapPastel()
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 20U;
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_COLOR_MAP;
    hdr_obj.data[1] = (uint8_t)CM_PASTEL;

    return hdr_obj;
}

CommunicationHeaderObject ColorMapJetSoft()
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 20U;
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_COLOR_MAP;
    hdr_obj.data[9U] = (uint8_t)CM_JET_SOFT;

    return hdr_obj;
}

CommunicationHeaderObject ColorMapJetBright()
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 20U;
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_COLOR_MAP;
    hdr_obj.data[9U] = (uint8_t)CM_JET_BRIGHT;

    return hdr_obj;
}

CommunicationHeaderObject Alpha(const float alpha)
{
    // TODO: Alignment of data needs to be generalized and not hard coded
    // Currently for some reason the Property data in C++
    // is aligned so that it first skips 8 bytes, then 1 byte for
    // property type, then 3 bytes for 4 byte alignment if it is a float,
    // so for Alpha it starts at 12, but for 1 byte data it starts at 9
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 40U;
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_ALPHA;
    memcpy(hdr_obj.data + 12U, &alpha, sizeof(float));

    return hdr_obj;
}

CommunicationHeaderObject PointSize(const int8_t point_size)
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 20U * sizeof(uint8_t);
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_POINT_SIZE;
    hdr_obj.data[9] = point_size;

    return hdr_obj;
}

CommunicationHeaderObject LineWidth(const uint8_t line_width)
{
    CommunicationHeaderObject hdr_obj;

    hdr_obj.type = CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(PROPERTY_TYPE_SERIALIZATION_TYPE) + 127 * sizeof(uint8_t);
    hdr_obj.data[0] = (PROPERTY_TYPE_SERIALIZATION_TYPE)PT_LINE_WIDTH;
    hdr_obj.data[9] = line_width;

    return hdr_obj;
}
#endif  // DVS_PLOT_PROPERTIES_H_
