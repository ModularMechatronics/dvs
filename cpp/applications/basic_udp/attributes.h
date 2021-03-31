#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include <stdint.h>
#include <stdlib.h>

namespace dvs
{
namespace internal
{

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
    static constexpr int num_data_bytes_ = 24;
    uint8_t data_[num_data_bytes_];
    AttributeType type;

public:
   
    Attribute()
    {
        type = AttributeType::NO_TYPE;
        for(size_t k = 0; k < num_data_bytes_; k++)
        {
            data_[k] = 0;
        }
    };

    Attribute(const Color& color)
    {

    }

    Attribute& operator=(const Color& color)
    {

    }

};
}

#endif
