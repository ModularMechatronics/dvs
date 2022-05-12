#include "main_application/plot_objects/im_show/im_show.h"

void ImShow::findMinMax()
{
    min_vec.x = 0;
    min_vec.y = 0;
    min_vec.z = -1.0;

    max_vec.x = width;
    max_vec.y = height;
    max_vec.z = 1.0;
}

template <typename T> GLuint loadTexture(const int width, const int height, const T* data)
{
    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

ImShow::ImShow(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::IM_SHOW)
    {
        throw std::runtime_error("Invalid function type for Surf!");
    }

    // https://stackoverflow.com/questions/34963324/c-opengl-mesh-rendering

    dims_ = hdr.get(FunctionHeaderObjectType::DIMENSION_2D).as<internal::Dimension2D>();
    num_channels_ = hdr.get(FunctionHeaderObjectType::NUM_CHANNELS).as<uint8_t>();

    width = dims_.cols;
    height = dims_.rows;

    if(num_channels_ == 3)
    {
        img_c3_.setInternalData(reinterpret_cast<float*>(data_ptr_), dims_.rows, dims_.cols);

        points_ptr_ = new float[dims_.rows * dims_.cols * 6 * 6];
        colors_ptr_ = new float[dims_.rows * dims_.cols * 6 * 6];
        size_t idx = 0;

        for (size_t r = 0; r < dims_.rows; r++)
        {
            for (size_t c = 0; c < dims_.cols; c++)
            {
                const size_t idx0_x = idx;
                const size_t idx0_y = idx + 1;
                const size_t idx0_z = idx + 2;

                const size_t idx0_r = idx + 3;
                const size_t idx0_g = idx + 4;
                const size_t idx0_b = idx + 5;

                const size_t idx1_x = idx + 6;
                const size_t idx1_y = idx + 7;
                const size_t idx1_z = idx + 8;

                const size_t idx1_r = idx + 9;
                const size_t idx1_g = idx + 10;
                const size_t idx1_b = idx + 11;

                const size_t idx2_x = idx + 12;
                const size_t idx2_y = idx + 13;
                const size_t idx2_z = idx + 14;

                const size_t idx2_r = idx + 15;
                const size_t idx2_g = idx + 16;
                const size_t idx2_b = idx + 17;

                const size_t idx3_x = idx + 18;
                const size_t idx3_y = idx + 19;
                const size_t idx3_z = idx + 20;

                const size_t idx3_r = idx + 21;
                const size_t idx3_g = idx + 22;
                const size_t idx3_b = idx + 23;

                const size_t idx4_x = idx + 24;
                const size_t idx4_y = idx + 25;
                const size_t idx4_z = idx + 26;

                const size_t idx4_r = idx + 27;
                const size_t idx4_g = idx + 28;
                const size_t idx4_b = idx + 29;

                const size_t idx5_x = idx + 30;
                const size_t idx5_y = idx + 31;
                const size_t idx5_z = idx + 32;

                const size_t idx5_r = idx + 33;
                const size_t idx5_g = idx + 34;
                const size_t idx5_b = idx + 35;
                idx = idx + 36;

                points_ptr_[idx0_x] = c;
                points_ptr_[idx1_x] = c + 1;
                points_ptr_[idx2_x] = c;
                points_ptr_[idx3_x] = c + 1;
                points_ptr_[idx4_x] = c + 1;
                points_ptr_[idx5_x] = c;

                points_ptr_[idx0_y] = r;
                points_ptr_[idx1_y] = r;
                points_ptr_[idx2_y] = r + 1;
                points_ptr_[idx3_y] = r;
                points_ptr_[idx4_y] = r + 1;
                points_ptr_[idx5_y] = r + 1;

                points_ptr_[idx0_z] = 0.0f;
                points_ptr_[idx1_z] = 0.0f;
                points_ptr_[idx2_z] = 0.0f;
                points_ptr_[idx3_z] = 0.0f;
                points_ptr_[idx4_z] = 0.0f;
                points_ptr_[idx5_z] = 0.0f;

                const float color_val_r = img_c3_(r, c, 0);
                const float color_val_g = img_c3_(r, c, 1);
                const float color_val_b = img_c3_(r, c, 2);

                points_ptr_[idx0_r] = color_val_r;
                points_ptr_[idx1_r] = color_val_r;
                points_ptr_[idx2_r] = color_val_r;
                points_ptr_[idx3_r] = color_val_r;
                points_ptr_[idx4_r] = color_val_r;
                points_ptr_[idx5_r] = color_val_r;

                points_ptr_[idx0_g] = color_val_g;
                points_ptr_[idx1_g] = color_val_g;
                points_ptr_[idx2_g] = color_val_g;
                points_ptr_[idx3_g] = color_val_g;
                points_ptr_[idx4_g] = color_val_g;
                points_ptr_[idx5_g] = color_val_g;

                points_ptr_[idx0_b] = color_val_b;
                points_ptr_[idx1_b] = color_val_b;
                points_ptr_[idx2_b] = color_val_b;
                points_ptr_[idx3_b] = color_val_b;
                points_ptr_[idx4_b] = color_val_b;
                points_ptr_[idx5_b] = color_val_b;
            }
        }

        img_c3_.setInternalData(nullptr, 0, 0);  // Hack
    }
    else if(num_channels_ == 1)
    {
        img_c1_.setInternalData(reinterpret_cast<float*>(data_ptr_), dims_.rows, dims_.cols);

        points_ptr_ = new float[dims_.rows * dims_.cols * 4 * 3];
        colors_ptr_ = new float[dims_.rows * dims_.cols * 4 * 3];
        size_t idx = 0;

        for (size_t r = 0; r < dims_.rows; r++)
        {
            for (size_t c = 0; c < dims_.cols; c++)
            {
                const size_t idx0_x = idx;
                const size_t idx0_y = idx + 1;
                const size_t idx0_z = idx + 2;

                const size_t idx0_r = idx + 3;
                const size_t idx0_g = idx + 4;
                const size_t idx0_b = idx + 5;

                const size_t idx1_x = idx + 6;
                const size_t idx1_y = idx + 7;
                const size_t idx1_z = idx + 8;

                const size_t idx1_r = idx + 9;
                const size_t idx1_g = idx + 10;
                const size_t idx1_b = idx + 11;

                const size_t idx2_x = idx + 12;
                const size_t idx2_y = idx + 13;
                const size_t idx2_z = idx + 14;

                const size_t idx2_r = idx + 15;
                const size_t idx2_g = idx + 16;
                const size_t idx2_b = idx + 17;

                const size_t idx3_x = idx + 18;
                const size_t idx3_y = idx + 19;
                const size_t idx3_z = idx + 20;

                const size_t idx3_r = idx + 21;
                const size_t idx3_g = idx + 22;
                const size_t idx3_b = idx + 23;

                const size_t idx4_x = idx + 24;
                const size_t idx4_y = idx + 25;
                const size_t idx4_z = idx + 26;

                const size_t idx4_r = idx + 27;
                const size_t idx4_g = idx + 28;
                const size_t idx4_b = idx + 29;

                const size_t idx5_x = idx + 30;
                const size_t idx5_y = idx + 31;
                const size_t idx5_z = idx + 32;

                const size_t idx5_r = idx + 33;
                const size_t idx5_g = idx + 34;
                const size_t idx5_b = idx + 35;
                idx = idx + 36;

                points_ptr_[idx0_x] = c;
                points_ptr_[idx1_x] = c + 1;
                points_ptr_[idx2_x] = c;
                points_ptr_[idx3_x] = c + 1;
                points_ptr_[idx4_x] = c + 1;
                points_ptr_[idx5_x] = c;

                points_ptr_[idx0_y] = r;
                points_ptr_[idx1_y] = r;
                points_ptr_[idx2_y] = r + 1;
                points_ptr_[idx3_y] = r;
                points_ptr_[idx4_y] = r + 1;
                points_ptr_[idx5_y] = r + 1;

                points_ptr_[idx0_z] = 0.0f;
                points_ptr_[idx1_z] = 0.0f;
                points_ptr_[idx2_z] = 0.0f;
                points_ptr_[idx3_z] = 0.0f;
                points_ptr_[idx4_z] = 0.0f;
                points_ptr_[idx5_z] = 0.0f;

                const float color_val_r = img_c1_(r, c);
                const float color_val_g = img_c1_(r, c);
                const float color_val_b = img_c1_(r, c);

                points_ptr_[idx0_r] = color_val_r;
                points_ptr_[idx1_r] = color_val_r;
                points_ptr_[idx2_r] = color_val_r;
                points_ptr_[idx3_r] = color_val_r;
                points_ptr_[idx4_r] = color_val_r;
                points_ptr_[idx5_r] = color_val_r;

                points_ptr_[idx0_g] = color_val_g;
                points_ptr_[idx1_g] = color_val_g;
                points_ptr_[idx2_g] = color_val_g;
                points_ptr_[idx3_g] = color_val_g;
                points_ptr_[idx4_g] = color_val_g;
                points_ptr_[idx5_g] = color_val_g;

                points_ptr_[idx0_b] = color_val_b;
                points_ptr_[idx1_b] = color_val_b;
                points_ptr_[idx2_b] = color_val_b;
                points_ptr_[idx3_b] = color_val_b;
                points_ptr_[idx4_b] = color_val_b;
                points_ptr_[idx5_b] = color_val_b;
            }
        }

        img_c1_.setInternalData(nullptr, 0, 0);  // Hack
    }
    else
    {
        std::cout << "Invalid number of channels: " << num_channels_ << std::endl;
    }
    findMinMax();

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6 * dims_.rows * dims_.cols, points_ptr_, GL_STATIC_DRAW);

    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void ImShow::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, dims_.rows * dims_.cols * 6);
    glBindVertexArray(0);
    /*glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glVertexPointer(3, GL_FLOAT, 0, points_ptr_);
        glColorPointer(3, GL_FLOAT, 0, colors_ptr_);
    }

    glDrawArrays(GL_QUADS, 0, 4 * dims_.rows * dims_.cols);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);*/
}

ImShow::~ImShow()
{
    delete[] points_ptr_;
    delete[] colors_ptr_;
}
