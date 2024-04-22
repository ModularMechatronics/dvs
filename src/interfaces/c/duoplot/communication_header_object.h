#ifndef DUOPLOT_COMMUNICATION_HEADER_OBJECT_H
#define DUOPLOT_COMMUNICATION_HEADER_OBJECT_H

#include <limits.h>
#include <stdint.h>

#include "duoplot/constants.h"
#include "duoplot/enumerations.h"
#include "duoplot/pp.h"

typedef struct S_CommunicationHeaderObject
{
    CommunicationHeaderObjectType type;
    uint8_t num_bytes;
    uint8_t data[DUOPLOT_INTERNAL_MAX_NUM_FUNCTION_HEADER_BYTES];
} duoplot_internal_CommunicationHeaderObject;

DUOPLOT_WEAK duoplot_internal_CommunicationHeaderObject duoplot_internal_getLastCommHdrObj()
{
    duoplot_internal_CommunicationHeaderObject obj;
    obj.type = CHOT_UNKNOWN;

    return obj;
}

#endif  // DUOPLOT_COMMUNICATION_HEADER_OBJECT_H
