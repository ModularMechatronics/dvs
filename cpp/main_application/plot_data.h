#ifndef PLOT_DATA_H_
#define PLOT_DATA_H_

#include <string>
#include <vector>

#include "communication/received_data.h"
#include "dvs.h"
#include "math/math.h"
#include "opengl_low_level/data_structures.h"

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
