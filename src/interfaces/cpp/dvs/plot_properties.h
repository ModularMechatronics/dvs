#ifndef DVS_PLOT_PROPERTIES_H_
#define DVS_PLOT_PROPERTIES_H_

#include <cstring>
#include <iostream>

#include "dvs/enumerations.h"
#include "dvs/logging.h"
#include "dvs/math/math.h"

namespace dvs
{
namespace internal
{
struct PropertyBase  // TODO: Should be class? Should all properties be classes?
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

struct ZOffset : internal::PropertyBase
{
public:
    float data;

    ZOffset() : internal::PropertyBase(internal::PropertyType::Z_OFFSET) {}
    ZOffset(const float z_offset) : internal::PropertyBase(internal::PropertyType::Z_OFFSET), data(z_offset) {}
};

struct Transform : internal::PropertyBase
{
public:
    Vec3<double> scale;
    MatrixFixed<double, 3, 3> rotation;
    Vec3<double> translation;

    Transform() : internal::PropertyBase(internal::PropertyType::TRANSFORM) {}
    Transform(const Vec3<double>& scale_, const MatrixFixed<double, 3, 3>& rotation_, const Vec3<double>& translation_)
        : internal::PropertyBase(internal::PropertyType::TRANSFORM),
          scale{scale_},
          rotation{rotation_},
          translation{translation_}
    {
    }

    Transform(const Vec3<double>& scale_, const Matrix<double>& rotation_, const Vec3<double>& translation_)
        : internal::PropertyBase(internal::PropertyType::TRANSFORM)
    {
        scale = scale_;
        translation = translation_;

        DVS_ASSERT(rotation_.numRows() == 3U) << "Expected 3 rows for rotation matrix, got " << rotation_.numRows();
        DVS_ASSERT(rotation_.numCols() == 3U) << "Expected 3 cols for rotation matrix, got " << rotation_.numCols();

        for (size_t r = 0; r < 3; r++)
        {
            for (size_t c = 0; c < 3; c++)
            {
                rotation(r, c) = rotation_(r, c);
            }
        }
    }
};

struct Name : internal::PropertyBase
{
public:
    static constexpr size_t max_length = 100;
    char data[max_length + 1];  // +1 for null termination
    size_t length;

    Name() : internal::PropertyBase(internal::PropertyType::NAME)
    {
        data[0] = '\0';
        length = 0;
    }
    Name(const char* const name) : internal::PropertyBase(internal::PropertyType::NAME)
    {
        DVS_ASSERT(name) << "Input name string is null!";
        const size_t idx = internal::safeStringLenCheck(name, max_length + 1);

        DVS_ASSERT(idx <= max_length) << "Name can't be more than 100 characters!";
        length = idx;

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
private:
    LineStyle(const internal::LineStyleType line_style_type)
        : internal::PropertyBase(internal::PropertyType::LINE_STYLE)
    {
        data = line_style_type;
    }

public:
    internal::LineStyleType data;

    LineStyle() : internal::PropertyBase(internal::PropertyType::LINE_STYLE)
    {
        data = internal::LineStyleType::DASHED;
    }

    static LineStyle Dashed()
    {
        return LineStyle(internal::LineStyleType::DASHED);
    }

    static LineStyle Dotted()
    {
        return LineStyle(internal::LineStyleType::DOTTED);
    }

    static LineStyle LongDashed()
    {
        return LineStyle(internal::LineStyleType::LONG_DASHED);
    }
};

struct ScatterStyle : internal::PropertyBase
{
    internal::ScatterStyleType data;

    ScatterStyle() : internal::PropertyBase(internal::PropertyType::SCATTER_STYLE) {}
    ScatterStyle(const internal::ScatterStyleType scatter_style_type)
        : internal::PropertyBase(internal::PropertyType::SCATTER_STYLE), data{scatter_style_type}
    {
    }

    static ScatterStyle Cross()
    {
        return {internal::ScatterStyleType::CROSS};
    }

    static ScatterStyle Circle()
    {
        return {internal::ScatterStyleType::CIRCLE};
    }

    static ScatterStyle Disc()
    {
        return {internal::ScatterStyleType::DISC};
    }

    static ScatterStyle Square()
    {
        return {internal::ScatterStyleType::SQUARE};
    }

    static ScatterStyle Plus()
    {
        return {internal::ScatterStyleType::PLUS};
    }
};

struct Color : internal::PropertyBase
{
public:
    uint8_t red, green, blue;

    static Color Red()
    {
        return Color(255, 0, 0);
    }

    static Color Green()
    {
        return Color(0, 255, 0);
    }

    static Color Blue()
    {
        return Color(0, 0, 255);
    }

    static Color Cyan()
    {
        return Color(0, 255, 255);
    }

    static Color Magenta()
    {
        return Color(255, 0, 255);
    }

    static Color Yellow()
    {
        return Color(255, 255, 0);
    }

    static Color Black()
    {
        return Color(0, 0, 0);
    }

    static Color White()
    {
        return Color(255, 255, 255);
    }

    static Color Gray()
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
    uint8_t use_color;
    uint8_t red, green, blue;

    static EdgeColor Red()
    {
        return EdgeColor(255, 0, 0);
    }

    static EdgeColor Green()
    {
        return EdgeColor(0, 255, 0);
    }

    static EdgeColor Blue()
    {
        return EdgeColor(0, 0, 255);
    }

    static EdgeColor Cyan()
    {
        return EdgeColor(0, 255, 255);
    }

    static EdgeColor Magenta()
    {
        return EdgeColor(255, 0, 255);
    }

    static EdgeColor Yellow()
    {
        return EdgeColor(255, 255, 0);
    }

    static EdgeColor Black()
    {
        return EdgeColor(0, 0, 0);
    }

    static EdgeColor White()
    {
        return EdgeColor(255, 255, 255);
    }

    static EdgeColor Gray()
    {
        return EdgeColor(127, 127, 127);
    }

    static EdgeColor None()
    {
        return EdgeColor(0U);
    }

    EdgeColor() : internal::PropertyBase(internal::PropertyType::EDGE_COLOR), use_color{1U} {}

    EdgeColor(const uint8_t red_, const uint8_t green_, const uint8_t blue_)
        : internal::PropertyBase(internal::PropertyType::EDGE_COLOR),
          use_color{1U},
          red(red_),
          green(green_),
          blue(blue_)
    {
    }

    EdgeColor(const uint8_t use_color_)
        : internal::PropertyBase(internal::PropertyType::EDGE_COLOR),
          use_color{use_color_},
          red(0U),
          green(0U),
          blue(0U)
    {
    }
};

struct FaceColor : internal::PropertyBase
{
public:
    uint8_t use_color;
    uint8_t red, green, blue;

    static FaceColor Red()
    {
        return FaceColor(255, 0, 0);
    }

    static FaceColor Green()
    {
        return FaceColor(0, 255, 0);
    }

    static FaceColor Blue()
    {
        return FaceColor(0, 0, 255);
    }

    static FaceColor Cyan()
    {
        return FaceColor(0, 255, 255);
    }

    static FaceColor Magenta()
    {
        return FaceColor(255, 0, 255);
    }

    static FaceColor Yellow()
    {
        return FaceColor(255, 255, 0);
    }

    static FaceColor Black()
    {
        return FaceColor(0, 0, 0);
    }

    static FaceColor White()
    {
        return FaceColor(255, 255, 255);
    }

    static FaceColor Gray()
    {
        return FaceColor(127, 127, 127);
    }

    static FaceColor None()
    {
        return FaceColor(0U);
    }

    FaceColor() : internal::PropertyBase(internal::PropertyType::FACE_COLOR), use_color{1U}, red(0), green(0), blue(0)
    {
    }

    FaceColor(const uint8_t red_, const uint8_t green_, const uint8_t blue_)
        : internal::PropertyBase(internal::PropertyType::FACE_COLOR),
          use_color{1U},
          red(red_),
          green(green_),
          blue(blue_)
    {
    }

    FaceColor(const uint8_t use_color_)
        : internal::PropertyBase(internal::PropertyType::FACE_COLOR),
          use_color{use_color_},
          red(0U),
          green(0U),
          blue(0U)
    {
    }
};

struct ColorMap : internal::PropertyBase
{
public:
    internal::ColorMapType data;

    static ColorMap Jet()
    {
        return ColorMap(internal::ColorMapType::JET);
    }

    static ColorMap Rainbow()
    {
        return ColorMap(internal::ColorMapType::RAINBOW);
    }

    static ColorMap Magma()
    {
        return ColorMap(internal::ColorMapType::MAGMA);
    }

    static ColorMap Viridis()
    {
        return ColorMap(internal::ColorMapType::VIRIDIS);
    }

    static ColorMap Pastel()
    {
        return ColorMap(internal::ColorMapType::PASTEL);
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

class DistanceFrom : internal::PropertyBase
{
private:
    Vec3<double> pt_;
    double min_dist_;
    double max_dist_;
    DistanceFromType dist_from_type_;

    DistanceFrom(const Vec3<double>& pt,
                 const double min_dist,
                 const double max_dist,
                 const DistanceFromType dist_from_type)
        : internal::PropertyBase(internal::PropertyType::DISTANCE_FROM),
          pt_{pt},
          min_dist_{min_dist},
          max_dist_{max_dist},
          dist_from_type_{dist_from_type}
    {
    }

public:
    DistanceFrom() : internal::PropertyBase(internal::PropertyType::DISTANCE_FROM) {}

    Vec3<double> getPoint() const
    {
        return pt_;
    }

    double getMinDist() const
    {
        return min_dist_;
    }

    double getMaxDist() const
    {
        return max_dist_;
    }

    DistanceFromType getDistFromType() const
    {
        return dist_from_type_;
    }

    static DistanceFrom x(const double x_val, const double min_dist, const double max_dist)
    {
        return DistanceFrom(Vec3<double>{x_val, 0.0, 0.0}, min_dist, max_dist, DistanceFromType::X);
    }

    static DistanceFrom y(const double y_val, const double min_dist, const double max_dist)
    {
        return DistanceFrom(Vec3<double>{0.0, y_val, 0.0}, min_dist, max_dist, DistanceFromType::Y);
    }

    static DistanceFrom z(const double z_val, const double min_dist, const double max_dist)
    {
        return DistanceFrom(Vec3<double>{0.0, 0.0, z_val}, min_dist, max_dist, DistanceFromType::Z);
    }

    static DistanceFrom xy(const PointXY<double>& p, const double min_dist, const double max_dist)
    {
        return DistanceFrom(Vec3<double>{p.x, p.y, 0.0}, min_dist, max_dist, DistanceFromType::XY);
    }

    static DistanceFrom xz(const PointXZ<double>& p, const double min_dist, const double max_dist)
    {
        return DistanceFrom(Vec3<double>{p.x, 0.0, p.z}, min_dist, max_dist, DistanceFromType::XZ);
    }

    static DistanceFrom yz(const PointYZ<double>& p, const double min_dist, const double max_dist)
    {
        return DistanceFrom(Vec3<double>{0.0, p.y, p.z}, min_dist, max_dist, DistanceFromType::YZ);
    }

    static DistanceFrom xyz(const Point3<double>& p, const double min_dist, const double max_dist)
    {
        return DistanceFrom(p, min_dist, max_dist, DistanceFromType::XYZ);
    }
};

struct BufferSize : internal::PropertyBase
{
public:
    uint16_t data;
    BufferSize() : internal::PropertyBase(internal::PropertyType::BUFFER_SIZE) {}
    BufferSize(const uint16_t buffer_size)
        : internal::PropertyBase(internal::PropertyType::BUFFER_SIZE), data(buffer_size)
    {
    }
};

constexpr internal::PropertyFlag PERSISTENT = internal::PropertyFlag::PERSISTENT;
constexpr internal::PropertyFlag INTERPOLATE_COLORMAP = internal::PropertyFlag::INTERPOLATE_COLORMAP;
constexpr internal::PropertyFlag UPDATABLE = internal::PropertyFlag::UPDATABLE;

}  // namespace properties

}  // namespace dvs

#endif  // DVS_PLOT_PROPERTIES_H_
