#ifndef DVS_FUNCTION_HEADER_OBJECT_H_
#define DVS_FUNCTION_HEADER_OBJECT_H_

#include <limits.h>
#include <stdint.h>

#include "dvs/enumerations.h"

#define kMaxNumFunctionHeaderBytes SCHAR_MAX

typedef struct S_FunctionHeaderObject
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

#endif  // DVS_FUNCTION_HEADER_OBJECT_H_
