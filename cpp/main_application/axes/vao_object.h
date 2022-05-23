#ifndef DVS_VAO_OBJECT_H_
#define DVS_VAO_OBJECT_H_

class VAOObject
{
private:
    size_t num_vertices_;
    size_t num_bytes_;
    GLuint vertex_buffer_, vertex_buffer_array_, color_buffer_;
    GLuint render_type_;

public:
    VAOObject() = default;
    VAOObject(const size_t num_vertices, const float* const vertices, const float* const colors, const GLuint render_type) :
        num_vertices_{num_vertices},
        num_bytes_{sizeof(float) * 3 * num_vertices_},
        render_type_{render_type}
    {
        glGenVertexArrays(1, &vertex_buffer_array_);
        glBindVertexArray(vertex_buffer_array_);

        glGenBuffers(1, &vertex_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glBufferData(GL_ARRAY_BUFFER, num_bytes_, vertices, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &color_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glBufferData(GL_ARRAY_BUFFER, num_bytes_, colors, GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
    }
    
    void renderAndUpdateData(const float* const new_vertices, const size_t num_bytes_to_update)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_to_update, new_vertices);

        glBindVertexArray(vertex_buffer_array_);
        glDrawArrays(render_type_, 0, num_vertices_);
        glBindVertexArray(0);
    }

    void render() const
    {
        glBindVertexArray(vertex_buffer_array_);
        glDrawArrays(render_type_, 0, num_vertices_);
        glBindVertexArray(0);
    }
};

#endif
