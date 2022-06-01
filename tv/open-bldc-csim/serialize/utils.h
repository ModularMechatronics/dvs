#ifndef SERIALIZE_UTILS_H_
#define SERIALIZE_UTILS_H_

template <typename T> constexpr bool isNativeType()
{
    return std::is_same<T, float>::value || std::is_same<T, double>::value ||
        std::is_same<T, uint64_t>::value || std::is_same<T, int64_t>::value ||
        std::is_same<T, uint32_t>::value || std::is_same<T, int32_t>::value ||
        std::is_same<T, uint16_t>::value || std::is_same<T, int16_t>::value ||
        std::is_same<T, uint8_t>::value || std::is_same<T, int8_t>::value;
}


#endif
