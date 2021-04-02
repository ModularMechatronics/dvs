#ifndef FUNCTION_HEADER_H_
#define FUNCTION_HEADER_H_

#include <vector>
#include <cassert>

#include "enumerations.h"
#include "plot_attributes.h"
#include "base_types.h"
#include "utils.h"

namespace dvs
{
namespace internal
{
constexpr char max_num_bytes = SCHAR_MAX;

struct FunctionHeaderDataPair
{
    AttributeType attribute_type;
    char data[max_num_bytes];
    char num_bytes;
};

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
    else
    {
        if(std::is_signed<T>::value)
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
                    break;
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
                    break;
            }
        }
    }
}

template <typename U> bool checkTypeValid(const AttributeType& attribute_type)
{
    if (attribute_type == AttributeType::NUM_BUFFERS_REQUIRED)
    {
        return std::is_same<U, char>::value;
    }
    else if (attribute_type == AttributeType::NUM_BYTES)
    {
        return std::is_same<U, size_t>::value;
    }
    else if (attribute_type == AttributeType::DATA_STRUCTURE)
    {
        return std::is_same<U, DataStructure>::value;
    }
    else if (attribute_type == AttributeType::BYTES_PER_ELEMENT)
    {
        return std::is_same<U, char>::value;
    }
    else if (attribute_type == AttributeType::DATA_TYPE)
    {
        return std::is_same<U, DataType>::value;
    }
    else if (attribute_type == AttributeType::NUM_ELEMENTS)
    {
        return std::is_same<U, size_t>::value;
    }
    else if (attribute_type == AttributeType::DIMENSION_2D)
    {
        return std::is_same<U, Dimension2D>::value;
    }
    else if (attribute_type == AttributeType::HAS_PAYLOAD)
    {
        return std::is_same<U, bool>::value;
    }
    else if (attribute_type == AttributeType::AZIMUTH)
    {
        return std::is_same<U, float>::value;
    }
    else if (attribute_type == AttributeType::ELEVATION)
    {
        return std::is_same<U, float>::value;
    }
    else if (attribute_type == AttributeType::AXES_DIMENSIONS)
    {
        return std::is_same<U, char>::value;
    }
    else if (attribute_type == AttributeType::AXIS_MIN_MAX_VEC)
    {
        return std::is_same<U, std::pair<Bound3D, Bound3D>>::value;
    }
    else if (attribute_type == AttributeType::LINEWIDTH)
    {
        return std::is_same<U, Linewidth>::value;
    }
    else if (attribute_type == AttributeType::FACE_COLOR)
    {
        return std::is_same<U, FaceColor>::value;
    }
    else if (attribute_type == AttributeType::EDGE_COLOR)
    {
        return std::is_same<U, EdgeColor>::value;
    }
    else if (attribute_type == AttributeType::COLOR)
    {
        return std::is_same<U, Color>::value;
    }
    else if (attribute_type == AttributeType::FIGURE_NUM)
    {
        return std::is_same<U, char>::value;
    }
    else if (attribute_type == AttributeType::ALPHA)
    {
        return std::is_same<U, Alpha>::value;
    }
    else if (attribute_type == AttributeType::LINE_STYLE)
    {
        return std::is_same<U, LineStyle>::value;
    }
    else if (attribute_type == AttributeType::NAME)
    {
        return std::is_same<U, Name>::value;
    }
    else if (attribute_type == AttributeType::COLOR_MAP)
    {
        return std::is_same<U, ColorMap>::value;
    }
    else if (attribute_type == AttributeType::FUNCTION)
    {
        return std::is_same<U, Function>::value;
    }
    else if (attribute_type == AttributeType::PERSISTENT)
    {
        return std::is_same<U, Persistent>::value;
    }
    else if (attribute_type == AttributeType::POINT_SIZE)
    {
        return std::is_same<U, PointSize>::value;
    }
    else if (attribute_type == AttributeType::ATTRIBUTE)
    {
        return std::is_same<U, Attribute>::value;
    }
    else if (attribute_type == AttributeType::POS2D)
    {
        return std::is_same<U, Pos2D>::value;
    }
    else
    {
        return false;
    }
}

template <typename U> constexpr bool isCorrectType()
{
    return std::is_same<U, Linewidth>::value || std::is_same<U, Alpha>::value ||
           std::is_same<U, Name>::value || std::is_same<U, LineStyle>::value ||
           std::is_same<U, Color>::value || std::is_same<U, EdgeColor>::value ||
           std::is_same<U, FaceColor>::value || std::is_same<U, ColorMap>::value ||
           std::is_same<U, Persistent>::value || std::is_same<U, PointSize>::value;
}

class FunctionHeader
{
private:
    std::vector<FunctionHeaderDataPair> values;

    void extendInternal(__attribute__((unused)) std::vector<FunctionHeaderDataPair>& values)
    {
        //  To catch when there are no arguments to extend
    }

    template <typename U> void extendInternal(std::vector<FunctionHeaderDataPair>& values, const U& obj)
    {
        static_assert(isCorrectType<U>(), "\n\nError: Input must have correct type!\n");

        values.push_back(FunctionHeaderDataPair());
        FunctionHeaderDataPair* const ptr = &(values[values.size() - 1]);

        ptr->attribute_type = obj.getCommandType();
        ptr->num_bytes = sizeof(U);

        assert((ptr->num_bytes <= max_num_bytes) && "Too many data bytes!");

        fillBufferWithObjects(ptr->data, obj);
    }

    template <typename U, typename... Us>
    void extendInternal(std::vector<FunctionHeaderDataPair>& values, const U& obj, const Us&... other_objs)
    {
        static_assert(isCorrectType<U>(), "\n\nError: Input must have correct type!\n");

        values.push_back(FunctionHeaderDataPair());
        FunctionHeaderDataPair* const ptr = &(values[values.size() - 1]);

        ptr->attribute_type = obj.getCommandType();
        ptr->num_bytes = sizeof(U);

        assert((ptr->num_bytes <= max_num_bytes) && "Too many data bytes!");

        fillBufferWithObjects(ptr->data, obj);

        extendInternal(values, other_objs...);
    }

public:
    FunctionHeader()
    {
        values.reserve(16);
    }

    std::vector<FunctionHeaderDataPair> getVector() const
    {
        return values;
    }

    template <typename U> void append(const AttributeType& attribute_type, const U& data)
    {
        assert(checkTypeValid<U>(attribute_type) && "Invalid data type for attribute_type data!");

        values.push_back(FunctionHeaderDataPair());
        FunctionHeaderDataPair* const ptr = &(values[values.size() - 1]);

        ptr->attribute_type = attribute_type;
        ptr->num_bytes = sizeof(U);

        assert((ptr->num_bytes <= max_num_bytes) && "Too many data bytes!");

        fillBufferWithObjects(ptr->data, data);
    }

    template <typename... Us> void extend(const Us&... objects)
    {
        extendInternal(values, objects...);
    }

    size_t totalNumBytesFromBuffer() const
    {
        // 1 for first byte, that indicates how many attributes in buffer, which is
        // same as values.size()
        size_t s = 1;

        for (size_t k = 0; k < values.size(); k++)
        {
            s = s + sizeof(AttributeType) + values[k].num_bytes;
        }

        return s;
    }

    void fillBufferWithData(char* const buffer) const
    {
        assert(values.size() < max_num_bytes);
        buffer[0] = static_cast<char>(values.size());
        size_t idx = 1;

        for (size_t k = 0; k < values.size(); k++)
        {
            const char* const command_type_ptr =
                reinterpret_cast<const char* const>(&(values[k].attribute_type));

            buffer[idx] = command_type_ptr[0];
            buffer[idx + 1] = command_type_ptr[1];
            idx = idx + 2;
            for (size_t i = 0; i < static_cast<size_t>(values[k].num_bytes); i++)
            {
                buffer[idx] = values[k].data[i];
                idx++;
            }
        }
    }
};

}

}


#endif
