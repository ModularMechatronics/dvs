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

using RGBTripletf = RGBTriplet<float>;

#endif  // DUOPLOT_RGBTRIPLET_H
