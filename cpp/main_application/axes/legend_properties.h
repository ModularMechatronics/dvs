#ifndef LEGEND_PROPERTIES_DVS_
#define LEGEND_PROPERTIES_DVS_

#include <string>
#include <string_view>

#include "dvs/enumerations.h"
#include "opengl_low_level/data_structures.h"
#include "dvs/plot_properties.h"
#include "misc/color_map.h"

// TODO: This should be filled in plot_object_base and the individual object types

enum class LegendType
{
    LINE,
    POLYGON,
    DOT
};

struct LegendProperties
{
    std::string_view name;
    LegendType type;
    RGBTripletf color;
    RGBTripletf edge_color;
    RGBTripletf face_color;
    RGBColorMap<float>* color_map;
    bool color_map_set;
    dvs::properties::LineStyle line_style;
};

#endif
