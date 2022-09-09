#ifndef MAIN_APPLICATION_PLOT_OBJECTS_STEM_STEM_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_STEM_STEM_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Stem : public PlotObjectBase
{
private:
    float* lines_ptr_;
    float* points_ptr_;
    GLuint lines_vertex_buffer_, lines_vertex_buffer_array_, points_vertex_buffer_, points_vertex_buffer_array_;

    void findMinMax() override;

public:
    Stem();
    Stem(std::unique_ptr<const ReceivedData> received_data, const CommunicationHeader& hdr, const ShaderCollection shader_collection);
    ~Stem();

    LegendProperties getLegendProperties() const override;

    void render() override;
    void modifyShader() override;
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_STEM_STEM_H_
