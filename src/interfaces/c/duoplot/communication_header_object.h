#ifndef DUOPLOT_COMMUNICATION_HEADER_OBJECT_H
#define DUOPLOT_COMMUNICATION_HEADER_OBJECT_H

#include <limits.h>
#include <stdint.h>

#include "duoplot/enumerations.h"

#define kMaxNumFunctionHeaderBytes SCHAR_MAX

typedef struct S_CommunicationHeaderObject
{
    CommunicationHeaderObjectType type;
    uint8_t num_bytes;
    uint8_t data[kMaxNumFunctionHeaderBytes];
} CommunicationHeaderObject;

CommunicationHeaderObject getLastCommHdrObj()
{
    CommunicationHeaderObject obj;
    obj.type = CHOT_UNKNOWN;

    return obj;
}

#endif  // DUOPLOT_COMMUNICATION_HEADER_OBJECT_H
