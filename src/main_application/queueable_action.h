#ifndef QUEUEABLE_ACTION_H_
#define QUEUEABLE_ACTION_H_

#include "dvs/communication_header.h"
#include "plot_objects/plot_object_base/plot_object_base.h"
#include "plot_objects/plot_objects.h"

class QueueableAction
{
public:
    enum class Type
    {
        RAW_PLOT_DATA_AND_HEADER,
        RECEIVED_DATA_WITH_HEADER,
        RECEIVED_DATA_HEADER_ONLY,
    };

    QueueableAction() = delete;

    QueueableAction(const internal::CommunicationHeader& hdr, std::unique_ptr<const ReceivedData>& received_data)
        : hdr_{hdr}, received_data_{std::move(received_data)}
    {
        type_ = QueueableAction::Type::RECEIVED_DATA_WITH_HEADER;
    }

    QueueableAction(const internal::CommunicationHeader& hdr) : hdr_{hdr}
    {
        type_ = QueueableAction::Type::RECEIVED_DATA_HEADER_ONLY;
    }

    QueueableAction(ConvertedDataBase* converted_data,
                    const internal::CommunicationHeader& hdr,
                    std::unique_ptr<const ReceivedData>& received_data)
        : hdr_{hdr}, received_data_{std::move(received_data)}, converted_data_{converted_data}
    {
        type_ = QueueableAction::Type::RAW_PLOT_DATA_AND_HEADER;
    }

    QueueableAction::Type getType() const
    {
        return type_;
    }

    internal::CommunicationHeader getHeader() const
    {
        return hdr_;
    }

    ConvertedDataBase* getConvertedData() const
    {
        return converted_data_;
    }

    internal::CommunicationHeader hdr_;
    std::unique_ptr<const ReceivedData> received_data_;
    ConvertedDataBase* converted_data_;
    QueueableAction::Type type_;
};

#endif
