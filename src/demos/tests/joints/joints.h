#ifndef DEMOS_TESTS_JOINTS_JOINTS_H_
#define DEMOS_TESTS_JOINTS_JOINTS_H_

#include <Box2D/Box2D.h>

#include <array>

#include "dvs/dvs.h"
// #include <box2d/b2_prismatic_joint.h>
// src/externals/box2d/include/box2d/b2_prismatic_joint.h

using namespace dvs;

namespace joints
{

inline ImageRGBA<std::uint8_t> createRectangle(
    const size_t width, const size_t height, const std::uint8_t red, const std::uint8_t green, const std::uint8_t blue)
{
    ImageRGBA<std::uint8_t> rectangle{height, width};

    rectangle.fill(red, green, blue, 255);
    const size_t black_border_width = 5;

    for (size_t r = 0; r < height; r++)
    {
        for (size_t c = 0; c < width; c++)
        {
            if ((r < black_border_width) || (r > (height - black_border_width)) || (c < black_border_width) ||
                (c > (width - black_border_width)))
            {
                rectangle(r, c, 0) = 0;
                rectangle(r, c, 1) = 0;
                rectangle(r, c, 2) = 0;
            }
        }
    }

    return rectangle;
}

inline ImageRGBA<std::uint8_t> createCircle(const size_t radius,
                                            const std::uint8_t red,
                                            const std::uint8_t green,
                                            const std::uint8_t blue)
{
    assert((radius % 2 == 0));
    ImageRGBA<std::uint8_t> circle{radius * 2, radius * 2};

    const size_t num_rows = circle.height();
    const size_t num_cols = circle.width();

    circle.fill(0, 0, 0, 255);

    const double edge_threshold_pixels = 3.0;
    const double blend_threshold_pixels = 1.0;

    const double input_radius = static_cast<double>(radius);
    const double inner_edge_radius = input_radius - edge_threshold_pixels;

    for (size_t r = 0; r < num_rows; r++)
    {
        for (size_t c = 0; c < num_cols; c++)
        {
            const double x = static_cast<double>(c) - static_cast<double>(num_cols) / 2.0;
            const double y = static_cast<double>(r) - static_cast<double>(num_rows) / 2.0;

            const double current_radius = std::sqrt(x * x + y * y);

            if (current_radius < input_radius)
            {
                if (current_radius > (input_radius - edge_threshold_pixels))
                {
                    // In black silhouette
                    circle(r, c, 0) = 0;
                    circle(r, c, 1) = 0;
                    circle(r, c, 2) = 0;
                    const double dist_from_edge = input_radius - current_radius;

                    if (dist_from_edge < blend_threshold_pixels)
                    {
                        const double alpha = dist_from_edge / blend_threshold_pixels;
                        circle(r, c, 3) = static_cast<std::uint8_t>(255.0 * alpha);
                    }
                    else
                    {
                        circle(r, c, 3) = 255;
                    }
                }
                else
                {
                    circle(r, c, 0) = red;
                    circle(r, c, 1) = green;
                    circle(r, c, 2) = blue;
                    circle(r, c, 3) = 255;
                }
            }
            else
            {
                circle(r, c, 3) = 0;
            }

            // Make transition between color and silhouette
            const double dist_from_edge = inner_edge_radius - current_radius;
            if (std::abs(dist_from_edge) < blend_threshold_pixels)
            {
                const double blend_factor = (dist_from_edge + blend_threshold_pixels) / (2.0 * blend_threshold_pixels);

                circle(r, c, 0) = static_cast<std::uint8_t>(blend_factor * static_cast<double>(red));
                circle(r, c, 1) = static_cast<std::uint8_t>(blend_factor * static_cast<double>(green));
                circle(r, c, 2) = static_cast<std::uint8_t>(blend_factor * static_cast<double>(blue));
            }
            // if(input_radius - current_radius)
        }
    }

    const size_t offset_r = 40U;
    const size_t offset_c = 40U;

    for (size_t r = 0; r < 10; r++)
    {
        for (size_t c = 0; c < 10; c++)
        {
            const double x = static_cast<double>(c) - 5.0;
            const double y = static_cast<double>(r) - 5.0;
            const double current_radius = std::sqrt(x * x + y * y);
            if (current_radius < 5.0)
            {
                circle(r + offset_r, c + offset_c, 0) = 0;
                circle(r + offset_r, c + offset_c, 1) = 0;
                circle(r + offset_r, c + offset_c, 2) = 0;
            }
        }
    }

    return circle;
}

struct Shape
{
    float x_pos;
    float y_pos;
    float angle;
    float width;
    float height;

    ImageRGBA<std::uint8_t> shape_img;

    b2Body* body_handle = nullptr;
    internal::ItemId id;

    float scale_factor = 100.0f;

    Shape() = default;
    Shape(b2Body* body_handle_, const float width_, const float height_, const internal::ItemId& id_)
        : width{width_}, height{height_}, body_handle{body_handle_}, id{id_}
    {
        b2Vec2 pos = body_handle_->GetPosition();
        x_pos = pos.x;
        y_pos = pos.y;

        angle = body_handle_->GetAngle();
        shape_img = createRectangle(width * scale_factor, height * scale_factor, 255, 0, 0);
        imShow(shape_img, id);
    }

    float getXScale() const
    {
        return width / 2.0;
    }

    float getYScale() const
    {
        return height / 2.0;
    }
};

inline properties::Transform getTransform(const ImageRGBAConstView<uint8_t>& img,
                                          const Vec3d& scale_vec,
                                          const double phi,
                                          const double x_offset,
                                          const double y_offset)
{
    const double image_width = img.width();
    const double image_height = img.height();

    const auto scale_mat = diagMatrix<double>(scale_vec);
    const Vec3d center_of_rotation = Vec3d{image_width / 2.0, image_height / 2.0, 0.0}.elementWiseMultiply(scale_vec);

    const Matrix<double> rot_mat = rotationMatrixZ<double>(phi);
    const Vec3<double> translation_vec =
        -rotationMatrixZ<double>(-phi) * center_of_rotation + Vec3d{x_offset, y_offset, 0.0};

    return properties::Transform{scale_mat, rot_mat, translation_vec};
}

inline void readShapeImage(const std::string bin_path, ImageRGBA<std::uint8_t>& output_img)
{
    std::ifstream input(bin_path, std::ios::binary);

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    std::uint16_t num_rows, num_cols;
    buffer.data();

    std::memcpy(&num_cols, buffer.data(), sizeof(std::uint16_t));
    std::memcpy(&num_rows, buffer.data() + sizeof(std::uint16_t), sizeof(std::uint16_t));

    const std::uint8_t* const img_raw_ptr = buffer.data() + 2 * sizeof(std::uint16_t);

    output_img.resize(num_rows, num_cols);

    const size_t num_element_per_channel = num_rows * num_cols;

    for (size_t ch = 0; ch < 3; ch++)
    {
        for (size_t r = 0; r < num_rows; r++)
        {
            for (size_t c = 0; c < num_cols; c++)
            {
                const std::uint8_t pixel_val = img_raw_ptr[ch * num_element_per_channel + r * num_cols + c];

                size_t mapped_ch = 0;
                switch (ch)
                {
                    case 0:
                        mapped_ch = 2;
                        break;
                    case 1:
                        mapped_ch = 1;
                        break;
                    case 2:
                        mapped_ch = 0;
                        break;
                    default:
                        mapped_ch = 0;
                }
                output_img(r, c, mapped_ch) = pixel_val;
            }
        }
    }

    for (size_t r = 0; r < num_rows; r++)
    {
        for (size_t c = 0; c < num_cols; c++)
        {
            // If white, set alpha to 0
            const bool cond =
                (output_img(r, c, 0) == 255) && (output_img(r, c, 1) == 255) && (output_img(r, c, 2) == 255);
            if (cond)
            {
                output_img(r, c, 3) = 0;
            }
            else
            {
                output_img(r, c, 3) = 255;
            }
        }
    }
}

class Joints
{
public:
    static constexpr std::size_t kNumSquares{10U};

private:
    b2Vec2 gravity_{};
    b2World world_;
    b2Body* ground_body_{};
    b2PolygonShape ground_box_{};
    b2Body* dynamic_body_;
    b2Body* spinner_body_;

    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    ImageRGBA<uint8_t> circle_;
    ImageRGBA<uint8_t> damper_;
    ImageRGBA<uint8_t> pentagon_;
    ImageRGBA<uint8_t> rounded_square_;
    ImageRGBA<uint8_t> spring_;
    ImageRGBA<uint8_t> square_;
    ImageRGBA<uint8_t> square_borderless_;

    std::array<Shape, kNumSquares> squares_;

    internal::ItemId id_counter_;
    Shape shape_;
    std::vector<Shape> shapes_;
    Shape funnel_left_;

    static constexpr size_t kNumShapes{10U};

    b2Body* createStaticCollisionlessBody(const Vec2d& position, const Vec2d& size)
    {
        b2BodyDef body_def;
        body_def.type = b2_staticBody;
        body_def.position.Set(position.x, position.y);

        b2Body* body = world_.CreateBody(&body_def);

        return body;
    }

    b2Body* createStaticBody(const Vec2d& position, const Vec2d& size, const double angle)
    {
        b2BodyDef body_def;
        body_def.type = b2_staticBody;
        body_def.position.Set(position.x, position.y);
        body_def.angle = angle;

        b2PolygonShape body_shape{};
        body_shape.SetAsBox(size.x, size.y);

        b2Body* body = world_.CreateBody(&body_def);

        body->CreateFixture(&body_shape, 0.0f);

        return body;
    }

    Shape createStaticBoxShape(const Vec2d& position, const Vec2d& size, const double angle)
    {
        b2BodyDef body_def;
        body_def.type = b2_staticBody;
        body_def.position.Set(position.x, position.y);
        body_def.angle = angle;

        b2PolygonShape body_shape{};
        body_shape.SetAsBox(size.x / 2.0, size.y / 2.0);

        b2Body* body = world_.CreateBody(&body_def);

        body->CreateFixture(&body_shape, 0.0f);

        const Shape shp(body, size.x, size.y, id_counter_);
        id_counter_ = static_cast<internal::ItemId>(static_cast<int>(id_counter_) + 1);
        return shp;
    }

    Shape createDynamicBoxShape(const Vec2d& position, const Vec2d& size, const double angle)
    {
        // Creating the dynamic body
        b2BodyDef body_def;
        body_def.type = b2_dynamicBody;
        body_def.position.Set(position.x, position.y);
        body_def.angle = angle;

        b2Body* body = world_.CreateBody(&body_def);

        b2PolygonShape dynamic_box;
        dynamic_box.SetAsBox(size.x / 2.0, size.y / 2.0);

        b2FixtureDef fixture_def;
        fixture_def.shape = &dynamic_box;
        fixture_def.density = 1.0f;
        fixture_def.friction = 0.0f;

        body->CreateFixture(&fixture_def);

        const Shape shp(body, size.x, size.y, id_counter_);
        id_counter_ = static_cast<internal::ItemId>(static_cast<int>(id_counter_) + 1);
        return shp;
    }

    b2Body* createDynamicBoxBody(const Vec2d& position, const Vec2d& size, const double angle)
    {
        // Creating the dynamic body
        b2BodyDef body_def;
        body_def.type = b2_dynamicBody;
        body_def.position.Set(0.0f, 4.0f);
        body_def.angle = 0.1;

        b2Body* body = world_.CreateBody(&body_def);

        b2PolygonShape dynamic_box;
        dynamic_box.SetAsBox(1.0f, 1.0f);

        b2FixtureDef fixture_def;
        fixture_def.shape = &dynamic_box;
        fixture_def.density = 1.0f;
        fixture_def.friction = 0.0f;

        body->CreateFixture(&fixture_def);

        return body;
    }

    static constexpr dvs::internal::ItemId kLaunchSquareId = properties::ID0;
    static constexpr dvs::internal::ItemId kSpinningSquareId = properties::ID1;

    void loadImages()
    {
        const std::string bin_path = "../demos/tests/joints/images/";

        readShapeImage(bin_path + "circle.bin", circle_);
        readShapeImage(bin_path + "damper.bin", damper_);
        readShapeImage(bin_path + "pentagon.bin", pentagon_);
        readShapeImage(bin_path + "rounded_square.bin", rounded_square_);
        readShapeImage(bin_path + "spring.bin", spring_);
        readShapeImage(bin_path + "square.bin", square_);

        square_.remapChannels({2, 1, 0});

        square_borderless_ = square_;
        square_borderless_.fill(square_(40, 40, 0), square_(40, 40, 1), square_(40, 40, 2), 255);
    }

public:
    Joints() : gravity_{0.0f, -10.0f}, world_{gravity_}, ground_body_{nullptr}, ground_box_{}
    {
        loadImages();
        id_counter_ = static_cast<internal::ItemId>(0);

        // b2Body* wall_body = createStaticCollisionlessBody({0.0, -10.0}, {50.0, 50.0});

        ground_body_ = createStaticBody({0.0, -10.0}, {50.0, 1.0}, -0.1);
        ground_body_ = createStaticBody({0.0, -10.0}, {50.0, 1.0}, 0.1);

        funnel_left_ = createStaticBoxShape(Vec2d{0.0, -1.0}, Vec2d{8.0, 1.0}, -0.2f);

        // spinner_body_ = createStaticBody({8.0, 0.0}, {1.0, 20.0}, 0.0);
        // dynamic_body_ = createDynamicBoxBody({0.0, 4.0}, {1.0, 1.0}, 0.0);

        // shape_ = createDynamicBoxShape(Vec2d{1.0, 4.0}, Vec2d{1.0, 1.0}, 0.0f);
        // createStaticBoxShape

        for (size_t k = 0; k < kNumShapes; k++)
        {
            const float y_pos = k < kNumShapes / 2 ? 2.0f : 6.0f;
            const float x_pos = k < kNumShapes / 2 ? k * 3 : (k - kNumShapes / 2) * 3;
            const float rot = static_cast<float>(rand() & 1001) / 1000.0f - 0.5f;
            shapes_.emplace_back(createDynamicBoxShape(Vec2d{x_pos, y_pos}, Vec2d{1.0, 1.0}, rot));
            // imShow(shapes_[k].shape_img, shapes_[k].id);
        }

        // imShow(funnel_left_.shape_img, funnel_left_.id);

        // Spring joint
        /*b2DistanceJointDef joint_def0, joint_def1;

        joint_def0.Initialize(wall_body, dynamic_body_, b2Vec2{-1.0, 1.0}, b2Vec2{-4.0, 4.0});
        joint_def0.length = 2.0f;

        joint_def0.collideConnected = false;
        joint_def0.frequencyHz = 10.0f;
        joint_def0.dampingRatio = 0.1f;

        joint_def1.Initialize(wall_body, dynamic_body_, b2Vec2{1.0, 1.0}, b2Vec2{4.0, 4.0});
        joint_def1.length = 2.0f;

        joint_def1.collideConnected = false;
        joint_def1.frequencyHz = 10.0f;
        joint_def1.dampingRatio = 0.1f;

        b2Joint* joint0 = world_.CreateJoint(&joint_def0);
        b2Joint* joint1 = world_.CreateJoint(&joint_def1);

        // Prism joint
        b2PrismaticJointDef joint_def2;
        joint_def2.Initialize(wall_body, dynamic_body_, b2Vec2{0.0, 0.0}, b2Vec2{0.0, 1.0});
        b2Joint* joint2 = world_.CreateJoint(&joint_def2);*/

        // imShow(square_borderless_, kSpinningSquareId);
    }

    ~Joints() {}

    void step()
    {
        // const Vec3d scale_vec1{
        //     2.0 / static_cast<double>(square_.width()), 20.0 * 2.0 / static_cast<double>(square_.width()), 1.0};

        world_.Step(timeStep, velocityIterations, positionIterations);
        // spinner_body_->SetTransform(spinner_body_->GetPosition(), spinner_body_->GetAngle() + 0.01f);

        for (size_t k = 0; k < kNumShapes; k++)
        {
            const Vec3d scale_vec{1.0 / static_cast<double>(shapes_[k].shape_img.width()),
                                  1.0 / static_cast<double>(shapes_[k].shape_img.width()),
                                  1.0};

            const properties::Transform transform = getTransform(shapes_[k].shape_img.constView(),
                                                                 scale_vec,
                                                                 -shapes_[k].body_handle->GetAngle(),
                                                                 shapes_[k].body_handle->GetPosition().x,
                                                                 shapes_[k].body_handle->GetPosition().y);
            setProperties(shapes_[k].id, transform);
        }

        {
            const Vec3d scale_vec{8.0 / static_cast<double>(funnel_left_.shape_img.width()),
                                  8.0 / static_cast<double>(funnel_left_.shape_img.width()),
                                  1.0};
            const properties::Transform transform = getTransform(funnel_left_.shape_img.constView(),
                                                                 scale_vec,
                                                                 -funnel_left_.body_handle->GetAngle(),
                                                                 funnel_left_.body_handle->GetPosition().x,
                                                                 funnel_left_.body_handle->GetPosition().y);
            setProperties(funnel_left_.id, transform);
        }

        // funnel_left_.body_handle->SetTransform(funnel_left_.body_handle->GetPosition(),
        //                                        funnel_left_.body_handle->GetAngle() + 0.01f);

        /*const properties::Transform transform1 = getTransform(square_.constView(),
                                                              scale_vec1,
                                                              -spinner_body_->GetAngle(),
                                                              spinner_body_->GetPosition().x,
                                                              spinner_body_->GetPosition().y);
        setProperties(kSpinningSquareId, transform1);*/
    }
};

void testBasic();

}  // namespace joints

#endif  // DEMOS_TESTS_JOINTS_JOINTS_H_