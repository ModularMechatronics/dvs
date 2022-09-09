#ifndef DVS_COMMUNICATION_HEADER_H_
#define DVS_COMMUNICATION_HEADER_H_

#include <algorithm>
#include <cassert>
#include <climits>
#include <stdexcept>
#include <vector>

#include "dvs/base_types.h"
#include "dvs/enumerations.h"
#include "dvs/plot_properties.h"
#include "dvs/constants.h"
#include "dvs/utils.h"
#include "dvs/math/math.h"
#include "dvs/fillable_uint8_array.h"

namespace dvs
{
namespace internal
{

template <typename T> uint8_t toUInt8(const T v)
{
    return static_cast<uint8_t>(v);
}

template <typename T> uint32_t toUInt32(const T v)
{
    return static_cast<uint32_t>(v);
}

struct CommunicationHeaderObject
{
    CommunicationHeaderObjectType type;
    uint8_t num_bytes;
    uint8_t data[kCommunicationHeaderObjectDataSize];

    template <typename T> T as() const
    {
        T out_var;
        std::memcpy(reinterpret_cast<uint8_t*>(&out_var), data, sizeof(T));
        /*uint8_t* const ptr = reinterpret_cast<uint8_t* const>(&out_var);

        for (size_t k = 0; k < sizeof(T); k++)
        {
            ptr[k] = data[k];
        }*/

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
    else if (std::is_same<T, float_t>::value)
    {
        return DataType::FLOAT;
    }
    else if (std::is_same<T, double>::value)
    {
        return DataType::DOUBLE;
    }
    else if (std::is_same<T, double_t>::value)
    {
        return DataType::DOUBLE;
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

template <typename U> bool checkTypeValid(const CommunicationHeaderObjectType& object_type)
{
    if (object_type == CommunicationHeaderObjectType::NUM_BUFFERS_REQUIRED)
    {
        return std::is_same<U, uint8_t>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::NUM_BYTES)
    {
        return std::is_same<U, uint32_t>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::DATA_STRUCTURE)
    {
        return std::is_same<U, DataStructure>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::BYTES_PER_ELEMENT)
    {
        return std::is_same<U, uint8_t>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::DATA_TYPE)
    {
        return std::is_same<U, DataType>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::NUM_ELEMENTS)
    {
        return std::is_same<U, uint32_t>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::DIMENSION_2D)
    {
        return std::is_same<U, Dimension2D>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::HAS_PAYLOAD)
    {
        return std::is_same<U, bool>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::AZIMUTH)
    {
        return std::is_same<U, float>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::ELEVATION)
    {
        return std::is_same<U, float>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::AXIS_MIN_MAX_VEC)
    {
        return std::is_same<U, std::pair<Vec3<double>, Vec3<double>>>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::ELEMENT_NAME)
    {
        return std::is_same<U, std::string>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::FIGURE_NUM)
    {
        return std::is_same<U, uint8_t>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::FUNCTION)
    {
        return std::is_same<U, Function>::value;
    }
    else if (object_type == CommunicationHeaderObjectType::PROPERTY)
    {
        return std::is_base_of<PropertyBase, U>::value;
    }
    else
    {
        return false;
    }
}

class CommunicationHeader
{
private:
    std::vector<CommunicationHeaderObject> values;

    void extendInternal(__attribute__((unused)) std::vector<CommunicationHeaderObject>& values)
    {
        // To catch when there are no arguments to extend
    }

    template <typename U> void extendInternal(std::vector<CommunicationHeaderObject>& values, const U& obj)
    {
        static_assert(std::is_base_of<PropertyBase, U>::value || std::is_same<PropertyType, U>::value,
                      "Incorrect type!");
        assert((sizeof(U) <= kCommunicationHeaderObjectDataSize) && "Too many data bytes!");

        values.push_back(CommunicationHeaderObject());
        CommunicationHeaderObject* const ptr = &(values[values.size() - 1]); // TODO: Change to ref

        ptr->type = CommunicationHeaderObjectType::PROPERTY;

        if(std::is_same<U, PropertyType>::value)
        {
            const PropertyType* const obj_ptr = reinterpret_cast<const PropertyType* const>(&obj);
            if(*obj_ptr == PropertyType::PERSISTENT)
            {
                ptr->num_bytes = sizeof(PropertyBase);
                PropertyBase ps;
                ps.setPropertyType(PropertyType::PERSISTENT);
                fillBufferWithObjects(ptr->data, ps);
            }
        }
        else
        {
            ptr->num_bytes = sizeof(U);
            fillBufferWithObjects(ptr->data, obj);
        }
    }

    template <typename U, typename... Us>
    void extendInternal(std::vector<CommunicationHeaderObject>& values, const U& obj, const Us&... other_objs)
    {
        static_assert(std::is_base_of<PropertyBase, U>::value || std::is_same<PropertyType, U>::value,
                      "Incorrect type!");
        assert((sizeof(U) <= kCommunicationHeaderObjectDataSize) && "Too many data bytes!");

        values.push_back(CommunicationHeaderObject());
        CommunicationHeaderObject* const ptr = &(values[values.size() - 1]); // TODO: Change to ref

        ptr->type = CommunicationHeaderObjectType::PROPERTY;

        if(std::is_same<U, PropertyType>::value)
        {
            const PropertyType* const obj_ptr = reinterpret_cast<const PropertyType* const>(&obj);
            if(*obj_ptr == PropertyType::PERSISTENT)
            {
                ptr->num_bytes = sizeof(PropertyBase);
                PropertyBase ps;
                ps.setPropertyType(PropertyType::PERSISTENT);
                fillBufferWithObjects(ptr->data, ps);
            }
        }
        else
        {
            ptr->num_bytes = sizeof(U);
            fillBufferWithObjects(ptr->data, obj);
        }

        extendInternal(values, other_objs...);
    }

public:
    CommunicationHeader()
    {
        values.reserve(16);
    }

    CommunicationHeader(const uint8_t* const buffer, const bool is_big_endian)
    {
        (void)is_big_endian;
        const uint8_t num_expected_values = buffer[0];
        values.reserve(num_expected_values);

        size_t idx = 1;

        uint8_t num_values = 0;

        while (num_values < num_expected_values)
        {
            values.push_back(CommunicationHeaderObject());
            CommunicationHeaderObject* const ptr = &(values[values.size() - 1]);

            std::memcpy(&(ptr->type), &(buffer[idx]), sizeof(CommunicationHeaderObjectType));
            idx += sizeof(CommunicationHeaderObjectType);

            std::memcpy(&(ptr->num_bytes), &(buffer[idx]), sizeof(CommunicationHeaderObject::num_bytes));
            idx += sizeof(CommunicationHeaderObject::num_bytes);

            std::memcpy(ptr->data, &(buffer[idx]), ptr->num_bytes);
            idx += ptr->num_bytes;

            num_values++;
        }

        assert(static_cast<size_t>(num_values) == values.size());
    }

    template <typename U> void append(const CommunicationHeaderObjectType& object_type, const U& data)
    {
        // assert(checkTypeValid<U>(object_type) && "Invalid data type for object_type data!");

        values.push_back(CommunicationHeaderObject());
        CommunicationHeaderObject* const ptr = &(values[values.size() - 1]);

        ptr->type = object_type;
        ptr->num_bytes = sizeof(U);

        assert((ptr->num_bytes <= kCommunicationHeaderObjectDataSize) && "Too many data bytes!");

        fillBufferWithObjects(ptr->data, data);
    }

    bool hasType(const CommunicationHeaderObjectType tp) const
    {
        return std::find_if(values.begin(), values.end(), [&tp](const CommunicationHeaderObject& fo) -> bool {
                   return fo.type == tp;
               }) != values.end();
    }

    const std::vector<CommunicationHeaderObject>& getValues() const
    {
        return values;
    }

    CommunicationHeaderObject getObjectAtIdx(const size_t idx) const
    {
        return values.at(idx);
    }

    CommunicationHeaderObject get(const CommunicationHeaderObjectType tp) const
    {
        if (!hasType(tp))
        {
            throw std::runtime_error("Requested object that is not in value list!");
        }

        CommunicationHeaderObject obj;

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
            s = s + sizeof(CommunicationHeaderObjectType) + sizeof(CommunicationHeaderObject::num_bytes) + values[k].num_bytes;
        }

        return s;
    }

    void fillBufferWithData(uint8_t* const buffer) const
    {
        size_t idx = 1;
        buffer[0] = static_cast<uint8_t>(values.size());

        for (size_t k = 0; k < values.size(); k++)
        {
            std::memcpy(&(buffer[idx]), &(values[k].type), sizeof(CommunicationHeaderObjectType));
            idx += sizeof(CommunicationHeaderObjectType);

            std::memcpy(&(buffer[idx]), &(values[k].num_bytes), sizeof(CommunicationHeaderObject::num_bytes));
            idx += sizeof(CommunicationHeaderObject::num_bytes);

            std::memcpy(&(buffer[idx]), values[k].data, values[k].num_bytes);
            idx += values[k].num_bytes;
        }
    }

    void fillBufferWithData(FillableUInt8Array& fillable_array) const
    {
        fillable_array.fillWithStaticType(static_cast<uint8_t>(values.size()));

        for (size_t k = 0; k < values.size(); k++)
        {
            fillable_array.fillWithStaticType(values[k].type);

            fillable_array.fillWithStaticType(values[k].num_bytes);

            fillable_array.fillWithDataFromPointer(values[k].data, values[k].num_bytes);
        }
    }
};

}  // namespace internal

}  // namespace dvs

#endif // DVS_COMMUNICATION_HEADER_H_
