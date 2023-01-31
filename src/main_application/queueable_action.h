#ifndef QUEUEABLE_ACTION_H_
#define QUEUEABLE_ACTION_H_

#include "dvs/communication_header.h"
#include "plot_objects/plot_object_base/plot_object_base.h"
#include "plot_objects/plot_objects.h"

class QueueableAction
{
public:
    QueueableAction() = delete;

    QueueableAction(const internal::CommunicationHeader& hdr, std::unique_ptr<const ReceivedData>& received_data)
        : hdr_{hdr}, function{hdr.getFunction()}, received_data_{std::move(received_data)}, converted_data_{nullptr}
    {
    }

    QueueableAction(const internal::CommunicationHeader& hdr,
                    std::unique_ptr<const ReceivedData>& received_data,
                    std::unique_ptr<const ConvertedDataBase>& converted_data)
        : hdr_{hdr},
          function{hdr.getFunction()},
          received_data_{std::move(received_data)},
          converted_data_{std::move(converted_data)}
    {
    }

    internal::CommunicationHeader getHeader() const
    {
        return hdr_;
    }

    internal::CommunicationHeader hdr_;
    internal::Function function;
    std::unique_ptr<const ReceivedData> received_data_;
    std::unique_ptr<const ConvertedDataBase> converted_data_;
};

#endif
