#ifndef MAIN_APPLICATION_PLOT_OBJECTS_SCROLLING_PLOT_2D_SCROLLING_PLOT_2D_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_SCROLLING_PLOT_2D_SCROLLING_PLOT_2D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class ScrollingPlot2D : public PlotObjectBase
{
private:
    float* points_ptr_;
    float* dt_vec_;

    void findMinMax() override;
    static constexpr size_t kDefaultBufferSize = 500U;

    size_t buffer_size_;
    size_t num_elements_to_draw_;

public:
    ScrollingPlot2D();
    ScrollingPlot2D(std::unique_ptr<const ReceivedData> received_data,
                    const CommunicationHeader& hdr,
                    const ShaderCollection shader_collection);
    ~ScrollingPlot2D();

    LegendProperties getLegendProperties() const override;
    void updateWithNewData(std::unique_ptr<const ReceivedData> received_data, const CommunicationHeader& hdr) override;

    void render() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_SCROLLING_PLOT_2D_SCROLLING_PLOT_2D_H_
