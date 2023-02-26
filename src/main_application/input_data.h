#ifndef INPUT_DATA_H_
#define INPUT_DATA_H_

#include <tuple>

#include "dvs/communication_header.h"
#include "plot_objects/plot_object_base/plot_object_base.h"
#include "plot_objects/plot_objects.h"

class InputData
{
public:
    InputData() = delete;

    InputData(ReceivedData& received_data);
    InputData(ReceivedData& received_data,
              const PlotObjectAttributes& plot_object_attributes,
              const PropertiesData& properties_data);
    InputData(ReceivedData& received_data,
              std::unique_ptr<const ConvertedDataBase>& converted_data,
              const PlotObjectAttributes& plot_object_attributes,
              const PropertiesData& properties_data);

    internal::Function getFunction() const;

    std::tuple<ReceivedData, PlotObjectAttributes, PropertiesData, std::unique_ptr<const ConvertedDataBase>>
    moveAllData();

    std::tuple<ReceivedData, PlotObjectAttributes, PropertiesData> moveAllDataButConvertedData();

private:
    ReceivedData received_data_;
    internal::Function function_;
    std::unique_ptr<const ConvertedDataBase> converted_data_;
    PlotObjectAttributes plot_object_attributes_;
    PropertiesData properties_data_;
};

#endif  // INPUT_DATA_H_
