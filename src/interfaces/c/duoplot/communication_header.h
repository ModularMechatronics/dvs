#ifndef DUOPLOT_COMMUNICATION_HEADER_H
#define DUOPLOT_COMMUNICATION_HEADER_H

#include <limits.h>
#include <stdint.h>
#include <string.h>

#include "duoplot/communication_header_object.h"
#include "duoplot/enumerations.h"
#include "duoplot/plot_properties.h"
#include "duoplot/pp.h"

#define COMMUNICATION_HEADER_OBJECT_LOOKUP_TABLE_SIZE ((uint8_t)(CHOT_UNKNOWN) + 1U)
#define PROPERTY_LOOKUP_TABLE_SIZE ((uint8_t)(PT_UNKNOWN) + 1U)

typedef struct S_CommunicationHeaderObjectLookupTable
{
    uint8_t data[COMMUNICATION_HEADER_OBJECT_LOOKUP_TABLE_SIZE];

} CommunicationHeaderObjectLookupTable;

#define NUM_FLAGS ((uint8_t)(PF_UNKNOWN) + 1U)

DUOPLOT_WEAK void initCommunicationHeaderObjectLookupTable(CommunicationHeaderObjectLookupTable* const lut)
{
    memset(lut->data, 255U, COMMUNICATION_HEADER_OBJECT_LOOKUP_TABLE_SIZE);
}

DUOPLOT_WEAK void appendObjectIndexToCommunicationHeaderObjectLookupTable(
    CommunicationHeaderObjectLookupTable* const lut, const CommunicationHeaderObjectType type, const uint8_t idx)
{
    lut->data[(uint8_t)(type)] = idx;
}

typedef struct S_PropertyLookupTable
{
    uint8_t data[PROPERTY_LOOKUP_TABLE_SIZE];
} PropertyLookupTable;

DUOPLOT_WEAK void initPropertyLookupTable(PropertyLookupTable* const lut)
{
    memset(lut->data, 255U, PROPERTY_LOOKUP_TABLE_SIZE);
}

DUOPLOT_WEAK void appendPropertyIndexToPropertyLookupTable(PropertyLookupTable* const props_lut,
                                                           const PropertyType type,
                                                           const uint8_t idx)
{
    props_lut->data[(uint8_t)type] = idx;
}

typedef struct S_CommunicationHeader
{
    CommunicationHeaderObject objects[DUOPLOT_MAX_NUM_HEADER_OBJECTS];
    CommunicationHeaderObject props[DUOPLOT_MAX_NUM_HEADER_OBJECTS];
    Function function;
    size_t obj_idx;
    size_t prop_idx;
    CommunicationHeaderObjectLookupTable objects_lut;
    PropertyLookupTable props_lut;
    uint8_t flags[NUM_FLAGS];

} CommunicationHeader;

DUOPLOT_WEAK void initCommunicationHeader(CommunicationHeader* const hdr, const Function function)
{
    hdr->function = function;
    hdr->obj_idx = 0U;
    hdr->prop_idx = 0U;

    memset(hdr->flags, 0U, NUM_FLAGS);

    initCommunicationHeaderObjectLookupTable(&(hdr->objects_lut));
    initPropertyLookupTable(&(hdr->props_lut));
}

DUOPLOT_WEAK void appendProperty(CommunicationHeader* const hdr, const CommunicationHeaderObject* const prop)
{
    if ((hdr->prop_idx + 1U) == DUOPLOT_MAX_NUM_HEADER_OBJECTS)
    {
        printf("Tried to append header objects to full CommunicationHeader!\n");
        exit(0);
    }

    const PropertyType pt = (PropertyType)(prop->data[0]);

    hdr->props[hdr->prop_idx] = *prop;
    appendPropertyIndexToPropertyLookupTable(&(hdr->props_lut), pt, hdr->prop_idx);
    hdr->prop_idx = hdr->prop_idx + 1;
}

#endif  // DUOPLOT_COMMUNICATION_HEADER_H
