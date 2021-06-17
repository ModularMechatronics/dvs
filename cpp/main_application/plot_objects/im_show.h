#ifndef IM_SHOW_H_
#define IM_SHOW_H_

// #include <arl/utilities/color_map.h>
#include "math/math.h"

#include <string>
#include <vector>

#include "main_application/misc/color_map.h"
#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"

bool has_run_;

class ImShow : public PlotObjectBase
{
private:
    RGBColorMap<float> color_map_;
    GLuint buffer_idx_;
    GLuint color_buffer_;

    float* points_ptr_;
    float* colors_ptr_;

    uint8_t num_channels_;

    ImageC3<float> img_;
    internal::Dimension2D dims_;
    unsigned char* pixel_data_;
    GLuint program_id_;
    GLuint texture_id_;
    int width;
    int height;
    void findMinMax() override;

public:
    ImShow();
    ImShow(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~ImShow();

    void visualize() override;
};

void ImShow::findMinMax()
{
    min_vec.x = 0;
    min_vec.y = 0;
    min_vec.z = -1.0;

    max_vec.x = width;
    max_vec.y = height;
    max_vec.z = 1.0;
}

template <typename T>
GLuint loadTexture(const int width, const int height, const T* data)
{
    GLuint textureID;
	glGenTextures(1, &textureID);
	
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

ImShow::ImShow(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::IM_SHOW)
    {
        throw std::runtime_error("Invalid function type for Surf!");
    }

    // https://stackoverflow.com/questions/34963324/c-opengl-mesh-rendering

    dims_ = hdr.getObjectFromType(FunctionHeaderObjectType::DIMENSION_2D).getAs<internal::Dimension2D>();
    num_channels_ = hdr.getObjectFromType(FunctionHeaderObjectType::NUM_CHANNELS).getAs<uint8_t>();

    color_map_ = color_maps::rainbowf;

    img_.setInternalData(reinterpret_cast<float*>(data_ptr_), dims_.rows, dims_.cols);

    width = dims_.cols;
    height = dims_.rows;

    // 4 vertices, 3 "elements" (xyz/rgb) per vertex
    points_ptr_ = new float[dims_.rows * dims_.cols * 4 * 3];
    colors_ptr_ = new float[dims_.rows * dims_.cols * 4 * 3];
    size_t idx = 0;

    for(size_t r = 0; r < dims_.rows; r++)
    {
        for(size_t c = 0; c < dims_.cols; c++)
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

            points_ptr_[idx0_x] = c;
            points_ptr_[idx1_x] = c + 1;
            points_ptr_[idx2_x] = c + 1;
            points_ptr_[idx3_x] = c;

            points_ptr_[idx0_y] = r;
            points_ptr_[idx1_y] = r;
            points_ptr_[idx2_y] = r + 1;
            points_ptr_[idx3_y] = r + 1;

            points_ptr_[idx0_z] = 0.0f;
            points_ptr_[idx1_z] = 0.0f;
            points_ptr_[idx2_z] = 0.0f;
            points_ptr_[idx3_z] = 0.0f;

            const float color_val_r = img_(r, c, 0);
            const float color_val_g = img_(r, c, 1);
            const float color_val_b = img_(r, c, 2);

            colors_ptr_[idx0_x] = color_val_r;
            colors_ptr_[idx1_x] = color_val_r;
            colors_ptr_[idx2_x] = color_val_r;
            colors_ptr_[idx3_x] = color_val_r;

            colors_ptr_[idx0_y] = color_val_g;
            colors_ptr_[idx1_y] = color_val_g;
            colors_ptr_[idx2_y] = color_val_g;
            colors_ptr_[idx3_y] = color_val_g;

            colors_ptr_[idx0_z] = color_val_b;
            colors_ptr_[idx1_z] = color_val_b;
            colors_ptr_[idx2_z] = color_val_b;
            colors_ptr_[idx3_z] = color_val_b;

            idx = idx + 12;
        }
    }

    img_.setInternalData(nullptr, 0, 0);  // Hack

    findMinMax();
}

void ImShow::visualize()
{
    // TODO: Make sure that the image is not mirrored
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    if(!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glVertexPointer(3, GL_FLOAT, 0, points_ptr_);
        glColorPointer(3, GL_FLOAT, 0, colors_ptr_);
    }
    

    glDrawArrays(GL_QUADS, 0, 4 * dims_.rows * dims_.cols);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

ImShow::~ImShow()
{
    delete[] points_ptr_;
    delete[] colors_ptr_;
}

#endif
