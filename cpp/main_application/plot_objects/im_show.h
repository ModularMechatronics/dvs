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

   

public:
    ImShow();
    ImShow(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~ImShow();

    void visualize() const override;
};

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

    const int width = dims_.cols;
    const int height = dims_.rows;

    // Give the image to OpenGL
    

    const float x = 0, y = 0;
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // if (blendFlag) glEnable(GL_BLEND);

}

void ImShow::visualize() const
{
    std::cout << "Imshow visualize!" << std::endl;
    if(!has_run_)
    {
        const int width = dims_.cols;
        const int height = dims_.rows;

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
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    
}

ImShow::~ImShow()
{
    img_.setInternalData(nullptr, 0, 0);  // Hack
}

#endif
