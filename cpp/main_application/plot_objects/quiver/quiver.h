#ifndef QUIVER_H_
#define QUIVER_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Quiver : public PlotObjectBase
{
private:
    void findMinMax() override;

    Dimension2D dims_;

    float* points_ptr_;

public:
    Quiver();
    Quiver(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);

    void render() override;
};

#endif
