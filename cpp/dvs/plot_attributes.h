#ifndef PLOT_ATTRIBUTES_H_
#define PLOT_ATTRIBUTES_H_

#include <cassert>
#include <cstring>
#include <iostream>

#include "enumerations.h"

namespace dvs
{

namespace internal
{


struct AttributeBase
{
protected:
    AttributeType attribute_type_;
public:
    AttributeBase() : attribute_type_(AttributeType::UNKNOWN) {}
    AttributeBase(const AttributeType attribute_type) : attribute_type_(attribute_type) {}

    AttributeType getAttributeType() const
    {
        return attribute_type_;
    };

    void setAttributeType(const AttributeType attr_tp)
    {
        attribute_type_ = attr_tp;
    }
};

inline size_t safeStringLenCheck(const char* const str, const size_t max_length)
{
    size_t idx = 0;
    while(str[idx] && (idx < max_length))
    {
        idx++;
    }

    return idx;
}

}

struct Linewidth : internal::AttributeBase
{
public:
    float data;

    Linewidth() : internal::AttributeBase(internal::AttributeType::LINEWIDTH) {}
    Linewidth(const float linewidth) : internal::AttributeBase(internal::AttributeType::LINEWIDTH), data(linewidth) {}
};

struct Alpha : internal::AttributeBase
{
public:
    float data;

    Alpha() : internal::AttributeBase(internal::AttributeType::ALPHA) {}
    Alpha(const float alpha) : internal::AttributeBase(internal::AttributeType::ALPHA), data(alpha) {}
};

struct Name : internal::AttributeBase
{
public:
    static constexpr size_t max_length = 10;
    char data[max_length + 1];  // +1 for null termination

    Name() : internal::AttributeBase(internal::AttributeType::NAME)
    {
        std::memset(data, 0, max_length + 1);
    }
    Name(const char* const name) : internal::AttributeBase(internal::AttributeType::NAME)
    {
        assert(name && "input name string is null!");
        const size_t idx = internal::safeStringLenCheck(name, max_length + 1);

        assert(idx <= max_length && "Name can't be more than 10 characters!");

        std::memcpy(data, name, idx);
        data[idx] = '\0';
    }
};

inline bool operator==(const Name& n0, const Name& n1)
{
    return strcmp(n0.data, n1.data) == 0;
}

struct LineStyle : internal::AttributeBase
{
public:
    static constexpr size_t max_length = 2;
    char data[max_length + 1];

    LineStyle() : internal::AttributeBase(internal::AttributeType::LINE_STYLE)
    {
        std::memset(data, 0, max_length + 1);
    }
    LineStyle(const char* const line_style) : internal::AttributeBase(internal::AttributeType::LINE_STYLE)
    {
        assert(line_style && "input line style string is null!");
        const size_t input_name_length = internal::safeStringLenCheck(line_style, max_length + 1);

        assert((input_name_length <= max_length) && "Line style can't be more than 2 characters!");

        std::memcpy(data, line_style, input_name_length);
        data[input_name_length] = '\0';
    }
};

struct Color;
namespace internal
{

inline Color charToColor(const char c);

}

struct Color : internal::AttributeBase
{
public:
    float red, green, blue;

    static constexpr int RED = 'r';
    static constexpr int GREEN = 'g';
    static constexpr int BLUE = 'b';
    static constexpr int CYAN = 'c';
    static constexpr int MAGENTA = 'm';
    static constexpr int YELLOW = 'y';
    static constexpr int BLACK = 'k';
    static constexpr int WHITE = 'w';
    static constexpr int GRAY = 'u';

    static Color makeRed()
    {
        return Color(1.0f, 0.0f, 0.0f);
    }

    Color() : internal::AttributeBase(internal::AttributeType::COLOR) {}

    Color(const float red_, const float green_, const float blue_)
        : internal::AttributeBase(internal::AttributeType::COLOR), red(red_), green(green_), blue(blue_)
    {
        assert(((red_ >= 0.0f) && (red_ <= 1.0f)) &&
               "Red color out of bounds! Should be constrained between [0.0, 1.0]");
        assert(((green_ >= 0.0f) && (green_ <= 1.0f)) &&
               "Green color out of bounds! Should be constrained between [0.0, 1.0]");
        assert(((blue_ >= 0.0f) && (blue_ <= 1.0f)) &&
               "Blue color out of bounds! Should be constrained between [0.0, 1.0]");
    }

    Color(const char c) : internal::AttributeBase(internal::AttributeType::COLOR), red(0.0f), green(0.0f), blue(0.0f)
    {
        *this = internal::charToColor(c);
    }
};

namespace internal
{

inline Color charToColor(const char c)
{
    assert((c == 'r') || 
           (c == 'g') || 
           (c == 'b') || 
           (c == 'c') || 
           (c == 'm') || 
           (c == 'y') || 
           (c == 'k') || 
           (c == 'w') || 
           (c == 'u') && "Invalid color input!");

    Color oc;
    oc.red = 0.0f;
    oc.green = 0.0f;
    oc.blue = 0.0f;

    switch (c)
    {
        case Color::RED:
            oc.red = 1.0f;
            break;
        case Color::GREEN:
            oc.green = 1.0f;
            break;
        case Color::BLUE:
            oc.blue = 1.0f;
            break;
        case Color::CYAN:
            oc.green = 1.0f;
            oc.blue = 1.0f;
            break;
        case Color::MAGENTA:
            oc.red = 1.0f;
            oc.blue = 1.0f;
            break;
        case Color::YELLOW:
            oc.red = 1.0f;
            oc.green = 1.0f;
            break;
        case Color::BLACK:
            oc.red = 0.0f;
            oc.green = 0.0f;
            oc.blue = 0.0f;
            break;
        case Color::WHITE:
            oc.red = 1.0f;
            oc.green = 1.0f;
            oc.blue = 1.0f;
            break;
        case Color::GRAY:
            oc.red = 0.5f;
            oc.green = 0.5f;
            oc.blue = 0.5f;
            break;
        default:
            oc.red = 0.0f;
            oc.green = 0.0f;
            oc.blue = 0.0f;
            break;
    }

    return oc;
}

}

struct EdgeColor : internal::AttributeBase
{
public:
    float red, green, blue;

    EdgeColor() : internal::AttributeBase(internal::AttributeType::EDGE_COLOR) {}

    EdgeColor(const float red_, const float green_, const float blue_)
        : internal::AttributeBase(internal::AttributeType::EDGE_COLOR), red(red_), green(green_), blue(blue_)
    {
    }

    EdgeColor(const char c) : internal::AttributeBase(internal::AttributeType::EDGE_COLOR)
    {
        const Color color(c);
        red = color.red;
        green = color.green;
        blue = color.blue;
    }

    EdgeColor(const Color& color)
    {
        attribute_type_ = internal::AttributeType::EDGE_COLOR;
        red = color.red;
        green = color.green;
        blue = color.blue;
    }

    EdgeColor& operator=(const Color& color)
    {
        this->attribute_type_ = internal::AttributeType::EDGE_COLOR;
        this->red = color.red;
        this->green = color.green;
        this->blue = color.blue;

        return *this;
    }
};

struct FaceColor : internal::AttributeBase
{
public:
    float red, green, blue;

    FaceColor() : internal::AttributeBase(internal::AttributeType::FACE_COLOR), red(0.0f), green(0.0f), blue(0.0f) {}

    FaceColor(const float red_, const float green_, const float blue_)
        : internal::AttributeBase(internal::AttributeType::FACE_COLOR), red(red_), green(green_), blue(blue_)
    {
    }

    FaceColor(const char c) : internal::AttributeBase(internal::AttributeType::FACE_COLOR)
    {
        const Color color(c);
        red = color.red;
        green = color.green;
        blue = color.blue;
    }

    FaceColor(const Color& color)
    {
        attribute_type_ = internal::AttributeType::FACE_COLOR;
        red = color.red;
        green = color.green;
        blue = color.blue;
    }

    FaceColor& operator=(const Color& color)
    {
        this->attribute_type_ = internal::AttributeType::FACE_COLOR;
        this->red = color.red;
        this->green = color.green;
        this->blue = color.blue;

        return *this;
    }
};

struct ColorMap : internal::AttributeBase
{
public:
    int data;

    static constexpr int JET = 1;
    static constexpr int RAINBOW = 2;
    static constexpr int MAGMA = 3;
    static constexpr int VIRIDIS = 4;

    ColorMap() : internal::AttributeBase(internal::AttributeType::COLOR_MAP), data(JET) {}
    ColorMap(const int i) : internal::AttributeBase(internal::AttributeType::COLOR_MAP), data(i)
    {
        assert(((i >= 1) && (i <= 4)) && "Incorrect color map input! A value in the interval [1, 4] is expected.");
    }
};

struct PointSize : internal::AttributeBase
{
public:
    float data;
    PointSize() : internal::AttributeBase(internal::AttributeType::POINT_SIZE) {}
    PointSize(const float point_size) : internal::AttributeBase(internal::AttributeType::POINT_SIZE), data(point_size) {}
};


}

#endif
