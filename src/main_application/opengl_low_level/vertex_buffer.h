#ifndef MAIN_APPLICATION_OPENGL_LOW_LEVEL_VERTEX_BUFFER_H_
#define MAIN_APPLICATION_OPENGL_LOW_LEVEL_VERTEX_BUFFER_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/opengl_low_level.h"

enum OGLPrimitiveType : uint64_t
{
    POINTS = GL_POINTS,
    LINES = GL_LINES,
    LINE_LOOP = GL_LINE_LOOP,
    LINE_STRIP = GL_LINE_STRIP,
    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLE_FAN = GL_TRIANGLE_FAN,
    QUADS = GL_QUADS
};

class VertexBuffer
{
private:
    GLuint vertex_buffer_array_;
    std::vector<GLuint> vertex_buffers_;

    OGLPrimitiveType primitive_type_;

public:
    VertexBuffer(const OGLPrimitiveType primitive_type) : primitive_type_{primitive_type}
    {
        glGenVertexArrays(1, &vertex_buffer_array_);
        glBindVertexArray(vertex_buffer_array_);
    }

    ~VertexBuffer()
    {
        for (size_t k = 0; k < vertex_buffers_.size(); k++)
        {
            glDeleteBuffers(1, vertex_buffers_.data() + k);
        }

        glDeleteVertexArrays(1, &vertex_buffer_array_);
    }

    template <typename T> void addBuffer(const T* const data, const size_t num_elements, const uint8_t num_dimensions)
    {
        // TODO: Remove this function once they all use the usage-based one?
        addBuffer(data, num_elements, num_dimensions, GL_STATIC_DRAW);
    }

    template <typename T>
    void addBuffer(const T* const data, const size_t num_elements, const uint8_t num_dimensions, const GLenum usage)
    {
        glBindVertexArray(vertex_buffer_array_);
        static_assert(std::is_same<T, float>::value || std::is_same<T, int>::value || std::is_same<T, int32_t>::value,
                      "Only float and int supported for now!");
        vertex_buffers_.push_back(1);
        const int idx = static_cast<int>(vertex_buffers_.size()) - 1;

        glGenBuffers(1, vertex_buffers_.data() + idx);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers_[idx]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * num_elements * num_dimensions, data, usage);

        glEnableVertexAttribArray(idx);
        if (std::is_same<T, float>::value)
        {
            glVertexAttribPointer(idx, num_dimensions, GL_FLOAT, GL_FALSE, 0, 0);
        }
        else if (std::is_same<T, int>::value || std::is_same<T, int32_t>::value)
        {
            glVertexAttribIPointer(idx, num_dimensions, GL_INT, 0, 0);
        }
        else
        {
            throw std::runtime_error("Shouldn't end up here!");
        }
    }

    template <typename T>
    void updateBufferData(const size_t buffer_idx,
                          const T* const data,
                          const size_t num_elements,
                          const uint8_t num_dimensions)
    {
        static_assert(std::is_same<T, float>::value || std::is_same<T, int>::value || std::is_same<T, int32_t>::value,
                      "Only float and int supported for now!");

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers_[buffer_idx]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(T) * num_elements * num_dimensions, data);
    }

    void render(const size_t num_elements) const
    {
        glBindVertexArray(vertex_buffer_array_);
        glDrawArrays(primitive_type_, 0, num_elements);
        glBindVertexArray(0);
    }

    void render(const size_t num_elements, const OGLPrimitiveType primitive_type) const
    {
        glBindVertexArray(vertex_buffer_array_);
        glDrawArrays(primitive_type, 0, num_elements);
        glBindVertexArray(0);
    }

    void render(const size_t num_elements, const size_t offset, const OGLPrimitiveType primitive_type) const
    {
        glBindVertexArray(vertex_buffer_array_);
        glDrawArrays(primitive_type, offset, num_elements);
        glBindVertexArray(0);
    }
};

#endif  // MAIN_APPLICATION_OPENGL_LOW_LEVEL_VERTEX_BUFFER_H_
