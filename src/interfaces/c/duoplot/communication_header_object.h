#ifndef DUOPLOT_COMMUNICATION_HEADER_OBJECT_H
#define DUOPLOT_COMMUNICATION_HEADER_OBJECT_H

#include <limits.h>
#include <stdint.h>

#include "duoplot/constants.h"
#include "duoplot/enumerations.h"
#include "duoplot/pp.h"

typedef struct S_duoplot_internal_CommunicationHeaderObject
{
    CommunicationHeaderObjectType type;
    uint8_t num_bytes;
    uint8_t data[DUOPLOT_INTERNAL_MAX_NUM_FUNCTION_HEADER_BYTES];
} duoplot_internal_CommunicationHeaderObject;

typedef struct S_duoplot_Property
{
    uint8_t data[sizeof(duoplot_internal_CommunicationHeaderObject)];
} duoplot_Property;

DUOPLOT_WEAK duoplot_Property duoplot_internal_getLastCommHdrObj()
{
    duoplot_internal_CommunicationHeaderObject obj;
    obj.type = CHOT_UNKNOWN;

    const duoplot_Property* const prop = (duoplot_Property*)(&obj);

    return *prop;
}

#endif  // DUOPLOT_COMMUNICATION_HEADER_OBJECT_H
