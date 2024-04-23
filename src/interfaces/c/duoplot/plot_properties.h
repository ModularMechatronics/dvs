#ifndef DUOPLOT_PLOT_PROPERTIES_H_
#define DUOPLOT_PLOT_PROPERTIES_H_

#include "duoplot/communication_header_object.h"
#include "duoplot/constants.h"
#include "duoplot/enumerations.h"
#include "duoplot/pp.h"

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_Color(const uint8_t red,
                                                                      const uint8_t green,
                                                                      const uint8_t blue)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + 3U * sizeof(uint8_t);

    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR;
    hdr_obj.data[1U] = red;
    hdr_obj.data[2U] = green;
    hdr_obj.data[3U] = blue;

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_FaceColor(const uint8_t red,
                                                                          const uint8_t green,
                                                                          const uint8_t blue)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + 3U * sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_FACE_COLOR;
    hdr_obj.data[1U] = red;
    hdr_obj.data[2U] = green;
    hdr_obj.data[3U] = blue;
    hdr_obj.data[4U] = 1U;  // TODO: use_color

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_EdgeColor(const uint8_t red,
                                                                          const uint8_t green,
                                                                          const uint8_t blue)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + 3U * sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_EDGE_COLOR;
    hdr_obj.data[1U] = red;
    hdr_obj.data[2U] = green;
    hdr_obj.data[3U] = blue;
    hdr_obj.data[4U] = 1U;  // TODO: use_color

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_ColorMapJet()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_JET;

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_ColorMapHsv()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_HSV;

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_ColorMapMagma()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    memset(hdr_obj.data, 0, DUOPLOT_INTERNAL_MAX_NUM_FUNCTION_HEADER_BYTES);

    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_MAGMA;

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_ColorMapViridis()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_VIRIDIS;

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_ColorMapPastel()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_PASTEL;

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_ColorMapJetSoft()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_JET_SOFT;

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_ColorMapJetBright()
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_COLOR_MAP;
    hdr_obj.data[1U] = (uint8_t)DUOPLOT_INTERNAL_CM_JET_BRIGHT;

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_Alpha(const float alpha)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(float);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_ALPHA;
    memcpy(hdr_obj.data + 1U, &alpha, sizeof(float));

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_PointSize(const int8_t point_size)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_POINT_SIZE;
    hdr_obj.data[1U] = point_size;

    return hdr_obj;
}

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_LineWidth(const uint8_t line_width)
{
    duoplot_internal_CommunicationHeaderObject hdr_obj;

    hdr_obj.type = DUOPLOT_INTERNAL_CHOT_PROPERTY;
    hdr_obj.num_bytes = sizeof(uint8_t) + sizeof(uint8_t);
    hdr_obj.data[0U] = (DUOPLOT_INTERNAL_PROPERTY_TYPE_SERIALIZATION_TYPE)DUOPLOT_INTERNAL_PT_LINE_WIDTH;
    hdr_obj.data[1U] = line_width;

    return hdr_obj;
}
#endif  // DUOPLOT_PLOT_PROPERTIES_H_
