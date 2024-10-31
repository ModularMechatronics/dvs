#ifndef DUOPLOT_RGBTRIPLET_H
#define DUOPLOT_RGBTRIPLET_H

template <typename T> struct RGBTriplet
{
    T red;
    T green;
    T blue;

    constexpr RGBTriplet() = default;
    constexpr RGBTriplet(const T red_, const T green_, const T blue_) : red(red_), green(green_), blue(blue_) {}

    RGBTriplet(const uint32_t hex_color_code)
    {
        red = static_cast<float>((hex_color_code >> 16) & 0xFF) / 255.0f;
        green = static_cast<float>((hex_color_code >> 8) & 0xFF) / 255.0f;
        blue = static_cast<float>(hex_color_code & 0xFF) / 255.0f;
    }

    bool operator==(const RGBTriplet& other) const
    {
        return (red == other.red) && (green == other.green) && (blue == other.blue);
    }

    bool operator!=(const RGBTriplet& other) const
    {
        return !(*this == other);
    }
};

template <typename T, typename Y> RGBTriplet<T> operator*(const RGBTriplet<T>& rgb_triplet, const Y scalar)
{
    RGBTriplet<T> new_rgb_triplet{rgb_triplet.red * static_cast<T>(scalar),
                                  rgb_triplet.green * static_cast<T>(scalar),
                                  rgb_triplet.blue * static_cast<T>(scalar)};
    if (new_rgb_triplet.red > 1.0f)
    {
        new_rgb_triplet.red = 1.0f;
    }
    else if (new_rgb_triplet.red < 0.0f)
    {
        new_rgb_triplet.red = 0.0f;
    }

    if (new_rgb_triplet.green > 1.0f)
    {
        new_rgb_triplet.green = 1.0f;
    }
    else if (new_rgb_triplet.green < 0.0f)
    {
        new_rgb_triplet.green = 0.0f;
    }

    if (new_rgb_triplet.blue > 1.0f)
    {
        new_rgb_triplet.blue = 1.0f;
    }
    else if (new_rgb_triplet.blue < 0.0f)
    {
        new_rgb_triplet.blue = 0.0f;
    }

    return new_rgb_triplet;
}

template <typename T, typename Y> RGBTriplet<T> operator*(const Y scalar, const RGBTriplet<T>& rgb_triplet)
{
    return rgb_triplet * scalar;
}

using RGBTripletf = RGBTriplet<float>;

#endif  // DUOPLOT_RGBTRIPLET_H
