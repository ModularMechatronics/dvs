#ifndef PLOT_DATA_H_
#define PLOT_DATA_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "received_data.h"
#include "opengl_low_level/data_structures.h"
#include "dvs.h"

using namespace dvs;
using namespace dvs::internal;

class PlotObjectBase;

class PlotDataHandler
{
private:
public:
    std::pair<Vec3Dd, Vec3Dd> getMinMaxVectors() const;
    std::vector<PlotObjectBase*> plot_datas_;
    PlotDataHandler();
    void clear();
    void softClear();
    void addData(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    void visualize() const;
};

#endif
