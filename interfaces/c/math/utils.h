#ifndef DVS_MATH_UTILS_H_
#define DVS_MATH_UTILS_H_

#include <stdint.h>

#include "enumerations.h"

size_t dataTypeToNumBytes(const DataType dt)
{
    switch (dt)
    {
        case DT_FLOAT:
            return sizeof(float);
            break;
        case DT_DOUBLE:
            return sizeof(double);
            break;
        case DT_UINT8:
            return sizeof(uint8_t);
            break;
        case DT_UINT16:
            return sizeof(uint16_t);
            break;
        case DT_UINT32:
            return sizeof(uint32_t);
            break;
        case DT_UINT64:
            return sizeof(uint64_t);
            break;
        case DT_INT8:
            return sizeof(int8_t);
            break;
        case DT_INT16:
            return sizeof(int16_t);
            break;
        case DT_INT32:
            return sizeof(int32_t);
            break;
        case DT_INT64:
            return sizeof(int64_t);
            break;
        default:
            exit(-1);  // Invalid data type
            break;
    }
}

#endif
