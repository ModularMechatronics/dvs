#ifndef MAIN_APPLICATION_PLOT_OBJECTS_LINE_COLLECTION3_LINE_COLLECTION3_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_LINE_COLLECTION3_LINE_COLLECTION3_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class LineCollection3D : public PlotObjectBase
{
private:
    uint8_t* points_ptr_;

    void findMinMax() override;

public:
    LineCollection3D();
    LineCollection3D(std::unique_ptr<const ReceivedData> received_data, const TransmissionHeader& hdr, const ShaderCollection shader_collection);
    ~LineCollection3D();

    void render() override;
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_LINE_COLLECTION3_LINE_COLLECTION3_H_