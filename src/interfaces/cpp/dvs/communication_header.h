#ifndef DVS_COMMUNICATION_HEADER_H_
#define DVS_COMMUNICATION_HEADER_H_

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <stdexcept>
#include <vector>

#include "dvs/base_types.h"
#include "dvs/constants.h"
#include "dvs/enumerations.h"
#include "dvs/fillable_uint8_array.h"
#include "dvs/logging.h"
#include "dvs/math/math.h"
#include "dvs/plot_properties.h"
#include "dvs/utils.h"

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
    uint8_t size;
    uint8_t data[kCommunicationHeaderObjectDataSize];

    CommunicationHeaderObject() : type{CommunicationHeaderObjectType::UNKNOWN}, size{0U} {}
    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type) : type{input_type}, size{0U} {}

    template <typename U>
    CommunicationHeaderObject(const CommunicationHeaderObjectType input_type, const U& input_data)
        : type{input_type}, size{sizeof(U)}
    {
        static_assert(sizeof(U) <= kCommunicationHeaderObjectDataSize, "Object too big!");
        std::memcpy(data, reinterpret_cast<const uint8_t* const>(&input_data), size);
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

struct CommunicationHeaderObjectLookupTable
{
    uint8_t size;
    uint8_t data[static_cast<uint8_t>(CommunicationHeaderObjectType::UNKNOWN) + 1];

    CommunicationHeaderObjectLookupTable() : size{static_cast<uint8_t>(CommunicationHeaderObjectType::UNKNOWN) + 1}
    {
        std::memset(data, static_cast<uint8_t>(255U), static_cast<uint8_t>(CommunicationHeaderObjectType::UNKNOWN) + 1);
    }

    void appendObjectIndex(const CommunicationHeaderObjectType type, const uint8_t idx)
    {
        data[static_cast<uint8_t>(type)] = idx;
    }
};

struct PropertyLookupTable
{
    uint8_t size;
    uint8_t data[static_cast<uint8_t>(PropertyType::UNKNOWN) + 1];

    PropertyLookupTable() : size{static_cast<uint8_t>(PropertyType::UNKNOWN) + 1}
    {
        std::memset(data, static_cast<uint8_t>(255U), static_cast<uint8_t>(PropertyType::UNKNOWN) + 1);
    }

    void appendPropertyIndex(const PropertyType type, const uint8_t idx)
    {
        data[static_cast<uint8_t>(type)] = idx;
    }
};

class CommunicationHeader
{
public:
    static constexpr uint8_t kNumFlags{static_cast<uint8_t>(PropertyFlag::UNKNOWN) + 1U};

    template <int16_t N> class Array : public std::array<CommunicationHeaderObject, N>
    {
    private:
        int16_t used_size_;

    public:
        Array() : used_size_{0}
        {
            static_assert(N > 0, "N can't be zero!");
        }

        void pushBack(const CommunicationHeaderObject& obj)
        {
            if (used_size_ == N)
            {
                throw std::runtime_error("Trying to push new object out of bounds!");
            }

            CommunicationHeaderObject* const data_ptr = std::array<CommunicationHeaderObject, N>::data();
            data_ptr[used_size_] = obj;

            used_size_++;
        }

        size_t usedSize() const
        {
            return used_size_;
        }

        CommunicationHeaderObject& lastElement()
        {
            if (used_size_ == 0)
            {
                throw std::runtime_error("Array empty!");
            }

            CommunicationHeaderObject* const data_ptr = std::array<CommunicationHeaderObject, N>::data();
            return data_ptr[used_size_ - 1];
        }

        void clear()
        {
            used_size_ = 0;
        }

        bool isEmpty() const
        {
            return used_size_ > 0;
        }

        void setUsedSize(const int16_t new_size)
        {
            if (new_size > N)
            {
                throw std::runtime_error("Trying set new size to larger than N!");
            }
            used_size_ = new_size;
        }
    };

    using ObjectsArray = Array<kMaxNumObjects>;
    using PropertiesArray = Array<kMaxNumProperties>;
    using FlagsArray = std::array<uint8_t, kNumFlags>;

private:
    ObjectsArray objects_;
    PropertiesArray props_;
    FlagsArray flags_;

    CommunicationHeaderObjectLookupTable objects_lut_;
    PropertyLookupTable properties_lut_;
    size_t obj_idx_;
    size_t prop_idx_;
    Function function_;

    void extendInternal()
    {
        // To catch when there are no arguments to extend
    }

    template <typename U> void extendInternal(const U& obj)
    {
        static_assert(std::is_base_of<PropertyBase, U>::value || std::is_same<PropertyType, U>::value ||
                          std::is_same<PropertyFlag, U>::value || std::is_same<PlotSlot, U>::value,
                      "Incorrect type!");
        DVS_ASSERT(sizeof(U) <= kCommunicationHeaderObjectDataSize) << "Object too big!";

        if (std::is_same<U, PropertyFlag>::value)
        {
            const PropertyFlag f = *reinterpret_cast<const PropertyFlag* const>(&obj);

            flags_[static_cast<uint8_t>(f)] = 1;
        }
        else if (std::is_same<PlotSlot, U>::value)
        {
            const PlotSlot f = *reinterpret_cast<const PlotSlot* const>(&obj);
            append(CommunicationHeaderObjectType::SLOT, f);
        }
        else
        {
            props_.pushBack(CommunicationHeaderObject{CommunicationHeaderObjectType::PROPERTY});
            CommunicationHeaderObject& current_obj = props_.lastElement();

            current_obj.size = sizeof(U);
            fillBufferWithObjects(current_obj.data, obj);

            const PropertyBase* const tmp_obj = reinterpret_cast<const PropertyBase* const>(&obj);
            properties_lut_.appendPropertyIndex(tmp_obj->getPropertyType(), prop_idx_);
            prop_idx_++;
        }
    }

    template <typename U, typename... Us> void extendInternal(const U& obj, const Us&... other_objs)
    {
        static_assert(std::is_base_of<PropertyBase, U>::value || std::is_same<PropertyType, U>::value ||
                          std::is_same<PropertyFlag, U>::value || std::is_same<PlotSlot, U>::value,
                      "Incorrect type!");
        static_assert(sizeof(U) <= kCommunicationHeaderObjectDataSize, "Object too big!");

        if (std::is_same<U, PropertyFlag>::value)
        {
            const PropertyFlag f = *reinterpret_cast<const PropertyFlag* const>(&obj);

            flags_[static_cast<uint8_t>(f)] = 1;
        }
        else if (std::is_same<PlotSlot, U>::value)
        {
            const PlotSlot f = *reinterpret_cast<const PlotSlot* const>(&obj);
            append(CommunicationHeaderObjectType::SLOT, f);
        }
        else
        {
            props_.pushBack(CommunicationHeaderObject{CommunicationHeaderObjectType::PROPERTY});
            CommunicationHeaderObject& current_obj = props_.lastElement();

            current_obj.size = sizeof(U);
            fillBufferWithObjects(current_obj.data, obj);

            const PropertyBase* const tmp_obj = reinterpret_cast<const PropertyBase* const>(&obj);
            properties_lut_.appendPropertyIndex(tmp_obj->getPropertyType(), prop_idx_);
            prop_idx_++;
        }

        extendInternal(other_objs...);
    }

    template <typename T> CommunicationHeaderObjectType templateToObjectType() const
    {
        static_assert(std::is_same<T, internal::PlotSlot>::value, "Type not allowed for template to type deduction!");
        if (std::is_same<T, internal::PlotSlot>::value)
        {
            return CommunicationHeaderObjectType::SLOT;
        }
        DVS_ASSERT(false);  // TODO: Ugly
    }

    // TODO: Remove or move to lambda
    template <typename T> void fillObjectFromBuffer(T& obj, size_t& idx, const uint8_t* const buffer)
    {
        std::memcpy(&(obj.type), buffer + idx, sizeof(CommunicationHeaderObjectType));
        idx += sizeof(CommunicationHeaderObjectType);

        std::memcpy(&(obj.size), buffer + idx, sizeof(obj.size));
        idx += sizeof(obj.size);

        std::memcpy(obj.data, buffer + idx, obj.size);
        idx += obj.size;
    }

public:
    // TODO: Split into send and receive comm. header classes?
    CommunicationHeader() = delete;

    CommunicationHeader(const Function& fcn)
        : objects_{}, props_{}, objects_lut_{}, properties_lut_{}, obj_idx_{0U}, prop_idx_{0U}, function_{fcn}
    {
        flags_.fill(0U);
    }

    CommunicationHeader(const UInt8ArrayView received_array_view)
        : objects_{},
          props_{},
          objects_lut_{},
          properties_lut_{},
          obj_idx_{0U},
          prop_idx_{0U},
          function_{Function::UNKNOWN}
    {
        flags_.fill(0U);

        const uint8_t* const buffer = received_array_view.data() + kHeaderDataStartOffset;
        const uint8_t num_expected_objects = buffer[0];
        const uint8_t num_expected_props = buffer[1];
        objects_.setUsedSize(num_expected_objects);
        props_.setUsedSize(num_expected_props);

        size_t idx = 2;

        // Function
        std::memcpy(&(function_), buffer + idx, sizeof(function_));
        idx += sizeof(function_);

        // Header Look Up Table
        std::memcpy(&(objects_lut_.size), buffer + idx, sizeof(objects_lut_.size));
        idx += sizeof(objects_lut_.size);

        std::memcpy(objects_lut_.data, buffer + idx, objects_lut_.size);
        idx += objects_lut_.size;

        // Properties Look Up Table
        std::memcpy(&(properties_lut_.size), buffer + idx, sizeof(properties_lut_.size));
        idx += sizeof(properties_lut_.size);

        std::memcpy(properties_lut_.data, buffer + idx, properties_lut_.size);
        idx += properties_lut_.size;

        uint8_t num_objects = 0;

        while (num_objects < num_expected_objects)
        {
            CommunicationHeaderObject& obj = objects_[num_objects];

            fillObjectFromBuffer(obj, idx, buffer);

            num_objects++;
        }

        uint8_t num_props = 0;

        while (num_props < num_expected_props)
        {
            CommunicationHeaderObject& obj = props_[num_props];

            fillObjectFromBuffer(obj, idx, buffer);

            num_props++;
        }

        uint8_t flag_idx = 0;

        while (flag_idx < kNumFlags)
        {
            flags_[flag_idx] = buffer[idx];
            idx++;
            flag_idx++;
        }
    }

    // TODO: Rename to "serialize" or something like that?
    void fillBufferWithData(FillableUInt8Array& fillable_array) const
    {
        fillable_array.fillWithStaticType(static_cast<uint8_t>(objects_.usedSize()));
        fillable_array.fillWithStaticType(static_cast<uint8_t>(props_.usedSize()));

        fillable_array.fillWithStaticType(function_);

        fillable_array.fillWithStaticType(objects_lut_.size);
        fillable_array.fillWithDataFromPointer(objects_lut_.data, objects_lut_.size);

        fillable_array.fillWithStaticType(properties_lut_.size);
        fillable_array.fillWithDataFromPointer(properties_lut_.data, properties_lut_.size);

        for (size_t k = 0; k < objects_.usedSize(); k++)
        {
            fillable_array.fillWithStaticType(objects_[k].type);
            fillable_array.fillWithStaticType(objects_[k].size);
            fillable_array.fillWithDataFromPointer(objects_[k].data, objects_[k].size);
        }

        for (size_t k = 0; k < props_.usedSize(); k++)
        {
            fillable_array.fillWithStaticType(props_[k].type);
            fillable_array.fillWithStaticType(props_[k].size);
            fillable_array.fillWithDataFromPointer(props_[k].data, props_[k].size);
        }

        fillable_array.fillWithDataFromPointer(flags_.data(), flags_.size());
    }

    size_t numBytes() const
    {
        // 2 for first two bytes, that indicates how many objects and
        // props there will be in the buffer
        size_t s = 2;

        s += sizeof(function_);
        s += sizeof(CommunicationHeaderObjectLookupTable::size) + objects_lut_.size;
        s += sizeof(PropertyLookupTable::size) + properties_lut_.size;

        constexpr size_t base_size{sizeof(CommunicationHeaderObjectType) + sizeof(CommunicationHeaderObject::size)};

        for (size_t k = 0; k < objects_.usedSize(); k++)
        {
            s = s + base_size + objects_[k].size;
        }

        for (size_t k = 0; k < props_.usedSize(); k++)
        {
            s = s + base_size + props_[k].size;
        }

        s += kNumFlags;

        return s;
    }

    const PropertyLookupTable& getPropertyLookupTable() const
    {
        return properties_lut_;
    }

    template <typename T> T valueOr(const T& alternative_value) const
    {
        const CommunicationHeaderObjectType type = templateToObjectType<T>();
        if (hasObjectWithType(type))
        {
            return get(type).as<T>();
        }
        else
        {
            return alternative_value;
        }
    }

    template <typename T> T value() const
    {
        const CommunicationHeaderObjectType type = templateToObjectType<T>();
        return get(type).as<T>();
    }

    template <typename U> void append(const CommunicationHeaderObjectType& object_type, const U& data)
    {
        static_assert(sizeof(U) <= kCommunicationHeaderObjectDataSize, "Object too big!");
        objects_lut_.appendObjectIndex(object_type, obj_idx_);
        obj_idx_++;

        objects_.pushBack(CommunicationHeaderObject{object_type, data});
    }

    template <typename... Us> void extend(const Us&... other_objects)
    {
        extendInternal(other_objects...);
    }

    void extendWithHeader(const CommunicationHeader& other_header)
    {
        for (size_t k = 0; k < other_header.objects_.usedSize(); k++)
        {
            const CommunicationHeaderObject& current_obj = other_header.objects_[k];
            if (std::find_if(
                    objects_.begin(), objects_.end(), [&current_obj](const CommunicationHeaderObject& this_obj) {
                        return this_obj.type == current_obj.type;
                    }) == objects_.end())
            {
                objects_.pushBack(current_obj);
            }
        }
    }

    bool hasObjectWithType(const CommunicationHeaderObjectType tp) const
    {
        return objects_lut_.data[static_cast<uint8_t>(tp)] != 255;
    }

    CommunicationHeaderObject get(const CommunicationHeaderObjectType tp) const
    {
        DVS_ASSERT(static_cast<uint8_t>(tp) < objects_lut_.size);
        const uint8_t idx = objects_lut_.data[static_cast<uint8_t>(tp)];
        if (idx == 255)
        {
            throw std::runtime_error("Requested object that is not present in header!");
        }
        else
        {
            return objects_[idx];
        }
    }

    const ObjectsArray& getObjects() const
    {
        return objects_;
    }

    const PropertiesArray& getProperties() const
    {
        return props_;
    }

    FlagsArray getFlags() const
    {
        return flags_;
    }

    Function getFunction() const
    {
        return function_;
    }
};

}  // namespace internal

}  // namespace dvs

#endif  // DVS_COMMUNICATION_HEADER_H_
