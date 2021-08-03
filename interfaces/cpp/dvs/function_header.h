#ifndef FUNCTION_HEADER_H_
#define FUNCTION_HEADER_H_

#include <algorithm>
#include <cassert>
#include <climits>
#include <stdexcept>
#include <vector>

#include "dvs/base_types.h"
#include "dvs/enumerations.h"
#include "dvs/plot_properties.h"
#include "dvs/utils.h"

namespace dvs
{
namespace internal
{
constexpr uint8_t kMaxNumFunctionHeaderBytes = SCHAR_MAX;

template <typename T> uint8_t toUInt8(const T v)
{
    return static_cast<uint8_t>(v);
}

template <typename T> uint32_t toUInt32(const T v)
{
    return static_cast<uint32_t>(v);
}

struct FunctionHeaderObject
{
    FunctionHeaderObjectType type;
    uint8_t num_bytes;
    uint8_t data[kMaxNumFunctionHeaderBytes];

    template <typename T> T as() const
    {
        T out_var;
        uint8_t* ptr = reinterpret_cast<uint8_t*>(&out_var);

        for (size_t k = 0; k < sizeof(T); k++)
        {
            ptr[k] = data[k];
        }

        return out_var;
    }
};

inline uint8_t dataTypeToNumBytes(const DataType data_type)
{
    uint8_t num_bytes = 0;
    switch (data_type)
    {
        case DataType::FLOAT:
            num_bytes = sizeof(float);
            break;
        case DataType::DOUBLE:
            num_bytes = sizeof(double);
            break;
        case DataType::INT8:
            num_bytes = sizeof(int8_t);
            break;
        case DataType::INT16:
            num_bytes = sizeof(int16_t);
            break;
        case DataType::INT32:
            num_bytes = sizeof(int32_t);
            break;
        case DataType::INT64:
            num_bytes = sizeof(int64_t);
            break;
        case DataType::UINT8:
            num_bytes = sizeof(uint8_t);
            break;
        case DataType::UINT16:
            num_bytes = sizeof(uint16_t);
            break;
        case DataType::UINT32:
            num_bytes = sizeof(uint32_t);
            break;
        case DataType::UINT64:
            num_bytes = sizeof(uint64_t);
            break;
        case DataType::UNKNOWN:
            throw std::runtime_error("Got unknown data type!");
            break;
        default:
            throw std::runtime_error("Didn't find valid data type!");
            break;
    }

    return num_bytes;
}

template <typename T> DataType typeToDataTypeEnum()
{
    if (std::is_same<T, float>::value)
    {
        return DataType::FLOAT;
    }
    else if (std::is_same<T, double>::value)
    {
        return DataType::DOUBLE;
    }
    else if (std::is_same<T, int>::value)
    {
        return DataType::INT32;
    }
    else if (std::is_same<T, unsigned int>::value)
    {
        return DataType::UINT32;
    }
    else if (std::is_same<T, char>::value)
    {
        return DataType::INT8;
    }
    else if (std::is_same<T, unsigned char>::value)
    {
        return DataType::UINT8;
    }
    else
    {
        if (std::is_signed<T>::value)
        {
            switch (sizeof(T))
            {
                case 1:
                    return DataType::INT8;
                    break;
                case 2:
                    return DataType::INT16;
                    break;
                case 4:
                    return DataType::INT32;
                    break;
                case 8:
                    return DataType::INT64;
                    break;
                default:
                    return DataType::UNKNOWN;
            }
        }
        else
        {
            switch (sizeof(T))
            {
                case 1:
                    return DataType::UINT8;
                    break;
                case 2:
                    return DataType::UINT16;
                    break;
                case 4:
                    return DataType::UINT32;
                    break;
                case 8:
                    return DataType::UINT64;
                    break;
                default:
                    return DataType::UNKNOWN;
            }
        }
    }
}

template <typename U> bool checkTypeValid(const FunctionHeaderObjectType& object_type)
{
    if (object_type == FunctionHeaderObjectType::NUM_BUFFERS_REQUIRED)
    {
        return std::is_same<U, uint8_t>::value;
    }
    else if (object_type == FunctionHeaderObjectType::NUM_BYTES)
    {
        return std::is_same<U, uint32_t>::value;
    }
    else if (object_type == FunctionHeaderObjectType::DATA_STRUCTURE)
    {
        return std::is_same<U, DataStructure>::value;
    }
    else if (object_type == FunctionHeaderObjectType::BYTES_PER_ELEMENT)
    {
        return std::is_same<U, uint8_t>::value;
    }
    else if (object_type == FunctionHeaderObjectType::DATA_TYPE)
    {
        return std::is_same<U, DataType>::value;
    }
    else if (object_type == FunctionHeaderObjectType::NUM_ELEMENTS)
    {
        return std::is_same<U, uint32_t>::value;
    }
    else if (object_type == FunctionHeaderObjectType::DIMENSION_2D)
    {
        return std::is_same<U, Dimension2D>::value;
    }
    else if (object_type == FunctionHeaderObjectType::HAS_PAYLOAD)
    {
        return std::is_same<U, bool>::value;
    }
    else if (object_type == FunctionHeaderObjectType::AZIMUTH)
    {
        return std::is_same<U, float>::value;
    }
    else if (object_type == FunctionHeaderObjectType::ELEVATION)
    {
        return std::is_same<U, float>::value;
    }
    else if (object_type == FunctionHeaderObjectType::AXIS_MIN_MAX_VEC)
    {
        return std::is_same<U, std::pair<Bound3D, Bound3D>>::value;
    }
    else if (object_type == FunctionHeaderObjectType::ELEMENT_NAME)
    {
        return std::is_same<U, std::string>::value;
    }
    else if (object_type == FunctionHeaderObjectType::FIGURE_NUM)
    {
        return std::is_same<U, uint8_t>::value;
    }
    else if (object_type == FunctionHeaderObjectType::FUNCTION)
    {
        return std::is_same<U, Function>::value;
    }
    else if (object_type == FunctionHeaderObjectType::PROPERTY)
    {
        return std::is_base_of<PropertyBase, U>::value;
    }
    else
    {
        return false;
    }
}

/*template <typename U> constexpr bool isCorrectType()
{
    return std::is_same<U, LineWidth>::value || std::is_same<U, Alpha>::value ||
           std::is_same<U, Name>::value || std::is_same<U, LineStyle>::value ||
           std::is_same<U, Color>::value || std::is_same<U, EdgeColor>::value ||
           std::is_same<U, FaceColor>::value || std::is_same<U, ColorMap>::value ||
           std::is_same<U, PointSize>::value || std::is_same<U, Property>::value;
}*/

class FunctionHeader
{
private:
    std::vector<FunctionHeaderObject> values;

    void extendInternal(__attribute__((unused)) std::vector<FunctionHeaderObject>& values)
    {
        //  To catch when there are no arguments to extend
    }

    template <typename U> void extendInternal(std::vector<FunctionHeaderObject>& values, const U& obj)
    {
        static_assert(std::is_base_of<PropertyBase, U>::value || std::is_same<PropertyType, U>::value,
                      "Incorrect type!");

        values.push_back(FunctionHeaderObject());
        FunctionHeaderObject* const ptr = &(values[values.size() - 1]);

        ptr->type = FunctionHeaderObjectType::PROPERTY;
        ptr->num_bytes = sizeof(U);

        assert((ptr->num_bytes <= kMaxNumFunctionHeaderBytes) && "Too many data bytes!");

        fillBufferWithObjects(ptr->data, obj);
    }

    template <typename U, typename... Us>
    void extendInternal(std::vector<FunctionHeaderObject>& values, const U& obj, const Us&... other_objs)
    {
        static_assert(std::is_base_of<PropertyBase, U>::value || std::is_same<PropertyType, U>::value,
                      "Incorrect type!");

        values.push_back(FunctionHeaderObject());
        FunctionHeaderObject* const ptr = &(values[values.size() - 1]);

        ptr->type = FunctionHeaderObjectType::PROPERTY;
        ptr->num_bytes = sizeof(U);

        assert((ptr->num_bytes <= kMaxNumFunctionHeaderBytes) && "Too many data bytes!");

        fillBufferWithObjects(ptr->data, obj);

        extendInternal(values, other_objs...);
    }

public:
    FunctionHeader()
    {
        values.reserve(16);
    }

    FunctionHeader(const uint8_t* const buffer, const bool is_big_endian)
    {
        (void)is_big_endian;
        const uint8_t num_expected_values = buffer[0];
        values.reserve(num_expected_values);

        size_t idx = 1;

        uint8_t num_values = 0;

        while (num_values < num_expected_values)
        {
            values.push_back(FunctionHeaderObject());
            FunctionHeaderObject* const ptr = &(values[values.size() - 1]);

            std::memcpy(&(ptr->type), &(buffer[idx]), sizeof(FunctionHeaderObjectType));
            idx += sizeof(FunctionHeaderObjectType);

            std::memcpy(&(ptr->num_bytes), &(buffer[idx]), sizeof(FunctionHeaderObject::num_bytes));
            idx += sizeof(FunctionHeaderObject::num_bytes);

            std::memcpy(ptr->data, &(buffer[idx]), ptr->num_bytes);
            idx += ptr->num_bytes;

            num_values++;
        }

        assert(static_cast<size_t>(num_values) == values.size());
    }

    template <typename U> void append(const FunctionHeaderObjectType& object_type, const U& data)
    {
        // assert(checkTypeValid<U>(object_type) && "Invalid data type for object_type data!");

        values.push_back(FunctionHeaderObject());
        FunctionHeaderObject* const ptr = &(values[values.size() - 1]);

        ptr->type = object_type;
        ptr->num_bytes = sizeof(U);

        assert((ptr->num_bytes <= kMaxNumFunctionHeaderBytes) && "Too many data bytes!");

        fillBufferWithObjects(ptr->data, data);
    }

    bool hasType(const FunctionHeaderObjectType tp) const
    {
        return std::find_if(values.begin(), values.end(), [&tp](const FunctionHeaderObject& fo) -> bool {
                   return fo.type == tp;
               }) != values.end();
    }

    const std::vector<FunctionHeaderObject>& getValues() const
    {
        return values;
    }

    FunctionHeaderObject getObjectAtIdx(const size_t idx) const
    {
        return values.at(idx);
    }

    FunctionHeaderObject get(const FunctionHeaderObjectType tp) const
    {
        if (!hasType(tp))
        {
            throw std::runtime_error("Requested object that is not in value list!");
        }

        FunctionHeaderObject obj;

        for (size_t k = 0; k < values.size(); k++)
        {
            if (values[k].type == tp)
            {
                obj = values[k];
            }
        }

        return obj;
    }

    size_t getNumValues() const
    {
        return values.size();
    }

    template <typename... Us> void extend(const Us&... objects)
    {
        extendInternal(values, objects...);
    }

    size_t numBytes() const
    {
        return totalNumBytesFromBuffer();
    }

    size_t totalNumBytesFromBuffer() const
    {
        // 1 for first byte, that indicates how many attributes in buffer, which is
        // same as values.size()
        size_t s = 1;

        for (size_t k = 0; k < values.size(); k++)
        {
            s = s + sizeof(FunctionHeaderObjectType) + sizeof(FunctionHeaderObject::num_bytes) + values[k].num_bytes;
        }

        return s;
    }

    void fillBufferWithData(uint8_t* const buffer) const
    {
        size_t idx = 1;
        buffer[0] = static_cast<uint8_t>(values.size());

        for (size_t k = 0; k < values.size(); k++)
        {
            std::memcpy(&(buffer[idx]), &(values[k].type), sizeof(FunctionHeaderObjectType));
            idx += sizeof(FunctionHeaderObjectType);

            std::memcpy(&(buffer[idx]), &(values[k].num_bytes), sizeof(FunctionHeaderObject::num_bytes));
            idx += sizeof(FunctionHeaderObject::num_bytes);

            std::memcpy(&(buffer[idx]), values[k].data, values[k].num_bytes);
            idx += values[k].num_bytes;
        }
    }
};

}  // namespace internal

}  // namespace dvs

#endif
