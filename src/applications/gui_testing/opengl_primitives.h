#ifndef DVS_OPENGL_PRIMITIVES_H
#define DVS_OPENGL_PRIMITIVES_H

#include <stddef.h>
#include <stdint.h>

class BufferedVector
{
public:
    BufferedVector() = delete;
    BufferedVector(size_t size) : data_(new float[size]), size_(size), idx_(0) {}
    ~BufferedVector()
    {
        delete[] data_;
    }

    float* data()
    {
        return data_;
    }

    size_t size()
    {
        return size_;
    }

    size_t idx()
    {
        return idx_;
    }

    void reset()
    {
        idx_ = 0;
    }

    void incrementIdx(size_t increment)
    {
        idx_ += increment;
    }

private:
    float* data_;
    size_t size_;
    size_t idx_;
};

void PutCircleSegmentIntoBuffer(BufferedVector& buffered_vector,
                                const float x_center,
                                const float y_center,
                                const float radius,
                                const float theta_start,
                                const float theta_end,
                                const size_t num_triangles);

void PutCircularCurveSegmentIntoBuffer(BufferedVector& buffered_vector,
                                       const float x_center,
                                       const float y_center,
                                       const float inner_radius,
                                       const float outer_radius,
                                       const float theta_start,
                                       const float theta_end,
                                       const size_t num_triangles);

void PutCircleIntoBuffer(BufferedVector& buffered_vector,
                         const float x_center,
                         const float y_center,
                         const float radius,
                         const size_t num_triangles);
void PutRectangleIntoBuffer(BufferedVector& buffered_vector,
                            const float x_top_left,
                            const float y_top_left,
                            const float width,
                            const float height);

#endif  // DVS_OPENGL_PRIMITIVES_H
