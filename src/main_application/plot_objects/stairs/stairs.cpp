#include "main_application/plot_objects/stairs/stairs.h"

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

Stairs::Stairs(std::unique_ptr<const ReceivedData> received_data,
               const CommunicationHeader& hdr,
               const Properties& props,
               const ShaderCollection shader_collection, ColorPicker& color_picker)
    : PlotObjectBase(std::move(received_data), hdr, props, shader_collection, color_picker),
      vertex_buffer_{OGLPrimitiveType::LINE_STRIP}
{
    if (type_ != Function::STAIRS)
    {
        throw std::runtime_error("Invalid function type for Stairs!");
    }

    const InputParams input_params{num_elements_};
    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    vertex_buffer_.addBuffer(output_data.points_ptr, (num_elements_ * 2 - 1), 2);

    delete[] output_data.points_ptr;
}

void Stairs::findMinMax()
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

void Stairs::render()
{
    vertex_buffer_.render(num_elements_ * 2 - 1);
}

Stairs::~Stairs() {}

namespace
{
template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    const size_t num_support_points = input_params.num_elements - 1;

    OutputData output_data;
    output_data.points_ptr = new float[2 * (input_params.num_elements + num_support_points)];

    const T* const input_data_dt_x = reinterpret_cast<const T* const>(input_data);
    const T* const input_data_dt_y = input_data_dt_x + input_params.num_elements;

    size_t idx = 0;

    for (size_t k = 0; k < (input_params.num_elements - 1); k++)
    {
        output_data.points_ptr[idx] = input_data_dt_x[k];
        output_data.points_ptr[idx + 1] = input_data_dt_y[k];

        output_data.points_ptr[idx + 2] = input_data_dt_x[k + 1];
        output_data.points_ptr[idx + 3] = input_data_dt_y[k];

        idx += 4;
    }

    output_data.points_ptr[idx] = input_data_dt_x[input_params.num_elements - 1U];
    output_data.points_ptr[idx + 1] = input_data_dt_y[input_params.num_elements - 1U];

    return output_data;
}

}  // namespace
