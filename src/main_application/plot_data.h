#ifndef MAIN_APPLICATION_PLOT_DATA_H_
#define MAIN_APPLICATION_PLOT_DATA_H_

#include <string>
#include <vector>

#include "axes/legend_properties.h"
#include "communication/received_data.h"
#include "dvs/dvs.h"
#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "shader.h"

using namespace dvs;
using namespace dvs::internal;

class PlotObjectBase;

class PlotDataHandler
{
private:
    bool pending_clear_;
    ShaderCollection shader_collection_;

public:
    std::pair<Vec3d, Vec3d> getMinMaxVectors() const;
    std::vector<PlotObjectBase*> plot_datas_;
    std::vector<PlotObjectBase*> old_plot_datas_;
    PlotDataHandler(const ShaderCollection shader_collection);
    void clear();
    void softClear();
    void addData(std::unique_ptr<const ReceivedData> received_data, const TransmissionHeader& hdr);
    void render() const;
    std::vector<LegendProperties> getLegendStrings() const;
};

#endif // MAIN_APPLICATION_PLOT_DATA_H_
