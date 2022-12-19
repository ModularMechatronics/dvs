#include "main_application/plot_objects/im_show/im_show.h"

#include "outer_converter.h"

using namespace dvs::internal;

namespace
{
struct OutputData
{
    float* points_ptr;
    float* color_ptr;
    float* alpha_ptr;
};

struct InputParams
{
    size_t num_channels;
    Dimension2D dims;
    float z_offset;
    float multiplier;

    InputParams() {}
    InputParams(const size_t num_channels_, const Dimension2D& dims_, const float z_offset_, const DataType dt)
        : num_channels{num_channels_}, dims{dims_}, z_offset{z_offset_}
    {
        // Multiplier for FLOAT and DOUBLE is 1.0f, so that's left as the default case
        multiplier = 1.0f;
        multiplier = (dt == DataType::INT8) ? (1.0f / 127.0f) : multiplier;
        multiplier = (dt == DataType::INT16) ? (1.0f / 32767.0f) : multiplier;
        multiplier = (dt == DataType::INT32) ? (1.0f / 2147483647.0f) : multiplier;
        multiplier = (dt == DataType::INT64) ? (1.0f / 9223372036854775807.0f) : multiplier;
        multiplier = (dt == DataType::UINT8) ? (1.0f / 255.0f) : multiplier;
        multiplier = (dt == DataType::UINT16) ? (1.0f / 65535.0f) : multiplier;
        multiplier = (dt == DataType::UINT32) ? (1.0f / 4294967295.0f) : multiplier;
        multiplier = (dt == DataType::UINT64) ? (1.0f / 18446744073709551615.0f) : multiplier;
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
}  // namespace

void ImShow::findMinMax()
{
    min_vec.x = 0;
    min_vec.y = 0;
    min_vec.z = -1.0;

    max_vec.x = width_;
    max_vec.y = height_;
    max_vec.z = 1.0;
}

ImShow::ImShow(std::unique_ptr<const ReceivedData> received_data,
               const CommunicationHeader& hdr,
               const Properties& props,
               const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, props, shader_collection),
      vertex_buffer_{OGLPrimitiveType::TRIANGLES}
{
    if (type_ != Function::IM_SHOW)
    {
        throw std::runtime_error("Invalid function type for Surf!");
    }

    // https://stackoverflow.com/questions/34963324/c-opengl-mesh-rendering

    dims_ = hdr.get(CommunicationHeaderObjectType::DIMENSION_2D).as<Dimension2D>();
    num_channels_ = hdr.get(CommunicationHeaderObjectType::NUM_CHANNELS).as<uint8_t>();

    width_ = dims_.cols;
    height_ = dims_.rows;

    const InputParams input_params{num_channels_, dims_, z_offset_, data_type_};
    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    findMinMax();

    vertex_buffer_.addBuffer(output_data.points_ptr, 6 * dims_.rows * dims_.cols, 3);
    vertex_buffer_.addBuffer(output_data.color_ptr, 6 * dims_.rows * dims_.cols, 3);

    if ((num_channels_ == 4) || (num_channels_ == 2))
    {
        vertex_buffer_.addBuffer(output_data.alpha_ptr, 6 * dims_.rows * dims_.cols, 1);

        delete[] output_data.alpha_ptr;
    }

    delete[] output_data.points_ptr;
    delete[] output_data.color_ptr;
}

void ImShow::render()
{
    glEnable(GL_BLEND);

    glUseProgram(shader_collection_.img_plot_shader.programId());

    preRender(shader_collection_.img_plot_shader);

    if ((num_channels_ == 4) || (num_channels_ == 2))
    {
        glUniform1i(glGetUniformLocation(shader_collection_.img_plot_shader.programId(), "use_global_alpha"),
                    static_cast<int>(0));
    }
    else
    {
        glUniform1i(glGetUniformLocation(shader_collection_.img_plot_shader.programId(), "use_global_alpha"),
                    static_cast<int>(1));
        glUniform1f(glGetUniformLocation(shader_collection_.img_plot_shader.programId(), "global_alpha"), alpha_);
    }

    vertex_buffer_.render(dims_.rows * dims_.cols * 6);

    glUseProgram(shader_collection_.basic_plot_shader.programId());

    glDisable(GL_BLEND);
}

ImShow::~ImShow() {}

namespace
{
template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    OutputData output_data;

    if (input_params.num_channels == 4)
    {
        const ImageRGBAConstView<T> img_c4{
            reinterpret_cast<const T* const>(input_data), input_params.dims.rows, input_params.dims.cols};

        output_data.points_ptr = new float[input_params.dims.rows * input_params.dims.cols * 6 * 3];
        output_data.color_ptr = new float[input_params.dims.rows * input_params.dims.cols * 6 * 3];
        output_data.alpha_ptr = new float[input_params.dims.rows * input_params.dims.cols * 6 * 6];
        size_t idx = 0;
        size_t idx_alpha = 0;

        for (size_t r = 0; r < input_params.dims.rows; r++)
        {
            for (size_t c = 0; c < input_params.dims.cols; c++)
            {
                const size_t idx0_x = idx;
                const size_t idx0_y = idx + 1;
                const size_t idx0_z = idx + 2;

                const size_t idx1_x = idx + 3;
                const size_t idx1_y = idx + 4;
                const size_t idx1_z = idx + 5;

                const size_t idx2_x = idx + 6;
                const size_t idx2_y = idx + 7;
                const size_t idx2_z = idx + 8;

                const size_t idx3_x = idx + 9;
                const size_t idx3_y = idx + 10;
                const size_t idx3_z = idx + 11;

                const size_t idx4_x = idx + 12;
                const size_t idx4_y = idx + 13;
                const size_t idx4_z = idx + 14;

                const size_t idx5_x = idx + 15;
                const size_t idx5_y = idx + 16;
                const size_t idx5_z = idx + 17;

                const size_t idx0_r = idx + 0;
                const size_t idx0_g = idx + 1;
                const size_t idx0_b = idx + 2;

                const size_t idx1_r = idx + 3;
                const size_t idx1_g = idx + 4;
                const size_t idx1_b = idx + 5;

                const size_t idx2_r = idx + 6;
                const size_t idx2_g = idx + 7;
                const size_t idx2_b = idx + 8;

                const size_t idx3_r = idx + 9;
                const size_t idx3_g = idx + 10;
                const size_t idx3_b = idx + 11;

                const size_t idx4_r = idx + 12;
                const size_t idx4_g = idx + 13;
                const size_t idx4_b = idx + 14;

                const size_t idx5_r = idx + 15;
                const size_t idx5_g = idx + 16;
                const size_t idx5_b = idx + 17;
                idx = idx + 18;

                output_data.points_ptr[idx0_x] = c;
                output_data.points_ptr[idx1_x] = c + 1;
                output_data.points_ptr[idx2_x] = c;
                output_data.points_ptr[idx3_x] = c + 1;
                output_data.points_ptr[idx4_x] = c + 1;
                output_data.points_ptr[idx5_x] = c;

                output_data.points_ptr[idx0_y] = r;
                output_data.points_ptr[idx1_y] = r;
                output_data.points_ptr[idx2_y] = r + 1;
                output_data.points_ptr[idx3_y] = r;
                output_data.points_ptr[idx4_y] = r + 1;
                output_data.points_ptr[idx5_y] = r + 1;

                output_data.points_ptr[idx0_z] = input_params.z_offset;
                output_data.points_ptr[idx1_z] = input_params.z_offset;
                output_data.points_ptr[idx2_z] = input_params.z_offset;
                output_data.points_ptr[idx3_z] = input_params.z_offset;
                output_data.points_ptr[idx4_z] = input_params.z_offset;
                output_data.points_ptr[idx5_z] = input_params.z_offset;

                const float color_val_r = static_cast<float>(img_c4(r, c, 0)) * input_params.multiplier;
                const float color_val_g = static_cast<float>(img_c4(r, c, 1)) * input_params.multiplier;
                const float color_val_b = static_cast<float>(img_c4(r, c, 2)) * input_params.multiplier;
                const float color_val_a = static_cast<float>(img_c4(r, c, 3)) * input_params.multiplier;

                output_data.color_ptr[idx0_r] = color_val_r;
                output_data.color_ptr[idx1_r] = color_val_r;
                output_data.color_ptr[idx2_r] = color_val_r;
                output_data.color_ptr[idx3_r] = color_val_r;
                output_data.color_ptr[idx4_r] = color_val_r;
                output_data.color_ptr[idx5_r] = color_val_r;

                output_data.color_ptr[idx0_g] = color_val_g;
                output_data.color_ptr[idx1_g] = color_val_g;
                output_data.color_ptr[idx2_g] = color_val_g;
                output_data.color_ptr[idx3_g] = color_val_g;
                output_data.color_ptr[idx4_g] = color_val_g;
                output_data.color_ptr[idx5_g] = color_val_g;

                output_data.color_ptr[idx0_b] = color_val_b;
                output_data.color_ptr[idx1_b] = color_val_b;
                output_data.color_ptr[idx2_b] = color_val_b;
                output_data.color_ptr[idx3_b] = color_val_b;
                output_data.color_ptr[idx4_b] = color_val_b;
                output_data.color_ptr[idx5_b] = color_val_b;

                output_data.alpha_ptr[idx_alpha + 0] = color_val_a;
                output_data.alpha_ptr[idx_alpha + 1] = color_val_a;
                output_data.alpha_ptr[idx_alpha + 2] = color_val_a;
                output_data.alpha_ptr[idx_alpha + 3] = color_val_a;
                output_data.alpha_ptr[idx_alpha + 4] = color_val_a;
                output_data.alpha_ptr[idx_alpha + 5] = color_val_a;

                idx_alpha += 6;
            }
        }
    }
    else if (input_params.num_channels == 2)
    {
        const ImageGrayAlphaConstView<T> img_ga{
            reinterpret_cast<const T* const>(input_data), input_params.dims.rows, input_params.dims.cols};

        output_data.points_ptr = new float[input_params.dims.rows * input_params.dims.cols * 6 * 6];
        output_data.color_ptr = new float[input_params.dims.rows * input_params.dims.cols * 6 * 3];
        output_data.alpha_ptr = new float[input_params.dims.rows * input_params.dims.cols * 6 * 6];
        size_t idx = 0;
        size_t idx_alpha = 0;

        for (size_t r = 0; r < input_params.dims.rows; r++)
        {
            for (size_t c = 0; c < input_params.dims.cols; c++)
            {
                const size_t idx0_x = idx;
                const size_t idx0_y = idx + 1;
                const size_t idx0_z = idx + 2;

                const size_t idx1_x = idx + 3;
                const size_t idx1_y = idx + 4;
                const size_t idx1_z = idx + 5;

                const size_t idx2_x = idx + 6;
                const size_t idx2_y = idx + 7;
                const size_t idx2_z = idx + 8;

                const size_t idx3_x = idx + 9;
                const size_t idx3_y = idx + 10;
                const size_t idx3_z = idx + 11;

                const size_t idx4_x = idx + 12;
                const size_t idx4_y = idx + 13;
                const size_t idx4_z = idx + 14;

                const size_t idx5_x = idx + 15;
                const size_t idx5_y = idx + 16;
                const size_t idx5_z = idx + 17;

                const size_t idx0_r = idx + 0;
                const size_t idx0_g = idx + 1;
                const size_t idx0_b = idx + 2;

                const size_t idx1_r = idx + 3;
                const size_t idx1_g = idx + 4;
                const size_t idx1_b = idx + 5;

                const size_t idx2_r = idx + 6;
                const size_t idx2_g = idx + 7;
                const size_t idx2_b = idx + 8;

                const size_t idx3_r = idx + 9;
                const size_t idx3_g = idx + 10;
                const size_t idx3_b = idx + 11;

                const size_t idx4_r = idx + 12;
                const size_t idx4_g = idx + 13;
                const size_t idx4_b = idx + 14;

                const size_t idx5_r = idx + 15;
                const size_t idx5_g = idx + 16;
                const size_t idx5_b = idx + 17;
                idx = idx + 18;

                output_data.points_ptr[idx0_x] = c;
                output_data.points_ptr[idx1_x] = c + 1;
                output_data.points_ptr[idx2_x] = c;
                output_data.points_ptr[idx3_x] = c + 1;
                output_data.points_ptr[idx4_x] = c + 1;
                output_data.points_ptr[idx5_x] = c;

                output_data.points_ptr[idx0_y] = r;
                output_data.points_ptr[idx1_y] = r;
                output_data.points_ptr[idx2_y] = r + 1;
                output_data.points_ptr[idx3_y] = r;
                output_data.points_ptr[idx4_y] = r + 1;
                output_data.points_ptr[idx5_y] = r + 1;

                output_data.points_ptr[idx0_z] = input_params.z_offset;
                output_data.points_ptr[idx1_z] = input_params.z_offset;
                output_data.points_ptr[idx2_z] = input_params.z_offset;
                output_data.points_ptr[idx3_z] = input_params.z_offset;
                output_data.points_ptr[idx4_z] = input_params.z_offset;
                output_data.points_ptr[idx5_z] = input_params.z_offset;

                const float color_val_g = static_cast<float>(img_ga(r, c, 0)) * input_params.multiplier;
                const float color_val_a = static_cast<float>(img_ga(r, c, 1)) * input_params.multiplier;

                output_data.color_ptr[idx0_r] = color_val_g;
                output_data.color_ptr[idx1_r] = color_val_g;
                output_data.color_ptr[idx2_r] = color_val_g;
                output_data.color_ptr[idx3_r] = color_val_g;
                output_data.color_ptr[idx4_r] = color_val_g;
                output_data.color_ptr[idx5_r] = color_val_g;

                output_data.color_ptr[idx0_g] = color_val_g;
                output_data.color_ptr[idx1_g] = color_val_g;
                output_data.color_ptr[idx2_g] = color_val_g;
                output_data.color_ptr[idx3_g] = color_val_g;
                output_data.color_ptr[idx4_g] = color_val_g;
                output_data.color_ptr[idx5_g] = color_val_g;

                output_data.color_ptr[idx0_b] = color_val_g;
                output_data.color_ptr[idx1_b] = color_val_g;
                output_data.color_ptr[idx2_b] = color_val_g;
                output_data.color_ptr[idx3_b] = color_val_g;
                output_data.color_ptr[idx4_b] = color_val_g;
                output_data.color_ptr[idx5_b] = color_val_g;

                output_data.alpha_ptr[idx_alpha + 0] = color_val_a;
                output_data.alpha_ptr[idx_alpha + 1] = color_val_a;
                output_data.alpha_ptr[idx_alpha + 2] = color_val_a;
                output_data.alpha_ptr[idx_alpha + 3] = color_val_a;
                output_data.alpha_ptr[idx_alpha + 4] = color_val_a;
                output_data.alpha_ptr[idx_alpha + 5] = color_val_a;

                idx_alpha += 6;
            }
        }
    }
    else if (input_params.num_channels == 3)
    {
        const ImageRGBConstView<T> img_c3{
            reinterpret_cast<const T* const>(input_data), input_params.dims.rows, input_params.dims.cols};

        output_data.points_ptr = new float[input_params.dims.rows * input_params.dims.cols * 6 * 6];
        output_data.color_ptr = new float[input_params.dims.rows * input_params.dims.cols * 6 * 3];
        size_t idx = 0;

        for (size_t r = 0; r < input_params.dims.rows; r++)
        {
            for (size_t c = 0; c < input_params.dims.cols; c++)
            {
                const size_t idx0_x = idx;
                const size_t idx0_y = idx + 1;
                const size_t idx0_z = idx + 2;

                const size_t idx1_x = idx + 3;
                const size_t idx1_y = idx + 4;
                const size_t idx1_z = idx + 5;

                const size_t idx2_x = idx + 6;
                const size_t idx2_y = idx + 7;
                const size_t idx2_z = idx + 8;

                const size_t idx3_x = idx + 9;
                const size_t idx3_y = idx + 10;
                const size_t idx3_z = idx + 11;

                const size_t idx4_x = idx + 12;
                const size_t idx4_y = idx + 13;
                const size_t idx4_z = idx + 14;

                const size_t idx5_x = idx + 15;
                const size_t idx5_y = idx + 16;
                const size_t idx5_z = idx + 17;

                const size_t idx0_r = idx + 0;
                const size_t idx0_g = idx + 1;
                const size_t idx0_b = idx + 2;

                const size_t idx1_r = idx + 3;
                const size_t idx1_g = idx + 4;
                const size_t idx1_b = idx + 5;

                const size_t idx2_r = idx + 6;
                const size_t idx2_g = idx + 7;
                const size_t idx2_b = idx + 8;

                const size_t idx3_r = idx + 9;
                const size_t idx3_g = idx + 10;
                const size_t idx3_b = idx + 11;

                const size_t idx4_r = idx + 12;
                const size_t idx4_g = idx + 13;
                const size_t idx4_b = idx + 14;

                const size_t idx5_r = idx + 15;
                const size_t idx5_g = idx + 16;
                const size_t idx5_b = idx + 17;
                idx = idx + 18;

                output_data.points_ptr[idx0_x] = c;
                output_data.points_ptr[idx1_x] = c + 1;
                output_data.points_ptr[idx2_x] = c;
                output_data.points_ptr[idx3_x] = c + 1;
                output_data.points_ptr[idx4_x] = c + 1;
                output_data.points_ptr[idx5_x] = c;

                output_data.points_ptr[idx0_y] = r;
                output_data.points_ptr[idx1_y] = r;
                output_data.points_ptr[idx2_y] = r + 1;
                output_data.points_ptr[idx3_y] = r;
                output_data.points_ptr[idx4_y] = r + 1;
                output_data.points_ptr[idx5_y] = r + 1;

                output_data.points_ptr[idx0_z] = input_params.z_offset;
                output_data.points_ptr[idx1_z] = input_params.z_offset;
                output_data.points_ptr[idx2_z] = input_params.z_offset;
                output_data.points_ptr[idx3_z] = input_params.z_offset;
                output_data.points_ptr[idx4_z] = input_params.z_offset;
                output_data.points_ptr[idx5_z] = input_params.z_offset;

                const float color_val_r = static_cast<float>(img_c3(r, c, 0)) * input_params.multiplier;
                const float color_val_g = static_cast<float>(img_c3(r, c, 1)) * input_params.multiplier;
                const float color_val_b = static_cast<float>(img_c3(r, c, 2)) * input_params.multiplier;

                output_data.color_ptr[idx0_r] = color_val_r;
                output_data.color_ptr[idx1_r] = color_val_r;
                output_data.color_ptr[idx2_r] = color_val_r;
                output_data.color_ptr[idx3_r] = color_val_r;
                output_data.color_ptr[idx4_r] = color_val_r;
                output_data.color_ptr[idx5_r] = color_val_r;

                output_data.color_ptr[idx0_g] = color_val_g;
                output_data.color_ptr[idx1_g] = color_val_g;
                output_data.color_ptr[idx2_g] = color_val_g;
                output_data.color_ptr[idx3_g] = color_val_g;
                output_data.color_ptr[idx4_g] = color_val_g;
                output_data.color_ptr[idx5_g] = color_val_g;

                output_data.color_ptr[idx0_b] = color_val_b;
                output_data.color_ptr[idx1_b] = color_val_b;
                output_data.color_ptr[idx2_b] = color_val_b;
                output_data.color_ptr[idx3_b] = color_val_b;
                output_data.color_ptr[idx4_b] = color_val_b;
                output_data.color_ptr[idx5_b] = color_val_b;
            }
        }
    }
    else if (input_params.num_channels == 1)
    {
        const ImageGrayConstView<T> img_c1{
            reinterpret_cast<const T* const>(input_data), input_params.dims.rows, input_params.dims.cols};

        output_data.points_ptr = new float[input_params.dims.rows * input_params.dims.cols * 6 * 6];
        output_data.color_ptr = new float[input_params.dims.rows * input_params.dims.cols * 6 * 3];
        size_t idx = 0;

        for (size_t r = 0; r < input_params.dims.rows; r++)
        {
            for (size_t c = 0; c < input_params.dims.cols; c++)
            {
                const size_t idx0_x = idx;
                const size_t idx0_y = idx + 1;
                const size_t idx0_z = idx + 2;

                const size_t idx1_x = idx + 3;
                const size_t idx1_y = idx + 4;
                const size_t idx1_z = idx + 5;

                const size_t idx2_x = idx + 6;
                const size_t idx2_y = idx + 7;
                const size_t idx2_z = idx + 8;

                const size_t idx3_x = idx + 9;
                const size_t idx3_y = idx + 10;
                const size_t idx3_z = idx + 11;

                const size_t idx4_x = idx + 12;
                const size_t idx4_y = idx + 13;
                const size_t idx4_z = idx + 14;

                const size_t idx5_x = idx + 15;
                const size_t idx5_y = idx + 16;
                const size_t idx5_z = idx + 17;

                const size_t idx0_r = idx + 0;
                const size_t idx0_g = idx + 1;
                const size_t idx0_b = idx + 2;

                const size_t idx1_r = idx + 3;
                const size_t idx1_g = idx + 4;
                const size_t idx1_b = idx + 5;

                const size_t idx2_r = idx + 6;
                const size_t idx2_g = idx + 7;
                const size_t idx2_b = idx + 8;

                const size_t idx3_r = idx + 9;
                const size_t idx3_g = idx + 10;
                const size_t idx3_b = idx + 11;

                const size_t idx4_r = idx + 12;
                const size_t idx4_g = idx + 13;
                const size_t idx4_b = idx + 14;

                const size_t idx5_r = idx + 15;
                const size_t idx5_g = idx + 16;
                const size_t idx5_b = idx + 17;
                idx = idx + 18;

                output_data.points_ptr[idx0_x] = c;
                output_data.points_ptr[idx1_x] = c + 1;
                output_data.points_ptr[idx2_x] = c;
                output_data.points_ptr[idx3_x] = c + 1;
                output_data.points_ptr[idx4_x] = c + 1;
                output_data.points_ptr[idx5_x] = c;

                output_data.points_ptr[idx0_y] = r;
                output_data.points_ptr[idx1_y] = r;
                output_data.points_ptr[idx2_y] = r + 1;
                output_data.points_ptr[idx3_y] = r;
                output_data.points_ptr[idx4_y] = r + 1;
                output_data.points_ptr[idx5_y] = r + 1;

                output_data.points_ptr[idx0_z] = input_params.z_offset;
                output_data.points_ptr[idx1_z] = input_params.z_offset;
                output_data.points_ptr[idx2_z] = input_params.z_offset;
                output_data.points_ptr[idx3_z] = input_params.z_offset;
                output_data.points_ptr[idx4_z] = input_params.z_offset;
                output_data.points_ptr[idx5_z] = input_params.z_offset;

                const float color_val_r = static_cast<float>(img_c1(r, c)) * input_params.multiplier;
                const float color_val_g = static_cast<float>(img_c1(r, c)) * input_params.multiplier;
                const float color_val_b = static_cast<float>(img_c1(r, c)) * input_params.multiplier;

                output_data.color_ptr[idx0_r] = color_val_r;
                output_data.color_ptr[idx1_r] = color_val_r;
                output_data.color_ptr[idx2_r] = color_val_r;
                output_data.color_ptr[idx3_r] = color_val_r;
                output_data.color_ptr[idx4_r] = color_val_r;
                output_data.color_ptr[idx5_r] = color_val_r;

                output_data.color_ptr[idx0_g] = color_val_g;
                output_data.color_ptr[idx1_g] = color_val_g;
                output_data.color_ptr[idx2_g] = color_val_g;
                output_data.color_ptr[idx3_g] = color_val_g;
                output_data.color_ptr[idx4_g] = color_val_g;
                output_data.color_ptr[idx5_g] = color_val_g;

                output_data.color_ptr[idx0_b] = color_val_b;
                output_data.color_ptr[idx1_b] = color_val_b;
                output_data.color_ptr[idx2_b] = color_val_b;
                output_data.color_ptr[idx3_b] = color_val_b;
                output_data.color_ptr[idx4_b] = color_val_b;
                output_data.color_ptr[idx5_b] = color_val_b;
            }
        }
    }
    else
    {
        std::cout << "Invalid number of channels: " << input_params.num_channels << std::endl;
    }

    return output_data;
}
}  // namespace
