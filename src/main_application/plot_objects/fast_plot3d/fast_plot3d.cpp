#include "main_application/plot_objects/fast_plot3d/fast_plot3d.h"

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

FastPlot3D::FastPlot3D(std::unique_ptr<const ReceivedData> received_data,
                       const CommunicationHeader& hdr,
                       const Properties& props,
                       const ShaderCollection shader_collection, ColorPicker& color_picker)
    : PlotObjectBase(std::move(received_data), hdr, props, shader_collection, color_picker),
      vertex_buffer_{OGLPrimitiveType::LINE_STRIP}
{
    if (type_ != Function::FAST_PLOT3)
    {
        throw std::runtime_error("Invalid function type for FastPlot3D!");
    }

    const InputParams input_params{num_elements_};
    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    vertex_buffer_.addBuffer(output_data.points_ptr, num_elements_, 3);

    delete[] output_data.points_ptr;
}

void FastPlot3D::findMinMax()
{
    std::tie<Vec3d, Vec3d>(min_vec, max_vec) =
        findMinMaxFromThreeVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);
}

void FastPlot3D::render()
{
    vertex_buffer_.render(num_elements_);
}

FastPlot3D::~FastPlot3D() {}

namespace
{
template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    OutputData output_data;
    output_data.points_ptr = new float[3 * input_params.num_elements];

    size_t idx = 0;

    const T* const x_data = reinterpret_cast<const T* const>(input_data);
    const T* const y_data = reinterpret_cast<const T* const>(input_data) + input_params.num_elements;
    const T* const z_data = reinterpret_cast<const T* const>(input_data) + 2 * input_params.num_elements;

    for (size_t k = 0; k < input_params.num_elements; k++)
    {
        output_data.points_ptr[idx] = x_data[k];
        output_data.points_ptr[idx + 1] = y_data[k];
        output_data.points_ptr[idx + 2] = z_data[k];

        idx += 3;
    }

    return output_data;
}

}  // namespace
