#include "main_application/plot_objects/surf/surf.h"

Surf::Surf(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::SURF)
    {
        throw std::runtime_error("Invalid function type for Surf!");
    }

    dims_ = hdr.get(FunctionHeaderObjectType::DIMENSION_2D).as<internal::Dimension2D>();

    points_ptr_ = convertMatrixDataOuter(data_ptr_, data_type_, dims_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 * (dims_.rows - 1) * (dims_.cols - 1) * 2, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Surf::findMinMax()
{
    std::tie<Vec3Dd, Vec3Dd>(min_vec, max_vec) =
        findMinMaxFromThreeMatrices(data_ptr_, dims_.rows, dims_.cols, num_bytes_for_one_vec_, data_type_);
}

void Surf::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, 4 * (dims_.rows - 1) * (dims_.cols - 1) * 2);
    glBindVertexArray(0);
}

Surf::~Surf()
{
    delete[] points_ptr_;
}
