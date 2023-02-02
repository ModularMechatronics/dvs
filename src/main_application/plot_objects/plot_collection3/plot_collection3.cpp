#include "main_application/plot_objects/plot_collection3/plot_collection3.h"

#include "outer_converter.h"

namespace
{
struct OutputData
{
    float* data_ptr;
    Vec3d min_vec;
    Vec3d max_vec;
};

struct InputParams
{
    size_t num_objects;
    size_t num_bytes_per_element;
    size_t num_points;
    Vector<uint16_t> vector_lengths;

    InputParams() = default;
    InputParams(const size_t num_objects_,
                const size_t num_bytes_per_element_,
                const size_t num_points_,
                const Vector<uint16_t>& vector_lengths_)
        : num_objects{num_objects_},
          num_bytes_per_element{num_bytes_per_element_},
          num_points{num_points_},
          vector_lengths{vector_lengths_}
    {
    }
};

template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params);

struct Converter
{
    template <class T> OutputData convert(const uint8_t* const input_data, const InputParams& input_params) const
    {
        return convertData<T>(input_data, input_params);
    }
};

template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    const size_t total_num_bytes = input_params.num_points * 3 * input_params.num_bytes_per_element;
    const size_t num_bytes_per_collection = input_params.vector_lengths.sum() * input_params.num_bytes_per_element;

    const T* data_x = reinterpret_cast<const T*>(input_data);
    const T* data_y = reinterpret_cast<const T*>(input_data + num_bytes_per_collection);
    const T* data_z = reinterpret_cast<const T*>(input_data + 2 * num_bytes_per_collection);

    OutputData output_data;
    output_data.data_ptr = new float[total_num_bytes];

    size_t idx_offset = 0;
    size_t idx = 0;

    Vec3d min_vec, max_vec;

    min_vec.x = data_x[0];
    min_vec.y = data_y[0];
    min_vec.z = data_z[0];

    max_vec.x = data_x[0];
    max_vec.y = data_y[0];
    max_vec.z = data_z[0];

    for (size_t i = 0; i < input_params.num_objects; i++)
    {
        for (size_t k = 0; k < input_params.vector_lengths(i) - 1; k++)
        {
            const T x_val = data_x[idx_offset + k];
            const T y_val = data_y[idx_offset + k];
            const T z_val = data_z[idx_offset + k];

            min_vec.x = x_val < min_vec.x ? x_val : min_vec.x;
            min_vec.y = y_val < min_vec.y ? y_val : min_vec.y;
            min_vec.z = z_val < min_vec.z ? z_val : min_vec.z;

            max_vec.x = x_val > max_vec.x ? x_val : max_vec.x;
            max_vec.y = y_val > max_vec.y ? y_val : max_vec.y;
            max_vec.z = z_val > max_vec.z ? z_val : max_vec.z;

            output_data.data_ptr[idx] = x_val;
            output_data.data_ptr[idx + 1] = y_val;
            output_data.data_ptr[idx + 2] = z_val;

            output_data.data_ptr[idx + 3] = data_x[idx_offset + k + 1];
            output_data.data_ptr[idx + 4] = data_y[idx_offset + k + 1];
            output_data.data_ptr[idx + 5] = data_z[idx_offset + k + 1];

            idx += 6;
        }
        idx_offset += input_params.vector_lengths(i);
    }

    output_data.min_vec = min_vec;
    output_data.max_vec = max_vec;

    return output_data;
}

}  // namespace

PlotCollection3D::PlotCollection3D(std::unique_ptr<const ReceivedData> received_data,
                                   const CommunicationHeader& hdr,
                                   const Properties& props,
                                   const ShaderCollection shader_collection, ColorPicker& color_picker)
    : PlotObjectBase(received_data, hdr, props, shader_collection, color_picker), vertex_buffer_{OGLPrimitiveType::LINES}
{
    if (type_ != Function::PLOT_COLLECTION3)
    {
        throw std::runtime_error("Invalid function type for PlotCollection3D!");
    }

    num_points_ = 0;
    num_objects_ = hdr.get(CommunicationHeaderObjectType::NUM_OBJECTS).as<uint32_t>();

    Vector<uint16_t> vector_lengths(num_objects_);

    std::memcpy(vector_lengths.data(), data_ptr_, num_objects_ * sizeof(uint16_t));

    for (size_t k = 0; k < num_objects_; k++)
    {
        num_points_ += (vector_lengths(k) - 1) * 2;
    }

    // Advance pointer to account for first bytes where 'vector_lengths' are stored
    data_ptr_ += num_objects_ * sizeof(uint16_t);

    const InputParams input_params{num_objects_, num_bytes_per_element_, num_points_, vector_lengths};
    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    min_vec = output_data.min_vec;
    max_vec = output_data.max_vec;

    vertex_buffer_.addBuffer(output_data.data_ptr, num_points_, 3);

    delete[] output_data.data_ptr;
}

void PlotCollection3D::findMinMax()
{
    // Already calculated in constructor phase
}

void PlotCollection3D::render()
{
    vertex_buffer_.render(num_points_);
}

PlotCollection3D::~PlotCollection3D() {}
