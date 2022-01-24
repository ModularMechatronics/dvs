#include "main_application/plot_objects/scatter/scatter.h"

uint8_t* convertData2DOuter(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec);

Scatter2D::Scatter2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::SCATTER2)
    {
        throw std::runtime_error("Invalid function type for Scatter2D!");
    }

    points_ptr_ =
        convertData2DOuter(data_ptr_, data_type_, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 2, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Scatter2D::modifyShader(const GLuint program_id_)
{
    PlotObjectBase::modifyShader(program_id_);
    glUniform1f(glGetUniformLocation(program_id_, "point_size"), point_size_);
}

void Scatter2D::findMinMax()
{
    Vec2Dd min_vec_2d, max_vec_2d;
    std::tie<Vec2Dd, Vec2Dd>(min_vec_2d, max_vec_2d) =
        findMinMaxFromTwoVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);

    min_vec.x = min_vec_2d.x;
    min_vec.y = min_vec_2d.y;
    min_vec.z = -1.0;

    max_vec.x = max_vec_2d.x;
    max_vec.y = max_vec_2d.y;
    max_vec.z = 1.0;
}

void Scatter2D::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_POINTS, 0, num_elements_);
    glBindVertexArray(0);
}

Scatter2D::~Scatter2D()
{
    delete[] points_ptr_;
}