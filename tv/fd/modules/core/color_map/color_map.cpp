#include "arl/utilities/color_map.h"

namespace arl
{
namespace color_maps
{
const RGBColorMap<float> rainbowf(
    {
        RGBTriplet<float>(1, 1, 0),
        RGBTriplet<float>(0, 1, 0),
        RGBTriplet<float>(0, 1, 1),
        RGBTriplet<float>(0, 0, 1),
        RGBTriplet<float>(1, 0, 1),
        RGBTriplet<float>(1, 0, 0),
    },
    1000);

const RGBColorMap<float> magmaf(
    {
        RGBTriplet<float>(6.0f / 255.0f, 3.0f / 255.0f, 23.0f / 255.0f),
        RGBTriplet<float>(109.0f / 255.0f, 15.0f / 255.0f, 128.0f / 255.0f),
        RGBTriplet<float>(237.0f / 255.0f, 101.0f / 255.0f, 93.0f / 255.0f),
        RGBTriplet<float>(250.0f / 255.0f, 242.0f / 255.0f, 180.0f / 255.0f),
    },
    1000);

const RGBColorMap<float> viridisf(
    {
        RGBTriplet<float>(67.0f / 255.0f, 0.0f / 255.0f, 84.0f / 255.0f),
        RGBTriplet<float>(71.0f / 255.0f, 42.0f / 255.0f, 124.0f / 255.0f),
        RGBTriplet<float>(62.0f / 255.0f, 84.0f / 255.0f, 139.0f / 255.0f),
        RGBTriplet<float>(54.0f / 255.0f, 115.0f / 255.0f, 141.0f / 255.0f),
        RGBTriplet<float>(50.0f / 255.0f, 143.0f / 255.0f, 139.0f / 255.0f),
        RGBTriplet<float>(56.0f / 255.0f, 172.0f / 255.0f, 129.0f / 255.0f),
        RGBTriplet<float>(87.0f / 255.0f, 196.0f / 255.0f, 109.0f / 255.0f),
        RGBTriplet<float>(159.0f / 255.0f, 220.0f / 255.0f, 65.0f / 255.0f),
        RGBTriplet<float>(246.0f / 255.0f, 233.0f / 255.0f, 51.0f / 255.0f),
    },
    1000);

const RGBColorMap<float> jetf(
    {
        RGBTriplet<float>(19.0f / 255.0f, 0.0f / 255.0f, 130.05 / 255.0f),
        RGBTriplet<float>(48.0f / 255.0f, 12.0f / 255.0f, 253.05 / 255.0f),
        RGBTriplet<float>(68.0f / 255.0f, 229.0f / 255.0f, 245.05 / 255.0f),
        RGBTriplet<float>(148.0f / 255.0f, 255.0f / 255.0f, 108.05 / 255.0f),
        RGBTriplet<float>(250.0f / 255.0f, 205.0f / 255.0f, 34.05 / 255.0f),
        RGBTriplet<float>(247.0f / 255.0f, 49.0f / 255.0f, 21.05 / 255.0f),
        RGBTriplet<float>(127.0f / 255.0f, 0.0f / 255.0f, 5.05 / 255.0f),
    },
    1000);

const RGBColorMap<unsigned char> rainbow(
    {
        RGBTriplet<unsigned char>(255, 255, 0),
        RGBTriplet<unsigned char>(0, 255, 0),
        RGBTriplet<unsigned char>(0, 255, 255),
        RGBTriplet<unsigned char>(0, 0, 255),
        RGBTriplet<unsigned char>(255, 0, 255),
        RGBTriplet<unsigned char>(255, 0, 0),
    },
    1000);

const RGBColorMap<unsigned char> magma(
    {
        RGBTriplet<unsigned char>(6, 3, 23),
        RGBTriplet<unsigned char>(109, 15, 128),
        RGBTriplet<unsigned char>(237, 101, 93),
        RGBTriplet<unsigned char>(250, 242, 180),
    },
    1000);

const RGBColorMap<unsigned char> viridis(
    {
        RGBTriplet<unsigned char>(67, 0, 84),
        RGBTriplet<unsigned char>(71, 42, 124),
        RGBTriplet<unsigned char>(62, 84, 139),
        RGBTriplet<unsigned char>(54, 115, 141),
        RGBTriplet<unsigned char>(50, 143, 139),
        RGBTriplet<unsigned char>(56, 172, 129),
        RGBTriplet<unsigned char>(87, 196, 109),
        RGBTriplet<unsigned char>(159, 220, 65),
        RGBTriplet<unsigned char>(246, 233, 51),
    },
    1000);

const RGBColorMap<unsigned char> jet(
    {
        RGBTriplet<unsigned char>(19, 0, 130),
        RGBTriplet<unsigned char>(48, 12, 253),
        RGBTriplet<unsigned char>(68, 229, 245),
        RGBTriplet<unsigned char>(148, 255, 108),
        RGBTriplet<unsigned char>(250, 205, 34),
        RGBTriplet<unsigned char>(247, 49, 21),
        RGBTriplet<unsigned char>(127, 0, 5),
    },
    1000);

}  // namespace color_maps
}  // namespace arl
