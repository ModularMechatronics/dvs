#ifndef SERIALIZABLE_TYPES_H_
#define SERIALIZABLE_TYPES_H_

#include <type_traits>

#include <stdint.h>

#include "forward_decl.h"

/*enum class SerializableType : uint16_t
{
    INT = 0,
    UINT8,
    VECTOR3DF,
    GPS_DATA
};

template <typename T>
constexpr SerializableType templateToSerializableType()
{
    if(std::is_same<T, Vector3Df>::value)
    {
        return SerializableType::VECTOR3DF;
    }
    else if(std::is_same<T, GpsData>::value)
    {
        return SerializableType::GPS_DATA;
    }
    else
    {
        std::cout << "Nooo!!" << std::endl;
        exit(-1);
    }
}*/

#endif
