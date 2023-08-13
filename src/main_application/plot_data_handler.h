#ifndef MAIN_APPLICATION_PLOT_DATA_HANDLER_H_
#define MAIN_APPLICATION_PLOT_DATA_HANDLER_H_

#include <string>
#include <vector>

#include "axes/legend_properties.h"
#include "color_picker.h"
#include "communication/received_data.h"
#include "dvs/dvs.h"
#include "dvs/math/math.h"
#include "input_data.h"
#include "misc/color_map.h"
#include "properties.h"

using namespace dvs;
using namespace dvs::internal;

class PlotObjectBase;
struct ShaderCollection;

class PlotDataHandler
{
private:
    bool pending_soft_clear_;
    ShaderCollection shader_collection_;
    bool isUpdatable(const Function fcn) const;
    ColorPicker color_picker_{};

public:
    std::pair<Vec3d, Vec3d> getMinMaxVectors() const;
    std::vector<PlotObjectBase*> plot_datas_;
    std::vector<PlotObjectBase*> old_plot_datas_;

    std::vector<Properties> awaiting_properties_;
    std::vector<PropertiesData> awaiting_properties_data_;
    PlotDataHandler(const ShaderCollection& shader_collection);
    ~PlotDataHandler();
    void clear();
    void softClear();
    void render();
    void addData(const CommunicationHeader& hdr,
                 const PlotObjectAttributes& plot_object_attributes,
                 const PropertiesData& properties_data,
                 ReceivedData& received_data,
                 const std::shared_ptr<const ConvertedDataBase>& converted_data);
    void setTransform(const internal::ItemId id,
                      const MatrixFixed<double, 3, 3>& rotation,
                      const Vec3<double>& translation,
                      const MatrixFixed<double, 3, 3>& scale);
    std::vector<LegendProperties> getLegendStrings() const;
    void propertiesExtension(const internal::ItemId id, const PropertiesData& properties_data);
    void propertiesExtensionMultiple(const ReceivedData& received_data);
    void deletePlotObject(const ItemId id);
};

#endif  // MAIN_APPLICATION_PLOT_DATA_HANDLER_H_
