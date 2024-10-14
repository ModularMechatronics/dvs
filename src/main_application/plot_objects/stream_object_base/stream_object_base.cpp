#include "plot_objects/stream_object_base/stream_object_base.h"

StreamObjectBase::StreamObjectBase() {}

StreamObjectBase::StreamObjectBase(const SubscribedStreamSettings& subscribed_stream_settings,
                                   const ShaderCollection& shader_collection)
    : subscribed_stream_settings_{subscribed_stream_settings}, shader_collection_{shader_collection}
{
}

/*
struct SubscribedStreamSettings
{
    TopicId topic_id{kUnknownTopicId};
    StreamType stream_type{StreamType::UNKNOWN};
    float alpha{1.0f};
    uint8_t line_width{1U};
    uint8_t point_size{1U};
    duoplot::properties::Color color{0U, 0U, 0U};
    duoplot::properties::LineStyle line_style{duoplot::properties::LineStyle::SOLID};
    duoplot::properties::ScatterStyle scatter_style{duoplot::properties::ScatterStyle::CIRCLE};
    std::string label{};


*/