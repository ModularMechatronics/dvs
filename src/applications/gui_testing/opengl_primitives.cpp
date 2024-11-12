#include "opengl_primitives.h"

#include <cmath>

#include "duoplot/internal.h"
#include "duoplot/math/math.h"
#include "rgbtriplet.h"

using namespace duoplot;

void PutCircularCurveSegmentIntoBuffer(BufferedVector& buffered_vector,
                                       const float x_center,
                                       const float y_center,
                                       const float inner_radius,
                                       const float outer_radius,
                                       const float theta_start,
                                       const float theta_end,
                                       const size_t num_triangles)
{
    const float angle_inc = (theta_end - theta_start) / static_cast<float>(num_triangles);

    float theta = theta_start;

    const size_t start_idx = buffered_vector.idx();

    for (size_t i = 0; i < num_triangles; ++i)
    {
        size_t idx = start_idx + i * 12U;

        const float x_pt = outer_radius * std::cos(theta);
        const float y_pt = -outer_radius * std::sin(theta);

        const float x_pt_inner = x_pt * inner_radius / outer_radius;
        const float y_pt_inner = y_pt * inner_radius / outer_radius;

        theta += angle_inc;
        const float x_pt_next = outer_radius * std::cos(theta);
        const float y_pt_next = -outer_radius * std::sin(theta);

        const float x_pt_next_inner = x_pt_next * inner_radius / outer_radius;
        const float y_pt_next_inner = y_pt_next * inner_radius / outer_radius;

        // First triangle
        buffered_vector.data()[idx] = x_pt_inner + x_center;
        buffered_vector.data()[idx + 1U] = y_pt_inner + y_center;

        buffered_vector.data()[idx + 2U] = x_pt + x_center;
        buffered_vector.data()[idx + 3U] = y_pt + y_center;

        buffered_vector.data()[idx + 4U] = x_pt_next_inner + x_center;
        buffered_vector.data()[idx + 5U] = y_pt_next_inner + y_center;

        // Second triangle
        buffered_vector.data()[idx + 6U] = x_pt_next_inner + x_center;
        buffered_vector.data()[idx + 7U] = y_pt_next_inner + y_center;

        buffered_vector.data()[idx + 8U] = x_pt + x_center;
        buffered_vector.data()[idx + 9U] = y_pt + y_center;

        buffered_vector.data()[idx + 10U] = x_pt_next + x_center;
        buffered_vector.data()[idx + 11U] = y_pt_next + y_center;

        buffered_vector.incrementIdx(12U);
    }
}

void PutCircleIntoBuffer(BufferedVector& buffered_vector,
                         const float x_center,
                         const float y_center,
                         const float radius,
                         const size_t num_triangles)
{
    size_t start_idx = buffered_vector.idx();

    for (size_t i = 0; i < num_triangles; ++i)
    {
        const size_t idx = start_idx + i * 6;

        const float theta0 = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(num_triangles);
        const float theta1 = 2.0f * M_PI * static_cast<float>(i + 1) / static_cast<float>(num_triangles);

        const float x0 = radius * std::cos(theta0);
        const float y0 = radius * std::sin(theta0);

        const float x1 = radius * std::cos(theta1);
        const float y1 = radius * std::sin(theta1);

        // First outer point
        buffered_vector.data()[idx] = x_center + x0;
        buffered_vector.data()[idx + 1] = y_center + y0;

        // Center point
        buffered_vector.data()[idx + 2] = x_center;
        buffered_vector.data()[idx + 3] = y_center;

        // Second outer point
        buffered_vector.data()[idx + 4] = x_center + x1;
        buffered_vector.data()[idx + 5] = y_center + y1;

        buffered_vector.incrementIdx(6);
    }
}

void PutRectangleIntoBuffer(BufferedVector& buffered_vector,
                            const float x_top_left,
                            const float y_top_left,
                            const float width,
                            const float height)
{
    const size_t start_idx = buffered_vector.idx();

    const Vec2f bottom_left{x_top_left, y_top_left + height};
    const Vec2f bottom_right{x_top_left + width, y_top_left + height};

    const Vec2f top_left{x_top_left, y_top_left};
    const Vec2f top_right{x_top_left + width, y_top_left};

    using Triangle = std::array<Vec2f, 3>;

    const std::array<Triangle, 2> triangles{Triangle{top_left, top_right, bottom_left},
                                            Triangle{bottom_left, top_right, bottom_right}};

    size_t idx = start_idx;

    for (const auto& corner_points : triangles)
    {
        for (const auto& corner_point : corner_points)
        {
            buffered_vector.data()[idx] = corner_point.x;
            buffered_vector.data()[idx + 1U] = corner_point.y;
            buffered_vector.incrementIdx(2U);

            idx += 2U;
        }
    }
}

void PutCircleSegmentIntoBuffer(BufferedVector& buffered_vector,
                                const float x_center,
                                const float y_center,
                                const float radius,
                                const float theta_start,
                                const float theta_end,
                                const size_t num_triangles)
{
    const size_t start_idx = buffered_vector.idx();

    const float delta_angle = (theta_end - theta_start) / static_cast<float>(num_triangles);

    for (size_t k = 0; k < num_triangles; ++k)
    {
        const size_t idx = start_idx + k * 6;

        const float theta0 = theta_start + static_cast<float>(k) * delta_angle;
        const float theta1 = theta_start + static_cast<float>(k + 1) * delta_angle;

        const float x0 = radius * std::cos(theta0);
        const float y0 = -radius * std::sin(theta0);

        const float x1 = radius * std::cos(theta1);
        const float y1 = -radius * std::sin(theta1);

        // First outer point
        buffered_vector.data()[idx] = x_center + x0;
        buffered_vector.data()[idx + 1] = y_center + y0;

        // Center point
        buffered_vector.data()[idx + 2] = x_center;
        buffered_vector.data()[idx + 3] = y_center;

        // Second outer point
        buffered_vector.data()[idx + 4] = x_center + x1;
        buffered_vector.data()[idx + 5] = y_center + y1;

        buffered_vector.incrementIdx(6);
    }

    /*
    float theta = theta0;
        for (size_t k = 0; k < kNumTrianglesPerEdge; ++k)
        {
            line_buffer_.data()[idx] = x_center;
            line_buffer_.data()[idx + 1U] = y_center;

            const float x_pt = kEdgeRadius * std::cos(theta);
            const float y_pt = kEdgeRadius * std::sin(theta);

            line_buffer_.data()[idx + 2U] = x_center + x_pt;
            line_buffer_.data()[idx + 3U] = y_center + y_pt;

            theta += angle_inc;
            const float x_pt_next = kEdgeRadius * std::cos(theta);
            const float y_pt_next = kEdgeRadius * std::sin(theta);

            line_buffer_.data()[idx + 4U] = x_center + x_pt_next;
            line_buffer_.data()[idx + 5U] = y_center + y_pt_next;

            idx += 6U;
        }
    */
}