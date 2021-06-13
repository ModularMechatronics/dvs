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

    Matrixd img_;
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
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// ... which requires mipmaps. Generate them automatically.
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

    dims_ = hdr.getObjectFromType(FunctionHeaderObjectType::DIMENSION_2D).getAs<internal::Dimension2D>();

    color_map_ = color_maps::rainbowf;

    img_.setInternalData(reinterpret_cast<double*>(data_ptr_), dims_.rows, dims_.cols);

    width = dims_.cols;
    height = dims_.rows;

    // 4 vertices, 6 "elements" (xyz rgb) per vertex
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
            idx = idx + 12;

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

            const float color_val = img_(r, c);

            colors_ptr_[idx0_x] = color_val;
            colors_ptr_[idx1_x] = color_val;
            colors_ptr_[idx2_x] = color_val;
            colors_ptr_[idx3_x] = color_val;

            colors_ptr_[idx0_y] = color_val;
            colors_ptr_[idx1_y] = color_val;
            colors_ptr_[idx2_y] = color_val;
            colors_ptr_[idx3_y] = color_val;

            colors_ptr_[idx0_z] = color_val;
            colors_ptr_[idx1_z] = color_val;
            colors_ptr_[idx2_z] = color_val;
            colors_ptr_[idx3_z] = color_val;
        }
    }

    img_.setInternalData(nullptr, 0, 0);  // Hack

    findMinMax();
}

void ImShow::visualize()
{
    if(!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 * dims_.rows * dims_.cols, points_ptr_, GL_STATIC_DRAW);

        program_id_ = LoadShaders("/Users/annotelldaniel/work/repos/dvs/cpp/main_application/plot_objects/img_shader.vertex",
                                  "/Users/annotelldaniel/work/repos/dvs/cpp/main_application/plot_objects/img_shader.fragment");
        glUseProgram(program_id_);
        //glGenBuffers(1, &color_buffer_);
        //glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 * dims_.rows * dims_.cols, colors_ptr_, GL_STATIC_DRAW);
    }

    
    // setColor(face_color_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_QUADS, 0, 4 * 3 * dims_.rows * dims_.cols);
    glDisableVertexAttribArray(0);
    // glDisableVertexAttribArray(1);

}

ImShow::~ImShow()
{
}

#endif
