#include "main_application/plot_objects/stairs/stairs.h"

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
std::unique_ptr<ConvertedData> convertData(const uint8_t* const input_data, const InputParams& input_params);

struct Converter
{
    template <class T>
    std::unique_ptr<ConvertedData> convert(const uint8_t* const input_data, const InputParams& input_params) const
    {
        return convertData<T>(input_data, input_params);
    }
};

}  // namespace

Stairs::Stairs(const CommunicationHeader& hdr,
               ReceivedData& received_data,
               const std::unique_ptr<const ConvertedDataBase>& converted_data,
               
               const PlotObjectAttributes& plot_object_attributes,
               const PropertiesData& properties_data,
               const ShaderCollection& shader_collection,
               ColorPicker& color_picker)
    : PlotObjectBase(
          received_data, hdr, plot_object_attributes, properties_data, shader_collection, color_picker),
      vertex_buffer_{OGLPrimitiveType::LINE_STRIP}
{
    if (function_ != Function::STAIRS)
    {
        throw std::runtime_error("Invalid function type for Stairs!");
    }

    const ConvertedData* const converted_data_local = static_cast<const ConvertedData* const>(converted_data.get());

    vertex_buffer_.addBuffer(converted_data_local->points_ptr, (num_elements_ * 2 - 1), 2);
}

std::unique_ptr<const ConvertedDataBase> Stairs::convertRawData(const CommunicationHeader& hdr, const PlotObjectAttributes& attributes,
                                                                const PropertiesData& properties_data,
                                                                const uint8_t* const data_ptr)
{
    const InputParams input_params{attributes.num_elements};

    std::unique_ptr<const ConvertedDataBase> converted_data_base{
        applyConverter<ConvertedData>(data_ptr, attributes.data_type, Converter{}, input_params)};

    return converted_data_base;
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
template <typename T>
std::unique_ptr<ConvertedData> convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    const size_t num_support_points = input_params.num_elements - 1;

    ConvertedData* converted_data = new ConvertedData;
    converted_data->points_ptr = new float[2 * (input_params.num_elements + num_support_points)];

    const T* const input_data_dt_x = reinterpret_cast<const T* const>(input_data);
    const T* const input_data_dt_y = input_data_dt_x + input_params.num_elements;

    size_t idx = 0;

    for (size_t k = 0; k < (input_params.num_elements - 1); k++)
    {
        converted_data->points_ptr[idx] = input_data_dt_x[k];
        converted_data->points_ptr[idx + 1] = input_data_dt_y[k];

        converted_data->points_ptr[idx + 2] = input_data_dt_x[k + 1];
        converted_data->points_ptr[idx + 3] = input_data_dt_y[k];

        idx += 4;
    }

    converted_data->points_ptr[idx] = input_data_dt_x[input_params.num_elements - 1U];
    converted_data->points_ptr[idx + 1] = input_data_dt_y[input_params.num_elements - 1U];

    return std::unique_ptr<ConvertedData>(converted_data);
}

}  // namespace
