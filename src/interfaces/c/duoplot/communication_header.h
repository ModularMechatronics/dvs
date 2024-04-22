#ifndef DUOPLOT_COMMUNICATION_HEADER_H
#define DUOPLOT_COMMUNICATION_HEADER_H

#include <limits.h>
#include <stdint.h>
#include <string.h>

#include "duoplot/communication_header_object.h"
#include "duoplot/enumerations.h"
#include "duoplot/plot_properties.h"
#include "duoplot/pp.h"

#define DUOPLOT_INTERNAL_COMMUNICATION_HEADER_OBJECT_LOOKUP_TABLE_SIZE ((uint8_t)(CHOT_UNKNOWN) + 1U)
#define DUOPLOT_INTERNAL_PROPERTY_LOOKUP_TABLE_SIZE ((uint8_t)(PT_UNKNOWN) + 1U)

typedef struct S_duoplot_internal_CommunicationHeaderObjectLookupTable
{
    uint8_t data[DUOPLOT_INTERNAL_COMMUNICATION_HEADER_OBJECT_LOOKUP_TABLE_SIZE];

} duoplot_internal_CommunicationHeaderObjectLookupTable;

#define DUOPLOT_INTERNAL_NUM_FLAGS ((uint8_t)(PF_UNKNOWN) + 1U)

DUOPLOT_WEAK void initduoplot_internal_CommunicationHeaderObjectLookupTable(
    duoplot_internal_CommunicationHeaderObjectLookupTable* const lut)
{
    memset(lut->data, 255U, DUOPLOT_INTERNAL_COMMUNICATION_HEADER_OBJECT_LOOKUP_TABLE_SIZE);
}

DUOPLOT_WEAK void duoplot_internal_appendObjectIndexToCommunicationHeaderObjectLookupTable(
    duoplot_internal_CommunicationHeaderObjectLookupTable* const lut,
    const CommunicationHeaderObjectType type,
    const uint8_t idx)
{
    lut->data[(uint8_t)(type)] = idx;
}

typedef struct S_duoplot_internal_PropertyLookupTable
{
    uint8_t data[DUOPLOT_INTERNAL_PROPERTY_LOOKUP_TABLE_SIZE];
} duoplot_internal_PropertyLookupTable;

DUOPLOT_WEAK void duoplot_internal_initPropertyLookupTable(duoplot_internal_PropertyLookupTable* const lut)
{
    memset(lut->data, 255U, DUOPLOT_INTERNAL_PROPERTY_LOOKUP_TABLE_SIZE);
}

DUOPLOT_WEAK void duoplot_internal_appendPropertyIndexToPropertyLookupTable(
    duoplot_internal_PropertyLookupTable* const props_lut, const PropertyType type, const uint8_t idx)
{
    props_lut->data[(uint8_t)type] = idx;
}

typedef struct S_duoplot_internal_CommunicationHeader
{
    duoplot_internal_CommunicationHeaderObject objects[DUOPLOT_INTERNAL_MAX_NUM_HEADER_OBJECTS];
    duoplot_internal_CommunicationHeaderObject props[DUOPLOT_INTERNAL_MAX_NUM_HEADER_OBJECTS];
    Function function;
    size_t obj_idx;
    size_t prop_idx;
    duoplot_internal_CommunicationHeaderObjectLookupTable objects_lut;
    duoplot_internal_PropertyLookupTable props_lut;
    uint8_t flags[DUOPLOT_INTERNAL_NUM_FLAGS];

} duoplot_internal_CommunicationHeader;

DUOPLOT_WEAK void duoplot_internal_initCommunicationHeader(duoplot_internal_CommunicationHeader* const hdr,
                                                           const Function function)
{
    hdr->function = function;
    hdr->obj_idx = 0U;
    hdr->prop_idx = 0U;

    memset(hdr->flags, 0U, DUOPLOT_INTERNAL_NUM_FLAGS);

    initduoplot_internal_CommunicationHeaderObjectLookupTable(&(hdr->objects_lut));
    duoplot_internal_initPropertyLookupTable(&(hdr->props_lut));
}

DUOPLOT_WEAK void duoplot_internal_appendProperty(duoplot_internal_CommunicationHeader* const hdr,
                                                  const duoplot_internal_CommunicationHeaderObject* const prop)
{
    if ((hdr->prop_idx + 1U) == DUOPLOT_INTERNAL_MAX_NUM_HEADER_OBJECTS)
    {
        printf("Tried to append header objects to full duoplot_internal_CommunicationHeader!\n");
        exit(0);
    }

    const PropertyType pt = (PropertyType)(prop->data[0]);

    hdr->props[hdr->prop_idx] = *prop;
    duoplot_internal_appendPropertyIndexToPropertyLookupTable(&(hdr->props_lut), pt, hdr->prop_idx);
    hdr->prop_idx = hdr->prop_idx + 1;
}

#endif  // DUOPLOT_COMMUNICATION_HEADER_H
