#ifndef DUOPLOT_PLOT_PROPERTIES_H_
#define DUOPLOT_PLOT_PROPERTIES_H_

#include "duoplot/communication_header_object.h"
#include "duoplot/constants.h"
#include "duoplot/enumerations.h"
#include "duoplot/pp.h"

DUOPLOT_WEAK duoplot_Property duoplot_Color(const uint8_t red, const uint8_t green, const uint8_t blue)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + 3U * sizeof(uint8_t);

    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR;
    hdr_obj.data[1U] = red;
    hdr_obj.data[2U] = green;
    hdr_obj.data[3U] = blue;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_internal_FaceColor_None()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + 3U * sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_FACE_COLOR;
    hdr_obj.data[1U] = 0U;
    hdr_obj.data[2U] = 0U;
    hdr_obj.data[3U] = 0U;
    hdr_obj.data[4U] = 0U;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_internal_EdgeColor_None()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + 3U * sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_EDGE_COLOR;
    hdr_obj.data[1U] = 0U;
    hdr_obj.data[2U] = 0U;
    hdr_obj.data[3U] = 0U;
    hdr_obj.data[4U] = 0U;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_FaceColor(const uint8_t red, const uint8_t green, const uint8_t blue)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + 3U * sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_FACE_COLOR;
    hdr_obj.data[1U] = red;
    hdr_obj.data[2U] = green;
    hdr_obj.data[3U] = blue;
    hdr_obj.data[4U] = 1U;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_EdgeColor(const uint8_t red, const uint8_t green, const uint8_t blue)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + 3U * sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_EDGE_COLOR;
    hdr_obj.data[1U] = red;
    hdr_obj.data[2U] = green;
    hdr_obj.data[3U] = blue;
    hdr_obj.data[4U] = 1U;  // TODO: use_color

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_internal_ColorMapJet()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_JET;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_internal_ColorMapHsv()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_HSV;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_internal_ColorMapMagma()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    memset(hdr_obj.data, 0, DUOPLOT_INTERNAL_MAX_NUM_FUNCTION_HEADER_BYTES);

    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_MAGMA;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_internal_ColorMapViridis()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_VIRIDIS;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_internal_ColorMapPastel()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_PASTEL;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_internal_ColorMapJetSoft()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_JET_SOFT;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_internal_ColorMapJetBright()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_JET_BRIGHT;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_Alpha(const float alpha)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(float);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_ALPHA;
    memcpy(hdr_obj.data + 1U, &alpha, sizeof(float));

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_PointSize(const int8_t point_size)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_POINT_SIZE;
    hdr_obj.data[1U] = point_size;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

DUOPLOT_WEAK duoplot_Property duoplot_LineWidth(const uint8_t line_width)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_LINE_WIDTH;
    hdr_obj.data[1U] = line_width;

    const duoplot_Property* const prop = (duoplot_Property*)(&hdr_obj);

    return *prop;
}

#define duoplot_Color_RED duoplot_Color(255, 0, 0)
#define duoplot_Color_GREEN duoplot_Color(0, 255, 0)
#define duoplot_Color_BLUE duoplot_Color(0, 0, 255)
#define duoplot_Color_BLACK duoplot_Color(0, 0, 0)
#define duoplot_Color_WHITE duoplot_Color(255, 255, 255)
#define duoplot_Color_YELLOW duoplot_Color(255, 255, 0)
#define duoplot_Color_CYAN duoplot_Color(0, 255, 255)
#define duoplot_Color_MAGENTA duoplot_Color(255, 0, 255)
#define duoplot_Color_GREY duoplot_Color(127, 127, 127)

#define duoplot_FaceColor_RED duoplot_FaceColor(255, 0, 0)
#define duoplot_FaceColor_GREEN duoplot_FaceColor(0, 255, 0)
#define duoplot_FaceColor_BLUE duoplot_FaceColor(0, 0, 255)
#define duoplot_FaceColor_BLACK duoplot_FaceColor(0, 0, 0)
#define duoplot_FaceColor_WHITE duoplot_FaceColor(255, 255, 255)
#define duoplot_FaceColor_YELLOW duoplot_FaceColor(255, 255, 0)
#define duoplot_FaceColor_CYAN duoplot_FaceColor(0, 255, 255)
#define duoplot_FaceColor_MAGENTA duoplot_FaceColor(255, 0, 255)
#define duoplot_FaceColor_GREY duoplot_FaceColor(127, 127, 127)

#define duoplot_EdgeColor_RED duoplot_EgeColor(255, 0, 0)
#define duoplot_EdgeColor_GREEN duoplot_EgeColor(0, 255, 0)
#define duoplot_EdgeColor_BLUE duoplot_EgeColor(0, 0, 255)
#define duoplot_EdgeColor_BLACK duoplot_EgeColor(0, 0, 0)
#define duoplot_EdgeColor_WHITE duoplot_EgeColor(255, 255, 255)
#define duoplot_EdgeColor_YELLOW duoplot_EgeColor(255, 255, 0)
#define duoplot_EdgeColor_CYAN duoplot_EgeColor(0, 255, 255)
#define duoplot_EdgeColor_MAGENTA duoplot_EgeColor(255, 0, 255)
#define duoplot_EdgeColor_GREY duoplot_EgeColor(127, 127, 127)

#define duoplot_ColorMap_JET duoplot_internal_ColorMapJet()
#define duoplot_ColorMap_HSV duoplot_internal_ColorMapHsv()
#define duoplot_ColorMap_MAGMA duoplot_internal_ColorMapMagma()
#define duoplot_ColorMap_VIRIDIS duoplot_internal_ColorMapViridis()
#define duoplot_ColorMap_PASTEL duoplot_internal_ColorMapPastel()
#define duoplot_ColorMap_JET_SOFT duoplot_internal_ColorMapJetSoft()
#define duoplot_ColorMap_JET_BRIGHT duoplot_internal_ColorMapJetBright()

#define duoplot_FaceColor_NONE duoplot_internal_FaceColor_None()
#define duoplot_EdgeColor_NONE duoplot_internal_EdgeColor_None()

#endif  // DUOPLOT_PLOT_PROPERTIES_H_
