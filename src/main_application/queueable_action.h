#ifndef QUEUEABLE_ACTION_H_
#define QUEUEABLE_ACTION_H_

#include <tuple>

#include "dvs/communication_header.h"
#include "plot_objects/plot_object_base/plot_object_base.h"
#include "plot_objects/plot_objects.h"

class QueueableAction
{
public:
    QueueableAction() = delete;

    QueueableAction(ReceivedData& received_data)
        : received_data_{std::move(received_data)}, function_{received_data_.getFunction()}, converted_data_{nullptr}
    {
    }

    QueueableAction(ReceivedData& received_data, std::unique_ptr<const ConvertedDataBase>& converted_data)
        : received_data_{std::move(received_data)},
          function_{received_data_.getFunction()},
          converted_data_{std::move(converted_data)}
    {
    }

    /*const internal::CommunicationHeader& getHeader() const
    {
        return received_data_->getCommunicationHeader();
    }*/

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

#endif
