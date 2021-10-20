#ifndef PLOT2_COLLECTION_H_
#define PLOT2_COLLECTION_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "plot_objects/plot_object_base.h"

class Plot2Collection : public PlotObjectBase
{
private:
    float* points_ptr_;

    GLuint buffer_handle_;
    uint32_t num_points_;
    uint32_t num_objects_;

    void findMinMax() override;

public:
    Plot2Collection();
    Plot2Collection(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Plot2Collection();

    void visualize() override;
};

template <typename T>
inline float* convertData(uint8_t* input_data,
                          const size_t num_objects,
                          const size_t num_bytes_per_element,
                          const size_t num_points,
                          const Vector<uint8_t>& vector_lengths)
{
    const size_t total_num_bytes = num_points * 2 * num_bytes_per_element;
    const size_t num_bytes_per_collection = vector_lengths.sum() * num_bytes_per_element;

    T* data_x = reinterpret_cast<T*>(input_data);
    T* data_y = reinterpret_cast<T*>(input_data + num_bytes_per_collection);

    float* output_data = new float[total_num_bytes];

    size_t idx_offset = 0;
    size_t idx = 0;

    for (size_t i = 0; i < num_objects; i++)
    {
        for (size_t k = 0; k < vector_lengths(i) - 1; k++)
        {
            output_data[idx] = data_x[idx_offset + k];
            output_data[idx + 1] = data_y[idx_offset + k];

            output_data[idx + 2] = data_x[idx_offset + k + 1];
            output_data[idx + 3] = data_y[idx_offset + k + 1];

            idx += 4;
        }
        idx_offset += vector_lengths(i);
    }

    return output_data;
}

Plot2Collection::Plot2Collection(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::PLOT2_COLLECTION)
    {
        throw std::runtime_error("Invalid function type for Plot2Collection!");
    }

    num_points_ = 0;
    num_objects_ = hdr.get(FunctionHeaderObjectType::NUM_OBJECTS).as<uint32_t>();

    Vector<uint8_t> vector_lengths(num_objects_);  // TODO: vector_lengths has to be larger than uint8_t

    for (size_t k = 0; k < num_objects_; k++)
    {
        vector_lengths(k) = data_ptr_[k];
    }

    for (size_t k = 0; k < num_objects_; k++)
    {
        num_points_ += (vector_lengths(k) - 1) * 2;
    }

    // Advance pointer to account for first bytes where 'vector_lengths' are stored
    data_ptr_ += num_objects_ * sizeof(uint8_t);

    points_ptr_ = convertData<double>(data_ptr_, num_objects_, num_bytes_per_element_, num_points_, vector_lengths);
}

void Plot2Collection::findMinMax()
{
    min_vec = {-1.0, -1.0, -1.0};
    max_vec = {1.0, 1.0, 1.0};
    /*min_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    max_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    size_t idx = 0;
    for (size_t k = 0; k < (num_indices_ * 3 * 3); k += 3)
    {
        const Point3Dd current_point(points_ptr_[k], points_ptr_[k + 1], points_ptr_[k + 2]);
        min_vec.x = std::min(current_point.x, min_vec.x);
        min_vec.y = std::min(current_point.y, min_vec.y);
        min_vec.z = std::min(current_point.z, min_vec.z);

        max_vec.x = std::max(current_point.x, max_vec.x);
        max_vec.y = std::max(current_point.y, max_vec.y);
        max_vec.z = std::max(current_point.z, max_vec.z);

        idx += 3;
    }*/
}

void Plot2Collection::visualize()
{
    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_handle_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_handle_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points_ * 2, points_ptr_, GL_STATIC_DRAW);
    }
    setColor(color_);
    setLinewidth(line_width_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_handle_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINES, 0, num_points_);
    glDisableVertexAttribArray(0);
}

Plot2Collection::~Plot2Collection() {}

#endif
