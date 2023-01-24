#include "main_application/plot_objects/fast_plot2d/fast_plot2d.h"

#include "outer_converter.h"

namespace
{
struct OutputData
{
    float* points_ptr;
};

struct InputParams
{
    size_t num_elements;

    InputParams() = default;
    InputParams(const size_t num_elements_) : num_elements{num_elements_} {}
};

template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params);

struct Converter
{
    template <class T> OutputData convert(const uint8_t* const input_data, const InputParams& input_params) const
    {
        return convertData<T>(input_data, input_params);
    }
};
}  // namespace

FastPlot2D::FastPlot2D(std::unique_ptr<const ReceivedData> received_data,
                       const CommunicationHeader& hdr,
                       const Properties& props,
                       const ShaderCollection shader_collection, ColorPicker& color_picker)
    : PlotObjectBase(std::move(received_data), hdr, props, shader_collection, color_picker),
      vertex_buffer_{OGLPrimitiveType::LINE_STRIP}
{
    if (type_ != Function::FAST_PLOT2)
    {
        throw std::runtime_error("Invalid function type for FastPlot2D!");
    }

    const InputParams input_params{num_elements_};
    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    vertex_buffer_.addBuffer(output_data.points_ptr, num_elements_, 2);

    delete[] output_data.points_ptr;
}

void FastPlot2D::findMinMax()
{
    Vec2d min_vec_2d, max_vec_2d;
    std::tie<Vec2d, Vec2d>(min_vec_2d, max_vec_2d) =
        findMinMaxFromTwoVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);

    min_vec.x = min_vec_2d.x;
    min_vec.y = min_vec_2d.y;
    min_vec.z = -1.0;

    max_vec.x = max_vec_2d.x;
    max_vec.y = max_vec_2d.y;
    max_vec.z = 1.0;
}

void FastPlot2D::render()
{
    vertex_buffer_.render(num_elements_);
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
template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    OutputData output_data;
    output_data.points_ptr = new float[2 * input_params.num_elements];

    size_t idx = 0;

    const T* const x_data = reinterpret_cast<const T* const>(input_data);
    const T* const y_data = reinterpret_cast<const T* const>(input_data) + input_params.num_elements;

    for (size_t k = 0; k < input_params.num_elements; k++)
    {
        output_data.points_ptr[idx] = x_data[k];
        output_data.points_ptr[idx + 1] = y_data[k];

        idx += 2;
    }

    return output_data;
}
}  // namespace
