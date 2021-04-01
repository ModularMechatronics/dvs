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
};

struct Linewidth : AttributeBase
{
public:
    float data;

    Linewidth() : AttributeBase(AttributeType::LINEWIDTH) {}
    Linewidth(const float linewidth) : AttributeBase(AttributeType::LINEWIDTH), data(linewidth) {}
};

struct Alpha : AttributeBase
{
public:
    float data;

    Alpha() : AttributeBase(AttributeType::ALPHA) {}
    Alpha(const float alpha) : AttributeBase(AttributeType::ALPHA), data(alpha) {}
};

struct Name : AttributeBase
{
public:
    static constexpr size_t name_max_length = 10;
    char data[name_max_length + 1];  // +1 for null termination

    Name() : AttributeBase(AttributeType::NAME)
    {
        std::memset(data, 0, name_max_length);
    }
    Name(const char* const name) : AttributeBase(AttributeType::NAME)
    {
        const size_t input_name_length = std::strlen(name);
        assert((input_name_length <= name_max_length) && "Name can't be more than 10 characters!");
        std::memcpy(data, name, input_name_length);
        data[input_name_length] = '\0';
    }
};

inline bool operator==(const Name& n0, const Name& n1)
{
    return strcmp(n0.data, n1.data) == 0;
}

struct LineStyle : AttributeBase
{
public:
    static constexpr size_t line_style_max_length = 2;
    char data[line_style_max_length + 1];

    LineStyle() : AttributeBase(AttributeType::LINE_STYLE)
    {
        std::memset(data, 0, line_style_max_length);
    }
    LineStyle(const char* const line_style) : AttributeBase(AttributeType::LINE_STYLE)
    {
        const size_t input_name_length = std::strlen(line_style);
        assert((input_name_length <= line_style_max_length) &&
               "Line style can't be more than 2 characters!");
        std::memcpy(data, line_style, input_name_length);
        data[input_name_length] = '\0';
    }
};

struct Color : AttributeBase
{
public:
    float red, green, blue;

    // TODO: Change to chars: 'r', 'b', 'g', 'k', 'w'?
    static constexpr int RED = 1;
    static constexpr int BLUE = 2;
    static constexpr int GREEN = 3;
    static constexpr int BLACK = 4;
    static constexpr int WHITE = 5;
    static constexpr int GRAY = 6;
    static constexpr int NONE = 7;

    Color() : AttributeBase(AttributeType::COLOR), red(0.0f), green(0.0f), blue(0.0f) {}

    Color(const float red_, const float green_, const float blue_)
        : AttributeBase(AttributeType::COLOR), red(red_), green(green_), blue(blue_)
    {
        assert(((red_ >= 0.0f) && (red_ <= 1.0f)) &&
               "Red color out of bounds! Should be constrained between [0.0, 1.0]");
        assert(((green_ >= 0.0f) && (green_ <= 1.0f)) &&
               "Green color out of bounds! Should be constrained between [0.0, 1.0]");
        assert(((blue_ >= 0.0f) && (blue_ <= 1.0f)) &&
               "Blue color out of bounds! Should be constrained between [0.0, 1.0]");
    }

    Color(const int i) : AttributeBase(AttributeType::COLOR), red(0.0f), green(0.0f), blue(0.0f)
    {
        assert(((i >= 1) && (i <= 7)) && "Incorrect color input!");
        switch (i)
        {
            case RED:
                red = 1.0f;
                break;
            case GREEN:
                green = 1.0f;
                break;
            case BLUE:
                blue = 1.0f;
                break;
            case BLACK:
                red = 0.0f;
                green = 0.0f;
                blue = 0.0f;
                break;
            case WHITE:
                red = 1.0f;
                green = 1.0f;
                blue = 1.0f;
                break;
            case GRAY:
                red = 0.5f;
                green = 0.5f;
                blue = 0.5f;
                break;
            case NONE:
                red = -1.0f;
                green = -1.0f;
                blue = -1.0f;
                break;
            default:
                std::cout << "Incorrect color input!" << std::endl;
                exit(-1);
                break;
        }
    }
};

struct EdgeColor : AttributeBase
{
public:
    float red, green, blue;

    EdgeColor() : AttributeBase(AttributeType::EDGE_COLOR), red(0.0f), green(0.0f), blue(0.0f) {}

    EdgeColor(const float red_, const float green_, const float blue_)
        : AttributeBase(AttributeType::EDGE_COLOR), red(red_), green(green_), blue(blue_)
    {
    }

    EdgeColor(const int i) : AttributeBase(AttributeType::EDGE_COLOR)
    {
        const Color color(i);
        red = color.red;
        green = color.green;
        blue = color.blue;
    }
};

struct FaceColor : AttributeBase
{
public:
    float red, green, blue;

    FaceColor() : AttributeBase(AttributeType::FACE_COLOR), red(0.0f), green(0.0f), blue(0.0f) {}

    FaceColor(const float red_, const float green_, const float blue_)
        : AttributeBase(AttributeType::FACE_COLOR), red(red_), green(green_), blue(blue_)
    {
    }

    FaceColor(const int i) : AttributeBase(AttributeType::FACE_COLOR)
    {
        const Color color(i);
        red = color.red;
        green = color.green;
        blue = color.blue;
    }
};

struct ColorMap : AttributeBase
{
public:
    int data;

    static constexpr int JET = 1;
    static constexpr int RAINBOW = 2;
    static constexpr int MAGMA = 3;
    static constexpr int VIRIDIS = 4;

    ColorMap() : AttributeBase(AttributeType::COLOR_MAP), data(JET) {}
    ColorMap(const int i) : AttributeBase(AttributeType::COLOR_MAP), data(i)
    {
        assert(((i >= 1) && (i <= 4)) && "Incorrect color map input!");
    }
};

struct Persistent : AttributeBase
{
public:
    int data;

    Persistent() : AttributeBase(AttributeType::PERSISTENT) {}
    Persistent(int data_) : AttributeBase(AttributeType::PERSISTENT), data(data_) {}
};

struct PointSize : AttributeBase
{
public:
    float data;
    PointSize() : AttributeBase(AttributeType::POINT_SIZE) {}
    PointSize(const float point_size) : AttributeBase(AttributeType::POINT_SIZE), data(point_size) {}
};

}
}

#endif
