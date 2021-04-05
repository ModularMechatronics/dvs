#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include <stdint.h>
#include <stdlib.h>

namespace dvs
{
/*namespace internal
{
    constexpr int num_bytes_in_attr_data = 24;
    constexpr int max_num_data_bytes = num_bytes_in_attr_data + 2;
}

enum class AttributeType : uint8_t
{
    NO_TYPE,
    COLOR,
    LINE_WIDTH,
    LINE_COLOR,
    FACE_COLOR
};

struct Color
{
    float r, g, b;
    Color() = default;
    Color(const float r_, const float g_, const float b_) : r(r_), g(g_), b(b_) {}
};

class Attribute
{
private:
    uint8_t data_[internal::num_bytes_in_attr_data];
    uint8_t num_bytes_used_;
    AttributeType type;

public:

    void fillBufferWithInternalData(uint8_t data_to_fill[internal::max_num_data_bytes])
    {
        data_to_fill[0] = static_cast<uint8_t>(type);
        data_to_fill[1] = num_bytes_used_;
        for(size_t k = 0; k < num_bytes_used_; k++)
        {
            data_to_fill[k + 2] = data_[k];
        }
    }

    Attribute()
    {
        
        std::memset(data_, 0, internal::num_bytes_in_attr_data);
        num_bytes_used_ = 0;
        type = AttributeType::NO_TYPE;
    };

    Attribute(const Color& color);
    Attribute& operator=(const Color& color);
};

inline Attribute::Attribute(const Color& color)
{
    const uint8_t* r_data = reinterpret_cast<const uint8_t*>(&color.r);
    const uint8_t* g_data = reinterpret_cast<const uint8_t*>(&color.g);
    const uint8_t* b_data = reinterpret_cast<const uint8_t*>(&color.b);

    std::memcpy(data_, r_data, sizeof(color.r));
    std::memcpy(&(data_[sizeof(color.r)]), g_data, sizeof(color.g));
    std::memcpy(&(data_[sizeof(color.r) + sizeof(color.g)]), b_data, sizeof(color.b));

    num_bytes_used_ = sizeof(color.r) + sizeof(color.g) + sizeof(color.b);

    type = AttributeType::COLOR;
}

inline Attribute& Attribute::operator=(const Color& color)
{
    const uint8_t* r_data = reinterpret_cast<const uint8_t*>(&color.r);
    const uint8_t* g_data = reinterpret_cast<const uint8_t*>(&color.g);
    const uint8_t* b_data = reinterpret_cast<const uint8_t*>(&color.b);

    std::memcpy(data_, r_data, sizeof(color.r));
    std::memcpy(&(data_[sizeof(color.r)]), g_data, sizeof(color.g));
    std::memcpy(&(data_[sizeof(color.r) + sizeof(color.g)]), b_data, sizeof(color.b));

    num_bytes_used_ = sizeof(color.r) + sizeof(color.g) + sizeof(color.b);

    type = AttributeType::COLOR;

    return *this;
}*/

}

#endif
