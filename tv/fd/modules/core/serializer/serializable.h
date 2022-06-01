#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <time.h>
#include <cstdlib>
#include <stdint.h>

#include <vector>
#include <iostream>
#include <utility>
#include <chrono>
#include <cstring>
#include <type_traits>

#include "utils.h"
#include "serializer.h"

struct Serializable
{
    virtual size_t serializedSize() const = 0;
    virtual void serialize(uint8_t* serialized_data) const = 0;
    virtual void deserialize(const uint8_t* const serialized_data) = 0;
};

#endif
