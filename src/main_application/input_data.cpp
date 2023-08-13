#include "input_data.h"

InputData::InputData(ReceivedData& received_data)
    : received_data_{std::move(received_data)},
      function_{received_data_.getFunction()},
      converted_data_{nullptr},
      plot_object_attributes_{},
      properties_data_{}
{
}

InputData::InputData(ReceivedData& received_data,
                     const PlotObjectAttributes& plot_object_attributes,
                     const PropertiesData& properties_data)
    : received_data_{std::move(received_data)},
      function_{received_data_.getFunction()},
      converted_data_{nullptr},
      plot_object_attributes_{plot_object_attributes},
      properties_data_{properties_data}
{
}

InputData::InputData(ReceivedData& received_data,
                     const std::shared_ptr<const ConvertedDataBase>& converted_data,
                     const PlotObjectAttributes& plot_object_attributes,
                     const PropertiesData& properties_data)
    : received_data_{std::move(received_data)},
      function_{received_data_.getFunction()},
      converted_data_{converted_data},
      plot_object_attributes_{plot_object_attributes},
      properties_data_{properties_data}
{
}

internal::Function InputData::getFunction() const
{
    return function_;
}

std::tuple<ReceivedData, PlotObjectAttributes, PropertiesData, std::shared_ptr<const ConvertedDataBase>>
InputData::moveAllData()
{
    return {std::move(received_data_), plot_object_attributes_, properties_data_, converted_data_};
}

std::tuple<ReceivedData, PlotObjectAttributes, PropertiesData> InputData::moveAllDataButConvertedData()
{
    return {std::move(received_data_), plot_object_attributes_, properties_data_};
}