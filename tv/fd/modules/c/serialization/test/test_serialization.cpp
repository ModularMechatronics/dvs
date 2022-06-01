#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "serialization.h"

class TestSerializationC : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

/*
TODO: Create enum of different types that can be serialized, hash of enum source
code is "version" of serialization?
*/

enum SerializationType
{
    ST_FLOAT,
    ST_DOUBLE,
    ST_UINT8,
    ST_UINT16,
    ST_UINT32,
    ST_UINT64,
    ST_INT8,
    ST_INT16,
    ST_INT32,
    ST_INT64
};

bool isVariableSize(const SerializationType st)
{
    switch(st)
    {
        case ST_FLOAT:
        case ST_DOUBLE:
        case ST_UINT8:
        case ST_UINT16:
        case ST_UINT32:
        case ST_UINT64:
        case ST_INT8:
        case ST_INT16:
        case ST_INT32:
        case ST_INT64:
            return false;
        break;
        default:
            exit(0);
    }
    return false;
}

struct SerializationBuffer
{
    uint8_t* data;
    size_t idx;
    size_t capacity;
    size_t num_elements;
};

void incrementIndexWithType(SerializationBuffer* const sb)
{
    sb->idx += sizeof(ST_FLOAT);
}

SerializationType readNextTypeInBuffer(SerializationBuffer* const sb)
{
    SerializationType st;
    uint8_t* const st_ptr = (uint8_t* const)(&st);

    memcpy(st_ptr, sb->data, sizeof(ST_FLOAT));

    sb->idx += sizeof(ST_FLOAT);

    return st;
}

SerializationType peekNextTypeInBuffer(const SerializationBuffer sb)
{
    SerializationType st;
    uint8_t* const st_ptr = (uint8_t* const)(&st);

    memcpy(st_ptr, sb.data, sizeof(ST_FLOAT));

    return st;
}

SerializationBuffer createSerializationBuffer(const size_t capacity)
{
    SerializationBuffer sb;
    sb.data = (uint8_t*)malloc(sizeof(uint8_t) * capacity);
    sb.idx = 0;
    sb.capacity = capacity;
    sb.num_elements = 0;

    return sb;
}

void deleteSerializationBuffer(SerializationBuffer* sb)
{
    free(sb->data);
    sb->idx = 0;
    sb->capacity = 0;
}

#define ENSURE_BUFFER_SIZE(__buffer_, __num_bytes_to_expand_with_) {         \
    if(((__buffer_)->idx + (__num_bytes_to_expand_with_)) > (__buffer_)->capacity)  \
    {                                                                        \
        (__buffer_)->capacity *= 2;                                             \
        uint8_t* const __tmp_ptr = (uint8_t*)malloc((__buffer_)->capacity);           \
        memcpy(__tmp_ptr, (__buffer_)->data, (__buffer_)->idx);                    \
        free((__buffer_)->data);                                                \
        (__buffer_)->data = __tmp_ptr;                                          \
    }                                                                        \
}

#define SERIALIZE_STATIC_TYPE(val, sb, st_type, type)               \
{                                                                   \
    /*ENSURE_BUFFER_SIZE(sb, sizeof(type) + sizeof(st_type));*/     \
                                                                    \
    const SerializationType __st = st_type;                           \
    const uint8_t* const __st_ptr = (const uint8_t* const)(&__st);      \
                                                                    \
    memcpy(sb->data + sb->idx, __st_ptr, sizeof(st_type));            \
    sb->idx += sizeof(st_type);                                     \
                                                                   \
    const uint8_t* const __f_ptr = (const uint8_t* const)(&val);      \
                                                                    \
    memcpy(sb->data + sb->idx, __f_ptr, sizeof(type));                 \
    sb->idx += sizeof(type);                                         \
}

void serializeInt8(const int8_t i, SerializationBuffer* const sb)
{
    SERIALIZE_STATIC_TYPE(i, sb, ST_INT8, int8_t);
}

void serializeFloat(const float f, SerializationBuffer* const sb)
{
    SERIALIZE_STATIC_TYPE(f, sb, ST_FLOAT, float);
}

float deserializeFloat(SerializationBuffer* const sb)
{
    float f;
    uint8_t* const f_ptr = (uint8_t* const)(&f);

    memcpy(f_ptr, sb->data + sb->idx, sizeof(float));
    sb->idx += sizeof(float);

    return f;
}

void writeSerializationBufferToFile(const SerializationBuffer sb, const char* const file_name)
{
    FILE *fptr = fopen(file_name, "wb");
    const uint8_t* const num_bytes_ptr = (const uint8_t* const)(&(sb.idx));
    fwrite(num_bytes_ptr, sizeof(sb.idx), 1, fptr);

    const uint8_t* const num_elements_ptr = (const uint8_t* const)(&(sb.num_elements));
    fwrite(num_elements_ptr, sizeof(sb.num_elements), 1, fptr);

    fwrite(sb.data, sb.idx, 1, fptr);
    fclose(fptr);
}

void readFileToSerializationBuffer(SerializationBuffer* sb, const char* const file_name)
{
    FILE *fptr = fopen(file_name, "rb");

    fread(&(sb->idx), sizeof(sb->idx), 1, fptr);
    fread(&(sb->num_elements), sizeof(sb->num_elements), 1, fptr);
    sb->data = (uint8_t*)malloc(sizeof(uint8_t) * sb->idx);

    fread(sb->data, sizeof(sb->num_elements), 1, fptr);
    sb->capacity = sb->idx;
    sb->idx = 0;
    std::cout << "Num elements: " << sb->num_elements << std::endl;
    std::cout << "Size: " << sb->capacity << std::endl;

    fclose(fptr);
}

#define appendToBuffer(val_to_write, sb)                                   \
    ENSURE_BUFFER_SIZE(sb, sizeof(val_to_write));                          \
    const uint8_t* const __f_ptr = (const uint8_t* const)(&val_to_write);  \
                                                                           \
    memcpy((sb)->data + (sb)->idx, __f_ptr, sizeof(val_to_write));         \
    (sb)->idx += sizeof(val_to_write);                                     \
    (sb)->num_elements += 1;

TEST_F(TestSerializationC, TestFile)
{
    const float f_exp[5] = {0.34f, 0.62f, 2.42f, 0.67f, 0.001f};
    const size_t num_vals = sizeof(f_exp) / sizeof(float);
    SerializationBuffer sb = createSerializationBuffer(sizeof(f_exp) * 3);

    for(size_t k = 0; k < num_vals; k++)
    {
        appendToBuffer(f_exp[k], &sb);
    }

    sb.idx = 0;

    writeSerializationBufferToFile(sb, "file.bin");

    SerializationBuffer read_sb;
    readFileToSerializationBuffer(&read_sb, "file.bin");

    for(size_t k = 0; k < num_vals; k++)
    {
        EXPECT_EQ(f_exp[k], deserializeFloat(&read_sb));
    }

    EXPECT_EQ(sb.num_elements, num_vals);
}

TEST_F(TestSerializationC, TestBasic)
{
    const float f_exp[5] = {0.34f, 0.62f, 2.42f, 0.67f, 0.001f};
    const size_t num_vals = sizeof(f_exp) / sizeof(float);
    SerializationBuffer sb = createSerializationBuffer(sizeof(f_exp) * 3);

    for(size_t k = 0; k < num_vals; k++)
    {
        appendToBuffer(f_exp[k], &sb);
    }

    sb.idx = 0;

    for(size_t k = 0; k < num_vals; k++)
    {
        EXPECT_EQ(f_exp[k], deserializeFloat(&sb));
    }

    EXPECT_EQ(sb.num_elements, num_vals);
}

/*TEST_F(TestSerializationC, TestBasic)
{
    const float f_exp[5] = {0.34f, 0.62f, 2.42f, 0.67f, 0.001f};
    const size_t num_vals = sizeof(f_exp) / sizeof(float);
    SerializationBuffer sb = createSerializationBuffer(sizeof(f_exp) * 3);

    for(size_t k = 0; k < num_vals; k++)
    {
        serializeFloat(f_exp[k], &sb);
    }

    sb.idx = 0;

    for(size_t k = 0; k < num_vals; k++)
    {
        // sb.idx += sizeof(ST_FLOAT);
        // incrementIndexWithType(&sb);
        EXPECT_EQ(f_exp[k], deserializeFloat(&sb));
    }

    
    // EXPECT_EQ(f_exp, f_actual);
    // deserializeFloat(&sb)
}*/

