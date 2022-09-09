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
    CommunicationHeaderObject() = default;

    template <typename U>
    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type, const U& input_data) :
        type{input_type}, num_bytes{sizeof(U)}
    {
        static_assert(sizeof(U) <= kCommunicationHeaderObjectDataSize, "Object too big!");
        std::memcpy(data, reinterpret_cast<const uint8_t* const>(&input_data), num_bytes);
    }

    template <typename T> T as() const
    {
        T out_val;
        std::memcpy(reinterpret_cast<uint8_t*>(&out_val), data, sizeof(T));

        return out_val;
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
            throw std::runtime_error("Invalid DataType was specified!");
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

class CommunicationHeader
{
private:
    std::vector<CommunicationHeaderObject> objects;

    void extendInternal(__attribute__((unused)) std::vector<CommunicationHeaderObject>& objects)
    {
        // To catch when there are no arguments to extend
    }

    template <typename U> void extendInternal(std::vector<CommunicationHeaderObject>& objects, const U& obj)
    {
        static_assert(std::is_base_of<PropertyBase, U>::value || std::is_same<PropertyType, U>::value,
                      "Incorrect type!");
        assert((sizeof(U) <= kCommunicationHeaderObjectDataSize) && "Too many data bytes!");

        objects.push_back(CommunicationHeaderObject());
        CommunicationHeaderObject* const ptr = &(objects[objects.size() - 1]); // TODO: Change to ref

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
    void extendInternal(std::vector<CommunicationHeaderObject>& objects, const U& obj, const Us&... other_objs)
    {
        static_assert(std::is_base_of<PropertyBase, U>::value || std::is_same<PropertyType, U>::value,
                      "Incorrect type!");
        static_assert(sizeof(U) <= kCommunicationHeaderObjectDataSize, "Object too big!");

        objects.push_back(CommunicationHeaderObject());
        CommunicationHeaderObject* const ptr = &(objects[objects.size() - 1]); // TODO: Change to ref

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

        extendInternal(objects, other_objs...);
    }

public:
    CommunicationHeader()
    {
        objects.reserve(16);
    }

    CommunicationHeader(const UInt8ArrayView received_array_view)
    {
        const uint8_t* const buffer = received_array_view.data() + kHeaderDataStartOffset;
        const uint8_t num_expected_objects = buffer[0];
        objects.reserve(num_expected_objects);

        size_t idx = 1;

        uint8_t num_objects = 0;

        while (num_objects < num_expected_objects)
        {
            objects.push_back(CommunicationHeaderObject());
            CommunicationHeaderObject& obj = objects.back();

            std::memcpy(&(obj.type), buffer + idx, sizeof(CommunicationHeaderObjectType));
            idx += sizeof(CommunicationHeaderObjectType);

            std::memcpy(&(obj.num_bytes), buffer + idx, sizeof(CommunicationHeaderObject::num_bytes));
            idx += sizeof(CommunicationHeaderObject::num_bytes);

            std::memcpy(obj.data, buffer + idx, obj.num_bytes);
            idx += obj.num_bytes;

            num_objects++;
        }

        assert(static_cast<size_t>(num_objects) == objects.size());
    }

    template <typename U> void append(const CommunicationHeaderObjectType& object_type, const U& data)
    {
        static_assert(sizeof(U) <= kCommunicationHeaderObjectDataSize, "Object too big!");

        objects.emplace_back(object_type, data);
    }

    bool hasObjectWithType(const CommunicationHeaderObjectType tp) const
    {
        return std::find_if(objects.begin(), objects.end(), [&tp](const CommunicationHeaderObject& obj) -> bool {
                   return obj.type == tp;
               }) != objects.end();
    }

    const std::vector<CommunicationHeaderObject>& getObjects() const
    {
        return objects;
    }

    CommunicationHeaderObject getObjectAtIdx(const size_t idx) const
    {
        return objects.at(idx);
    }

    CommunicationHeaderObject get(const CommunicationHeaderObjectType tp) const
    {
        if (!hasObjectWithType(tp))
        {
            throw std::runtime_error("Requested object that is not present in header!");
        }

        const auto obj = std::find_if(
            objects.begin(), objects.end(), [&tp](const CommunicationHeaderObject& obj) -> bool {
                   return obj.type == tp;
               });

        return *obj;
    }

    size_t getNumObjects() const
    {
        return objects.size();
    }

    template <typename... Us> void extend(const Us&... other_objects)
    {
        extendInternal(objects, other_objects...);
    }

    size_t numBytes() const
    {
        // 1 for first byte, that indicates how many attributes in buffer, which is
        // same as objects.size()
        size_t s = 1;

        for (size_t k = 0; k < objects.size(); k++)
        {
            s = s + sizeof(CommunicationHeaderObjectType) + sizeof(CommunicationHeaderObject::num_bytes) + objects[k].num_bytes;
        }

        return s;
    }

    void fillBufferWithData(FillableUInt8Array& fillable_array) const
    {
        fillable_array.fillWithStaticType(static_cast<uint8_t>(objects.size()));

        for (size_t k = 0; k < objects.size(); k++)
        {
            fillable_array.fillWithStaticType(objects[k].type);

            fillable_array.fillWithStaticType(objects[k].num_bytes);

            fillable_array.fillWithDataFromPointer(objects[k].data, objects[k].num_bytes);
        }
    }
};

}  // namespace internal

}  // namespace dvs

#endif // DVS_COMMUNICATION_HEADER_H_
