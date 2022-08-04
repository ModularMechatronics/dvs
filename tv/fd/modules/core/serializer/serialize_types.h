#ifndef MODULES_CORE_SERIALIZER_SERIALIZE_TYPES_H_
#define MODULES_CORE_SERIALIZER_SERIALIZE_TYPES_H_

#include <stddef.h>
#include <stdint.h>
#include <cstring>

#include <iostream>
#include <string>

inline size_t sizeOfType(const char v)
{
    return sizeof(v);
}

inline size_t sizeOfType(const uint8_t v)
{
    return sizeof(v);
}

inline size_t sizeOfType(const uint16_t v)
{
    return sizeof(v);
}

inline size_t sizeOfType(const uint32_t v)
{
    return sizeof(v);
}

inline size_t sizeOfType(const uint64_t v)
{
    return sizeof(v);
}


inline size_t sizeOfType(const int8_t v)
{
    return sizeof(v);
}

inline size_t sizeOfType(const int16_t v)
{
    return sizeof(v);
}

inline size_t sizeOfType(const int32_t v)
{
    return sizeof(v);
}

inline size_t sizeOfType(const int64_t v)
{
    return sizeof(v);
}

inline size_t sizeOfType(const float v)
{
    return sizeof(v);
}

inline size_t sizeOfType(const double v)
{
    return sizeof(v);
}

inline size_t sizeOfType(const char* const c)
{
    // +2 to encode string length
    return strlen(c) + 2;
}

inline size_t sizeOfType(const std::string& s)
{
    // +2 to encode string length
    return s.length() + 2;
}

template <typename T>
size_t sizeOfType(const T& v)
{
    return v.serializedSize();
}

template <typename T>
void serialize(const T& t, uint8_t* const output_buffer)
{
    t.serialize(output_buffer);
}

inline void serialize(const std::string& s, uint8_t* const output_buffer)
{
    const uint16_t string_len = s.length();
    size_t idx = 0U;

    std::memcpy(output_buffer, &string_len, sizeof(uint16_t));
    idx += sizeof(uint16_t);

    // TODO: Use memcpy for string data

    for(size_t k = 0; k < s.length(); k++)
    {
        output_buffer[idx] = s[k];
        idx += 1;
    }
}

inline void serialize(const char* const s, uint8_t* const output_buffer)
{
    const uint16_t string_len = strlen(s);
    size_t idx = 0U;

    std::memcpy(output_buffer, &string_len, sizeof(uint16_t));
    idx += sizeof(uint16_t);

    // TODO: Use memcpy for string data

    for(size_t k = 0; k < string_len; k++)
    {
        output_buffer[idx] = s[k];
        idx += 1;
    }
}

inline void serialize(const char v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}

inline void serialize(const float v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}

inline void serialize(const double v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}

inline void serialize(const uint8_t v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}

inline void serialize(const uint16_t v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}

inline void serialize(const uint32_t v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}

inline void serialize(const uint64_t v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}

inline void serialize(const int8_t v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}

inline void serialize(const int16_t v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}

inline void serialize(const int32_t v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}

inline void serialize(const int64_t v, uint8_t* const output_buffer)
{
    std::memcpy(output_buffer, &v, sizeof(v));
}


template <typename T>
void deserialize(uint8_t* const input_buffer, T& t)
{
    t.deserialize(input_buffer);
}

inline void deserialize(uint8_t* const input_buffer, std::string& s)
{
    
    size_t idx = 0U;

    uint16_t string_len = s.length();
    std::memcpy(&string_len, input_buffer, sizeof(uint16_t));
    idx += sizeof(uint16_t);

    s.resize(string_len);

    for(size_t k = 0; k < s.length(); k++)
    {
        s[k] = input_buffer[idx];
        idx += 1;
    }
}

inline void deserialize(uint8_t* const input_buffer, char& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

inline void deserialize(uint8_t* const input_buffer, float& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

inline void deserialize(uint8_t* const input_buffer, double& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

inline void deserialize(uint8_t* const input_buffer, uint8_t& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

inline void deserialize(uint8_t* const input_buffer, uint16_t& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

inline void deserialize(uint8_t* const input_buffer, uint32_t& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

inline void deserialize(uint8_t* const input_buffer, uint64_t& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

inline void deserialize(uint8_t* const input_buffer, int8_t& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

inline void deserialize(uint8_t* const input_buffer, int16_t& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

inline void deserialize(uint8_t* const input_buffer, int32_t& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

inline void deserialize(uint8_t* const input_buffer, int64_t& v)
{
    std::memcpy(&v, input_buffer, sizeof(v));
}

#endif
