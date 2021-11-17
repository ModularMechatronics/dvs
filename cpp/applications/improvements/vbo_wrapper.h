#ifndef VBO_WRAPPER_H_
#define VBO_WRAPPER_H_

#include "opengl_low_level/opengl_header.h"
#include <stddef.h>

class VboWrapper3D
{
private:
    GLuint vertex_buffer_, vertex_buffer_array_, color_buffer_;
    size_t num_vertices_;

public:
    VboWrapper3D() = default;
    VboWrapper3D(const size_t num_vertices, const float* const vertex_data, const float* const color_data);

    void render() const;

};

#endif
