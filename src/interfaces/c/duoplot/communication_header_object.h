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
    uint8_t data[DUOPLOT_MAX_NUM_FUNCTION_HEADER_BYTES];
} CommunicationHeaderObject;

DUOPLOT_WEAK CommunicationHeaderObject getLastCommHdrObj()
{
    CommunicationHeaderObject obj;
    obj.type = CHOT_UNKNOWN;

    return obj;
}

#endif  // DUOPLOT_COMMUNICATION_HEADER_OBJECT_H
