#ifndef MAIN_APPLICATION_PLOT_OBJECTS_PLOT_OBJECT_BASE_PLOT_OBJECT_BASE_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_PLOT_OBJECT_BASE_PLOT_OBJECT_BASE_H_

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <string_view>

#include "communication/received_data.h"
#include "dvs/enumerations.h"
#include "dvs/math/math.h"
#include "dvs/plot_properties.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/utils.h"
#include "properties.h"
#include "axes/legend_properties.h"
#include "shader.h"

using namespace dvs;
using namespace dvs::internal;
using namespace dvs::properties;

class PlotObjectBase
{
protected:
    std::unique_ptr<const ReceivedData> received_data_;
    size_t num_dimensions_;
    size_t num_bytes_per_element_;
    uint32_t num_elements_;
    size_t num_data_bytes_;
    uint64_t num_bytes_for_one_vec_;
    uint8_t* data_ptr_;

    GLuint vertex_buffer_, vertex_buffer_array_, color_buffer_;

    Function type_;
    DataType data_type_;

    Vec3d min_vec;
    Vec3d max_vec;

    // Properties
    Name name_;
    RGBTripletf color_;
    RGBTripletf edge_color_;
    RGBTripletf face_color_;
    ColorMapType color_map_;
    LineStyle line_style_;
    int is_dashed_;
    ScatterStyleType scatter_style_type_;
    float alpha_;
    float line_width_;
    float point_size_;
    bool is_persistent_;
    bool min_max_calculated_;
    bool visualize_has_run_;
    bool has_name_;
    bool color_map_set_;
    ShaderCollection shader_collection_;

    void assignProperties(const Properties& props);
    virtual void findMinMax() = 0;

public:
    size_t getNumDimensions() const;
    virtual ~PlotObjectBase();
    PlotObjectBase();
    PlotObjectBase(std::unique_ptr<const ReceivedData> received_data, const TransmissionHeader& hdr, const ShaderCollection shader_collection);
    virtual void render() = 0;
    std::pair<Vec3d, Vec3d> getMinMaxVectors();

    bool isPersistent() const;
    std::string getName() const;
    std::string_view getNameStringView() const;

    virtual LegendProperties getLegendProperties() const
    {
        LegendProperties lp;
        lp.name = name_.data;

        return lp;
    }

    Function getType() const
    {
        return type_;
    }

    RGBTripletf getColor() const
    {
        return color_;
    }
    
    bool hasName()
    {
        return has_name_;
    }

    virtual void modifyShader();
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_PLOT_OBJECT_BASE_PLOT_OBJECT_BASE_H_
