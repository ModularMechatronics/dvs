#ifndef PLOT_DATA_H_
#define PLOT_DATA_H_

#include <string>
#include <vector>

#include "communication/received_data.h"
#include "dvs/dvs.h"
#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"

using namespace dvs;
using namespace dvs::internal;

class PlotObjectBase;

class PlotDataHandler
{
private:
    bool pending_clear_;

public:
    std::pair<Vec3Dd, Vec3Dd> getMinMaxVectors() const;
    std::vector<PlotObjectBase*> plot_datas_;
    std::vector<PlotObjectBase*> old_plot_datas_;
    PlotDataHandler();
    void clear();
    void softClear();
    void addData(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    void render(const GLuint shader_id) const;
};

#endif
