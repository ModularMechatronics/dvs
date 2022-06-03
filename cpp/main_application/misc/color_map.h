#ifndef COLOR_MAP_H_
#define COLOR_MAP_H_

#include <stdlib.h>

#include <cassert>
#include <cmath>
#include <iostream>
#include <type_traits>
#include <vector>

#include "dvs/enumerations.h"

template <typename T> struct RGBTriplet
{
    T red;
    T green;
    T blue;

    RGBTriplet() = default;
    RGBTriplet(const T red_, const T green_, const T blue_) : red(red_), green(green_), blue(blue_) {}

    RGBTriplet(const uint32_t hex_color_code)
    {
        red = static_cast<float>((hex_color_code >> 16) & 0xFF) / 255.0f;
        green = static_cast<float>((hex_color_code >> 8) & 0xFF) / 255.0f;
        blue = static_cast<float>(hex_color_code & 0xFF) / 255.0f;
    }
};

// ******************************************************
// ********************* RGBTriplet *********************
// ******************************************************

template <typename T> class RGBColorMap
{
private:
    T* red_;
    T* green_;
    T* blue_;

    size_t num_values_;
    size_t num_color_brake_points_;

public:
    RGBColorMap();
    RGBColorMap(const std::vector<RGBTriplet<T>>& color_brake_points, const size_t num_values = 100);
    RGBColorMap(const RGBColorMap<T>& color_map);

    RGBColorMap& operator=(const RGBColorMap<T>& color_map);

    ~RGBColorMap();

    void setupTables(const std::vector<RGBTriplet<T>>& color_brake_points);
    void setupTablesFloat(const std::vector<RGBTriplet<T>>& color_brake_points);

    size_t getNumValues() const;
    size_t getNumColorBrakePoints() const;
    T* getRedPtr() const;
    T* getGreenPtr() const;
    T* getBluePtr() const;

    RGBTriplet<T> getColor(const double d) const;
    RGBTriplet<T> operator()(const double d) const;
    template <typename Y, typename U> RGBTriplet<T> operator()(const Y d_in, const U max_val) const;

    template <typename Y, typename U> RGBTriplet<T> operator()(const Y d_in, const U min_val, const U max_val) const;
};

template <typename T> RGBColorMap<T>::RGBColorMap() : red_(nullptr), green_(nullptr), blue_(nullptr), num_values_(0) {}

template <typename T>
RGBColorMap<T>::RGBColorMap(const std::vector<RGBTriplet<T>>& color_brake_points, const size_t num_values)
{
    assert(num_values > 1);
    assert(color_brake_points.size() > 1);

    num_values_ = num_values;
    num_color_brake_points_ = color_brake_points.size();

    red_ = new T[num_values_];
    green_ = new T[num_values_];
    blue_ = new T[num_values_];

    if (std::is_same<T, float>::value || std::is_same<T, double>::value)
    {
        setupTablesFloat(color_brake_points);
    }
    else
    {
        setupTables(color_brake_points);
    }
}

template <typename T> RGBColorMap<T>& RGBColorMap<T>::operator=(const RGBColorMap<T>& color_map)
{
    if (this != &color_map)
    {
        num_values_ = color_map.getNumValues();
        num_color_brake_points_ = color_map.getNumColorBrakePoints();

        red_ = new T[num_values_];
        green_ = new T[num_values_];
        blue_ = new T[num_values_];

        const T* const red_ptr = color_map.getRedPtr();
        const T* const green_ptr = color_map.getGreenPtr();
        const T* const blue_ptr = color_map.getBluePtr();

        for (size_t k = 0; k < num_values_; k++)
        {
            red_[k] = red_ptr[k];
            green_[k] = green_ptr[k];
            blue_[k] = blue_ptr[k];
        }
    }

    return *this;
}

template <typename T> RGBColorMap<T>::RGBColorMap(const RGBColorMap<T>& color_map)
{
    num_values_ = color_map.getNumValues();
    num_color_brake_points_ = color_map.getNumColorBrakePoints();

    red_ = new T[num_values_];
    green_ = new T[num_values_];
    blue_ = new T[num_values_];

    const T* const red_ptr = color_map.getRedPtr();
    const T* const green_ptr = color_map.getGreenPtr();
    const T* const blue_ptr = color_map.getBluePtr();

    for (size_t k = 0; k < num_values_; k++)
    {
        red_[k] = red_ptr[k];
        green_[k] = green_ptr[k];
        blue_[k] = blue_ptr[k];
    }
}

template <typename T> RGBColorMap<T>::~RGBColorMap()
{
    delete[] red_;
    delete[] green_;
    delete[] blue_;
}

template <typename T> void RGBColorMap<T>::setupTablesFloat(const std::vector<RGBTriplet<T>>& color_brake_points)
{
    const double color_table_inc =
        static_cast<double>(num_values_ - 1) / static_cast<double>(num_color_brake_points_ - 1);

    std::vector<double> color_brake_point_vals(num_color_brake_points_);

    for (size_t k = 1; k <= num_color_brake_points_; k++)
    {
        color_brake_point_vals[k - 1] = static_cast<double>(k - 1) * color_table_inc + 1.0;
    }

    for (size_t d = 1; d <= num_values_; d++)
    {
        for (size_t k = 0; k < (num_color_brake_points_ - 1); k++)
        {
            const double dd = static_cast<double>(d);

            const bool lb_cond = dd >= color_brake_point_vals[k];
            const bool ub_cond = (k + 1) == (num_color_brake_points_ - 1) ? dd <= color_brake_point_vals[k + 1]
                                                                          : dd < color_brake_point_vals[k + 1];
            if (lb_cond && ub_cond)
            {
                const double interval_length = color_brake_point_vals[k + 1] - color_brake_point_vals[k];
                const double d_mapped = (dd - color_brake_point_vals[k]) / interval_length;

                const RGBTriplet<T> c0 = color_brake_points[k];
                const RGBTriplet<T> c1 = color_brake_points[k + 1];

                red_[d - 1] = c0.red * (1.0 - d_mapped) + c1.red * d_mapped;
                green_[d - 1] = c0.green * (1.0 - d_mapped) + c1.green * d_mapped;
                blue_[d - 1] = c0.blue * (1.0 - d_mapped) + c1.blue * d_mapped;
            }
        }
    }
}

template <typename T> void RGBColorMap<T>::setupTables(const std::vector<RGBTriplet<T>>& color_brake_points)
{
    const double color_table_inc =
        static_cast<double>(num_values_ - 1) / static_cast<double>(num_color_brake_points_ - 1);

    std::vector<double> color_brake_point_vals(num_color_brake_points_);

    for (size_t k = 1; k <= num_color_brake_points_; k++)
    {
        color_brake_point_vals[k - 1] = static_cast<double>(k - 1) * color_table_inc + 1.0;
    }

    const auto f = [](const unsigned char uc) -> double { return static_cast<double>(uc); };

    for (size_t d = 1; d <= num_values_; d++)
    {
        for (size_t k = 0; k < (num_color_brake_points_ - 1); k++)
        {
            const double dd = static_cast<double>(d);

            const bool lb_cond = dd >= color_brake_point_vals[k];
            const bool ub_cond = (k + 1) == (num_color_brake_points_ - 1) ? dd <= color_brake_point_vals[k + 1]
                                                                          : dd < color_brake_point_vals[k + 1];
            if (lb_cond && ub_cond)
            {
                const double interval_length = color_brake_point_vals[k + 1] - color_brake_point_vals[k];
                const double d_mapped = (dd - color_brake_point_vals[k]) / interval_length;

                const RGBTriplet<T> c0 = color_brake_points[k];
                const RGBTriplet<T> c1 = color_brake_points[k + 1];

                red_[d - 1] = std::round(f(c0.red) * (1.0 - d_mapped) + f(c1.red) * d_mapped);
                green_[d - 1] = std::round(f(c0.green) * (1.0 - d_mapped) + f(c1.green) * d_mapped);
                blue_[d - 1] = std::round(f(c0.blue) * (1.0 - d_mapped) + f(c1.blue) * d_mapped);
            }
        }
    }
}

template <typename T> RGBTriplet<T> RGBColorMap<T>::getColor(const double d) const
{
    if ((d < 0.0) || (d > 1.0))
    {
        std::cout << "WARNING: Tried to index outside of map range [0.0, 1.0]!" << std::endl;
    }
    const double d_clamped = (d < 0.0) || (d > 1.0) ? std::max(std::min(1.0, d), 0.0) : d;

    const size_t idx = std::round(static_cast<double>(num_values_ - 1) * d_clamped);
    return RGBTriplet<T>(red_[idx], green_[idx], blue_[idx]);
}

template <typename T> RGBTriplet<T> RGBColorMap<T>::operator()(const double d) const
{
    if ((d < 0.0) || (d > 1.0))
    {
        std::cout << "WARNING: Tried to index outside of map range [0.0, 1.0]!" << std::endl;
    }
    const double d_clamped = (d < 0.0) || (d > 1.0) ? std::max(std::min(1.0, d), 0.0) : d;

    const size_t idx = std::round(static_cast<double>(num_values_ - 1) * d_clamped);
    return RGBTriplet<T>(red_[idx], green_[idx], blue_[idx]);
}

template <typename T>
template <typename Y, typename U>
RGBTriplet<T> RGBColorMap<T>::operator()(const Y d_in, const U max_val) const
{
    const double d_mapped = static_cast<double>(d_in) / static_cast<double>(max_val);

    if ((d_mapped < 0.0) || (d_mapped > 1.0))
    {
        std::cout << "WARNING: Tried to index outside of map range [0.0, 1.0]!" << std::endl;
    }
    const double d_clamped = (d_mapped < 0.0) || (d_mapped > 1.0) ? std::max(std::min(1.0, d_mapped), 0.0) : d_mapped;

    const size_t idx = std::round(static_cast<double>(num_values_ - 1) * d_clamped);
    return RGBTriplet<T>(red_[idx], green_[idx], blue_[idx]);
}

template <typename T>
template <typename Y, typename U>
RGBTriplet<T> RGBColorMap<T>::operator()(const Y d_in, const U min_val, const U max_val) const
{
    const double d_mapped = mapValueToInterval(d_in, {min_val, max_val}, {0.0, 1.0});

    if ((d_mapped < 0.0) || (d_mapped > 1.0))
    {
        std::cout << "WARNING: Tried to index outside of map range [0.0, 1.0]!" << std::endl;
    }
    const double d_clamped = (d_mapped < 0.0) || (d_mapped > 1.0) ? std::max(std::min(1.0, d_mapped), 0.0) : d_mapped;

    const size_t idx = std::round(static_cast<double>(num_values_ - 1) * d_clamped);
    return RGBTriplet<T>(red_[idx], green_[idx], blue_[idx]);
}

template <typename T> size_t RGBColorMap<T>::getNumValues() const
{
    return num_values_;
}

template <typename T> size_t RGBColorMap<T>::getNumColorBrakePoints() const
{
    return num_color_brake_points_;
}

template <typename T> T* RGBColorMap<T>::getRedPtr() const
{
    return red_;
}

template <typename T> T* RGBColorMap<T>::getGreenPtr() const
{
    return green_;
}

template <typename T> T* RGBColorMap<T>::getBluePtr() const
{
    return blue_;
}

namespace color_maps
{
const extern RGBColorMap<unsigned char> rainbow;
const extern RGBColorMap<unsigned char> magma;
const extern RGBColorMap<unsigned char> viridis;
const extern RGBColorMap<unsigned char> jet;

extern RGBColorMap<float> rainbowf;
extern RGBColorMap<float> magmaf;
extern RGBColorMap<float> viridisf;
extern RGBColorMap<float> jetf;
}  // namespace color_maps

inline RGBColorMap<float>* getColorMapFromColorMapType(const dvs::internal::ColorMapType cmt)
{
    switch(cmt)
    {
        case dvs::internal::ColorMapType::JET:
            return &color_maps::jetf;
            break;
        case dvs::internal::ColorMapType::RAINBOW:
            return &color_maps::rainbowf;
            break;
        case dvs::internal::ColorMapType::MAGMA:
            return &color_maps::magmaf;
            break;
        case dvs::internal::ColorMapType::VIRIDIS:
            return &color_maps::viridisf;
            break;
        default:
            std::cout << "Invalid ColorMapType!" << std::endl;
            exit(0);
            return nullptr;
    }
    
}

#endif
