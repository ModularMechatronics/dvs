#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>

#include "dvs/constants.h"
#include "dvs/fillable_uint8_array.h"

// #define USE_ACK 1

inline bool ackValid(const char data[256])
{
    const bool ack_received = data[0] == 'a' && data[1] == 'c' && data[2] == 'k' && data[3] == '#' && data[4] == '\0';
    return ack_received;
}

#endif
