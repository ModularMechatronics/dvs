#ifndef DRAW_TILES_H_
#define DRAW_TILES_H_

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
    Vec2Dd tile_size_;

    void findMinMax() override;

public:
    DrawTiles();
    DrawTiles(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~DrawTiles();

    void render() override;
};

#endif
