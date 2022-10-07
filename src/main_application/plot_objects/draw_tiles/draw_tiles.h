#ifndef MAIN_APPLICATION_PLOT_OBJECTS_DRAW_TILES_DRAW_TILES_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_DRAW_TILES_DRAW_TILES_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "dvs/utils.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class DrawTiles : public PlotObjectBase
{
private:
    float* points_ptr_;
    Dimension2D dims_;
    GLuint buffer_handle_;
    Vec2d tile_size_;

    void findMinMax() override;

public:
    DrawTiles();
    DrawTiles(std::unique_ptr<const ReceivedData> received_data,
              const CommunicationHeader& hdr,
              const ShaderCollection shader_collection);
    ~DrawTiles();

    bool affectsColormapMinMax() const override;
    void render() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_DRAW_TILES_DRAW_TILES_H_
