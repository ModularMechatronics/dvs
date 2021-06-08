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

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

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
    has_run_ = false;

    color_map_ = color_maps::rainbowf;

    img_.setInternalData(reinterpret_cast<double*>(data_ptr_), dims_.rows, dims_.cols);

    width = dims_.cols;
    height = dims_.rows;

    pixel_data_ = new unsigned char[width * height * 3];
    for(int k = 0; k < (width * height * 3); k++)
    {
        pixel_data_[k] = 2 * k + 3;
    }

    // const std::string vtx_path = "/Users/annotelldaniel/work/repos/dvs/cpp/main_application/plot_objects/img_shader.vertex";
    // const std::string frg_path = "/Users/annotelldaniel/work/repos/dvs/cpp/main_application/plot_objects/img_shader.fragment";

    const std::string vtx_path = "/Users/annotelldaniel/work/repos/dvs/cpp/main_application/plot_objects/tri.vertex";
    const std::string frg_path = "/Users/annotelldaniel/work/repos/dvs/cpp/main_application/plot_objects/tri.fragment";
    program_id_ = LoadShaders(vtx_path, frg_path);

    // Give the image to OpenGL
    texture_id_ = loadTexture(width, height, pixel_data_);

    // const float x = 0, y = 0;
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // if (blendFlag) glEnable(GL_BLEND);

}



void ImShow::visualize()
{
    // if(!has_run_)
    {
        has_run_ = true;

        
        GLuint VertexArrayID;
        glGenVertexArraysAPPLE(1, &VertexArrayID);
        glBindVertexArrayAPPLE(VertexArrayID);

        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glUseProgram(program_id_);

        glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);


        // glDrawArrays(GL_TRIANGLES, 0, 3*3);
        /*glUseProgram(program_id_);
        
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, pixel_data_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        GLuint VertexArrayID;
        glGenVertexArraysAPPLE(1, &VertexArrayID);
        glBindVertexArrayAPPLE(VertexArrayID);
        GLuint vertexbuffer;
        // Generate 1 buffer, put the resulting identifier in vertexbuffer
        glGenBuffers(1, &vertexbuffer);
        // The following commands will talk about our 'vertexbuffer' buffer
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        // Give our vertices to OpenGL.
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
        0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glBindVertexArrayAPPLE(0);
        glDisableVertexAttribArray(0);*/

        /*
        glEnable(GL_BLEND);

        GLuint textureID;
        glGenTextures(1, &textureID);

        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data_ptr_);


        const float x = 0, y = 0;

        GLfloat TexCoord[] = {0, 0,
            1, 0,
            1, 1,
            0, 1,
        };

        GLubyte indices[] = {0, 1, 2,
                             0, 2, 3};

        GLfloat Vertices[] = {(float)x, (float)y, 0,
                            (float)x + width, (float)y, 0,
                            (float)x + (float)width, (float)y + (float)height, 0,
                            (float)x, (float)y + (float)height, 0};

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, Vertices);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, 0, TexCoord);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);*/
    }
}

ImShow::~ImShow()
{
    img_.setInternalData(nullptr, 0, 0);  // Hack
}

#endif
