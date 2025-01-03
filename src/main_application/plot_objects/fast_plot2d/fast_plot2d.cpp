#include "main_application/plot_objects/fast_plot2d/fast_plot2d.h"

#include "outer_converter.h"

namespace
{
struct ConvertedData : ConvertedDataBase
{
    float* points_ptr;

    ConvertedData() : points_ptr{nullptr} {}

    ConvertedData(const ConvertedData& other) = delete;
    ConvertedData& operator=(const ConvertedData& other) = delete;
    ConvertedData(ConvertedData&& other) = delete;
    ConvertedData& operator=(ConvertedData&& other) = delete;

    ~ConvertedData() override
    {
        delete[] points_ptr;
    }
};

struct InputParams
{
    size_t num_elements;

    InputParams() = default;
    InputParams(const size_t num_elements_) : num_elements{num_elements_} {}
};

template <typename T>
std::shared_ptr<const ConvertedData> convertData(const uint8_t* const input_data, const InputParams& input_params);

struct Converter
{
    template <class T>
    std::shared_ptr<const ConvertedData> convert(const uint8_t* const input_data, const InputParams& input_params) const
    {
        return convertData<T>(input_data, input_params);
    }
};
}  // namespace

FastPlot2D::FastPlot2D(const CommunicationHeader& hdr,
                       ReceivedData& received_data,
                       const std::shared_ptr<const ConvertedDataBase>& converted_data,
                       const PlotObjectAttributes& plot_object_attributes,
                       const UserSuppliedProperties& user_supplied_properties,
                       const ShaderCollection& shader_collection,
                       ColorPicker& color_picker)
    : PlotObjectBase(received_data, hdr, plot_object_attributes, user_supplied_properties, shader_collection, color_picker),
      vertex_buffer_{OGLPrimitiveType::LINE_STRIP}
{
    if (function_ != Function::FAST_PLOT2)
    {
        throw std::runtime_error("Invalid function type for FastPlot2D!");
    }

    color_ = RGBTripletf(1.0f, 0.0f, 0.0f);  // TODO: Remove

    const ConvertedData* const converted_data_local = static_cast<const ConvertedData* const>(converted_data.get());

    num_added_elements_ = 0;

    if (user_supplied_properties.is_appendable)
    {
        vertex_buffer_.addExpandableBuffer<float>(user_supplied_properties.buffer_size.value_or(kDefaultBufferSize), 2);

        vertex_buffer_.updateBufferData(0U, converted_data_local->points_ptr, num_elements_, 2U, num_added_elements_);
    }
    else
    {
        vertex_buffer_.addBuffer(converted_data_local->points_ptr, num_elements_, 2);
    }

    num_added_elements_ += num_elements_;
}

void FastPlot2D::appendNewData(ReceivedData& received_data,
                               const CommunicationHeader& hdr,
                               const std::shared_ptr<const ConvertedDataBase>& converted_data,
                               const UserSuppliedProperties& user_supplied_properties)
{
    const ConvertedData* const converted_data_local = static_cast<const ConvertedData* const>(converted_data.get());

    num_elements_ = hdr.get(CommunicationHeaderObjectType::NUM_ELEMENTS).as<uint32_t>();

    if ((num_added_elements_ + num_elements_) > buffer_size_)
    {
        DUOPLOT_LOG_ERROR() << "Buffer overflow!";
        return;
    }

    vertex_buffer_.updateBufferData(0U, converted_data_local->points_ptr, num_elements_, 2U, num_added_elements_);

    num_added_elements_ += num_elements_;
}

std::shared_ptr<const ConvertedDataBase> FastPlot2D::convertRawData(const CommunicationHeader& hdr,
                                                                    const PlotObjectAttributes& attributes,
                                                                    const UserSuppliedProperties& user_supplied_properties,
                                                                    const uint8_t* const data_ptr)
{
    const InputParams input_params{attributes.num_elements};

    std::shared_ptr<const ConvertedDataBase> converted_data_base{
        applyConverter<ConvertedData>(data_ptr, attributes.data_type, Converter{}, input_params)};

    return converted_data_base;
}

void FastPlot2D::findMinMax()
{
    Vec2d min_vec_2d, max_vec_2d;
    std::tie<Vec2d, Vec2d>(min_vec_2d, max_vec_2d) =
        findMinMaxFromTwoVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);

    min_vec_.x = min_vec_2d.x;
    min_vec_.y = min_vec_2d.y;
    min_vec_.z = -1.0;

    max_vec_.x = max_vec_2d.x;
    max_vec_.y = max_vec_2d.y;
    max_vec_.z = 1.0;
}

void FastPlot2D::render()
{
    shader_collection_.basic_plot_shader.use();
    vertex_buffer_.render(num_added_elements_);
}

FastPlot2D::~FastPlot2D() {}

LegendProperties FastPlot2D::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::LINE;
    lp.color = color_;

    return lp;
}

namespace
{
template <typename T>
std::shared_ptr<const ConvertedData> convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    ConvertedData* converted_data = new ConvertedData;
    converted_data->points_ptr = new float[2 * input_params.num_elements];

    size_t idx = 0;

    const T* const x_data = reinterpret_cast<const T* const>(input_data);
    const T* const y_data = reinterpret_cast<const T* const>(input_data) + input_params.num_elements;

    for (size_t k = 0; k < input_params.num_elements; k++)
    {
        converted_data->points_ptr[idx] = x_data[k];
        converted_data->points_ptr[idx + 1] = y_data[k];

        idx += 2;
    }

    return std::shared_ptr<const ConvertedData>(converted_data);
}
}  // namespace
