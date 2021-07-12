#ifndef PLOT_ATTRIBUTES_H_
#define PLOT_ATTRIBUTES_H_

#include <cassert>
#include <cstring>
#include <iostream>

#include "dvs/enumerations.h"

namespace dvs
{
namespace internal
{
struct PropertyBase
{
protected:
    PropertyType property_type_;

public:
    PropertyBase() : property_type_(PropertyType::UNKNOWN) {}
    PropertyBase(const PropertyType property_type) : property_type_(property_type) {}

    PropertyType getPropertyType() const
    {
        return property_type_;
    };

    void setPropertyType(const PropertyType attr_tp)
    {
        property_type_ = attr_tp;
    }

    virtual ~PropertyBase() {}
};

inline size_t safeStringLenCheck(const char* const str, const size_t max_length)
{
    size_t idx = 0;
    while (str[idx] && (idx < max_length))
    {
        idx++;
    }

    return idx;
}

}  // namespace internal

namespace properties
{
struct LineWidth : internal::PropertyBase
{
public:
    uint8_t data;

    LineWidth() : internal::PropertyBase(internal::PropertyType::LINE_WIDTH) {}
    LineWidth(const uint8_t line_width) : internal::PropertyBase(internal::PropertyType::LINE_WIDTH), data(line_width)
    {
    }
};

struct Alpha : internal::PropertyBase
{
public:
    uint8_t data;

    Alpha() : internal::PropertyBase(internal::PropertyType::ALPHA) {}
    Alpha(const uint8_t alpha) : internal::PropertyBase(internal::PropertyType::ALPHA), data(alpha) {}
};

struct Name : internal::PropertyBase
{
public:
    static constexpr size_t max_length = 20;
    char data[max_length + 1];  // +1 for null termination

    Name() : internal::PropertyBase(internal::PropertyType::NAME)
    {
        std::memset(data, 0, max_length + 1);
    }
    Name(const char* const name) : internal::PropertyBase(internal::PropertyType::NAME)
    {
        assert(name && "input name string is null!");
        const size_t idx = internal::safeStringLenCheck(name, max_length + 1);

        // TODO: The assertion says it can't be more than, but the condition is more than or equal to
        assert(idx <= max_length && "Name can't be more than 20 characters!");

        std::memcpy(data, name, idx);
        data[idx] = '\0';
    }
};

inline bool operator==(const Name& n0, const Name& n1)
{
    return strcmp(n0.data, n1.data) == 0;
}

struct LineStyle : internal::PropertyBase
{
public:
    static constexpr size_t max_length = 2;
    char data[max_length + 1];

    LineStyle() : internal::PropertyBase(internal::PropertyType::LINE_STYLE)
    {
        std::memset(data, 0, max_length + 1);
    }
    LineStyle(const char* const line_style) : internal::PropertyBase(internal::PropertyType::LINE_STYLE)
    {
        assert(line_style && "input line style string is null!");
        const size_t input_name_length = internal::safeStringLenCheck(line_style, max_length + 1);

        assert((input_name_length <= max_length) && "Line style can't be more than 2 characters!");

        std::memcpy(data, line_style, input_name_length);
        data[input_name_length] = '\0';
    }
};

struct Color : internal::PropertyBase
{
public:
    uint8_t red, green, blue;

    static Color RED()
    {
        return Color(255, 0, 0);
    }

    static Color GREEN()
    {
        return Color(0, 255, 0);
    }

    static Color BLUE()
    {
        return Color(0, 0, 255);
    }

    static Color CYAN()
    {
        return Color(0, 255, 255);
    }

    static Color MAGENTA()
    {
        return Color(255, 0, 255);
    }

    static Color YELLOW()
    {
        return Color(255, 255, 0);
    }

    static Color BLACK()
    {
        return Color(0, 0, 0);
    }

    static Color WHITE()
    {
        return Color(255, 255, 255);
    }

    static Color GRAY()
    {
        return Color(127, 127, 127);
    }

    Color() : internal::PropertyBase(internal::PropertyType::COLOR) {}

    Color(const uint8_t red_, const uint8_t green_, const uint8_t blue_)
        : internal::PropertyBase(internal::PropertyType::COLOR), red(red_), green(green_), blue(blue_)
    {
    }
};

struct EdgeColor : internal::PropertyBase
{
public:
    uint8_t red, green, blue;

    static EdgeColor RED()
    {
        return EdgeColor(255, 0, 0);
    }

    static EdgeColor GREEN()
    {
        return EdgeColor(0, 255, 0);
    }

    static EdgeColor BLUE()
    {
        return EdgeColor(0, 0, 255);
    }

    static EdgeColor CYAN()
    {
        return EdgeColor(0, 255, 255);
    }

    static EdgeColor MAGENTA()
    {
        return EdgeColor(255, 0, 255);
    }

    static EdgeColor YELLOW()
    {
        return EdgeColor(255, 255, 0);
    }

    static EdgeColor BLACK()
    {
        return EdgeColor(0, 0, 0);
    }

    static EdgeColor WHITE()
    {
        return EdgeColor(255, 255, 255);
    }

    static EdgeColor GRAY()
    {
        return EdgeColor(127, 127, 127);
    }

    EdgeColor() : internal::PropertyBase(internal::PropertyType::EDGE_COLOR) {}

    EdgeColor(const uint8_t red_, const uint8_t green_, const uint8_t blue_)
        : internal::PropertyBase(internal::PropertyType::EDGE_COLOR), red(red_), green(green_), blue(blue_)
    {
    }
};

struct FaceColor : internal::PropertyBase
{
public:
    uint8_t red, green, blue;

    static FaceColor RED()
    {
        return FaceColor(255, 0, 0);
    }

    static FaceColor GREEN()
    {
        return FaceColor(0, 255, 0);
    }

    static FaceColor BLUE()
    {
        return FaceColor(0, 0, 255);
    }

    static FaceColor CYAN()
    {
        return FaceColor(0, 255, 255);
    }

    static FaceColor MAGENTA()
    {
        return FaceColor(255, 0, 255);
    }

    static FaceColor YELLOW()
    {
        return FaceColor(255, 255, 0);
    }

    static FaceColor BLACK()
    {
        return FaceColor(0, 0, 0);
    }

    static FaceColor WHITE()
    {
        return FaceColor(255, 255, 255);
    }

    static FaceColor GRAY()
    {
        return FaceColor(127, 127, 127);
    }

    FaceColor() : internal::PropertyBase(internal::PropertyType::FACE_COLOR), red(0), green(0), blue(0) {}

    FaceColor(const uint8_t red_, const uint8_t green_, const uint8_t blue_)
        : internal::PropertyBase(internal::PropertyType::FACE_COLOR), red(red_), green(green_), blue(blue_)
    {
    }
};

struct ColorMap : internal::PropertyBase
{
public:
    internal::ColorMapType data;

    static ColorMap JET()
    {
        return ColorMap(internal::ColorMapType::JET);
    }

    static ColorMap RAINBOW()
    {
        return ColorMap(internal::ColorMapType::RAINBOW);
    }

    static ColorMap MAGMA()
    {
        return ColorMap(internal::ColorMapType::MAGMA);
    }

    static ColorMap VIRIDIS()
    {
        return ColorMap(internal::ColorMapType::VIRIDIS);
    }

    ColorMap() : internal::PropertyBase(internal::PropertyType::COLOR_MAP), data(internal::ColorMapType::JET) {}
    ColorMap(const internal::ColorMapType ct) : internal::PropertyBase(internal::PropertyType::COLOR_MAP), data(ct) {}
};

struct PointSize : internal::PropertyBase
{
public:
    uint8_t data;
    PointSize() : internal::PropertyBase(internal::PropertyType::POINT_SIZE) {}
    PointSize(const uint8_t point_size) : internal::PropertyBase(internal::PropertyType::POINT_SIZE), data(point_size)
    {
    }
};

}  // namespace properties

}  // namespace dvs

#endif
