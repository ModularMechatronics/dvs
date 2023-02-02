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

    InputData(ReceivedData& received_data)
        : received_data_{std::move(received_data)}, function_{received_data_.getFunction()}, converted_data_{nullptr}
    {
    }

    InputData(ReceivedData& received_data, std::unique_ptr<const ConvertedDataBase>& converted_data)
        : received_data_{std::move(received_data)},
          function_{received_data_.getFunction()},
          converted_data_{std::move(converted_data)}
    {
    }

    internal::Function getFunction() const
    {
        return function_;
    }

    std::tuple<ReceivedData, std::unique_ptr<const ConvertedDataBase>> moveAllData()
    {
        return {std::move(received_data_), std::move(converted_data_)};
    }

    ReceivedData&& moveReceivedData()
    {
        return std::move(received_data_);
    }

private:
    ReceivedData received_data_;
    internal::Function function_;
    std::unique_ptr<const ConvertedDataBase> converted_data_;
};

#endif  // INPUT_DATA_H_
