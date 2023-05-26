#ifndef DEMOS_TESTS_JOINTS_JOINTS_H_
#define DEMOS_TESTS_JOINTS_JOINTS_H_

#include <Box2D/Box2D.h>

#include <array>

#include "debug_value_reader.h"
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

inline properties::Transform getTransformExt(const ImageRGBAConstView<uint8_t>& img,
                                             const Vec3d& size,
                                             const double phi,
                                             const double x_offset,
                                             const double y_offset)
{
    const double image_width = img.width();
    const double image_height = img.height();

    const double max_dim_size = std::max(size.x, size.y);
    const double max_dim_img = std::max(img.width(), img.height());
    const Vec3d scale_vec{max_dim_size / max_dim_img, max_dim_size / max_dim_img, 1.0};

    const auto scale_mat = diagMatrix<double>(scale_vec);
    const Vec3d center_of_rotation = Vec3d{image_width / 2.0, image_height / 2.0, 0.0}.elementWiseMultiply(scale_vec);

    const Matrix<double> rot_mat = rotationMatrixZ<double>(phi);
    const Vec3<double> translation_vec =
        -rotationMatrixZ<double>(-phi) * center_of_rotation + Vec3d{x_offset, y_offset, 0.0};

    return properties::Transform{scale_mat, rot_mat, translation_vec};
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
    Shape(b2Body* body_handle_,
          const float width_,
          const float height_,
          const internal::ItemId& id_,
          const std::uint8_t red,
          const std::uint8_t green,
          const std::uint8_t blue)
        : width{width_}, height{height_}, body_handle{body_handle_}, id{id_}
    {
        b2Vec2 pos = body_handle_->GetPosition();
        x_pos = pos.x;
        y_pos = pos.y;

        angle = body_handle_->GetAngle();
        shape_img = createRectangle(width * scale_factor, height * scale_factor, red, green, blue);
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

    properties::Transform getTransform() const
    {
        const properties::Transform transform = getTransformExt(shape_img.constView(),
                                                                {width, height, 1.0},
                                                                -body_handle->GetAngle(),
                                                                body_handle->GetPosition().x,
                                                                body_handle->GetPosition().y);
        return transform;
    }
};

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

    internal::ItemId id_counter_;

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

        const Shape shp(body, size.x, size.y, id_counter_, 0, 255, 0);
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
        fixture_def.density = debug_value_reader::readFloat("density");
        fixture_def.friction = debug_value_reader::readFloat("friction");

        body->CreateFixture(&fixture_def);

        const Shape shp(body, size.x, size.y, id_counter_, 255, 0, 0);
        id_counter_ = static_cast<internal::ItemId>(static_cast<int>(id_counter_) + 1);
        return shp;
    }

    static constexpr dvs::internal::ItemId kLaunchSquareId = properties::ID0;
    static constexpr dvs::internal::ItemId kSpinningSquareId = properties::ID1;

    float t = 0.0f;
    std::vector<Shape> shapes_;
    Shape funnel_left_;
    Shape funnel_right_;
    Shape pusher_;
    Shape flipper_;

    // Pusher
    float pusher_p;
    float pusher_d;

    float pusher_frequency;
    float pusher_amplitude;
    float pusher_x;
    float pusher_y;

    // Flipper
    float flipper_p;
    float flipper_d;

    float flipper_frequency;
    float flipper_amplitude;
    float flipper_offset;

    static constexpr size_t kNumShapes{20U};

public:
    Joints() : gravity_{0.0f, -10.0f}, world_{gravity_}, ground_body_{nullptr}, ground_box_{}
    {
        id_counter_ = static_cast<internal::ItemId>(0);

        b2Body* wall_body = createStaticCollisionlessBody({0.0, 0.0}, {50.0, 50.0});

        createStaticBody({0.0, -10.0}, {6.0, 1.0}, 0.0);

        funnel_left_ = createStaticBoxShape(Vec2d{-4.0, -1.0}, Vec2d{12.0, 1.0}, (120.0) * M_PI / 180.0);
        funnel_right_ = createStaticBoxShape(Vec2d{4.0, -1.0}, Vec2d{12.0, 1.0}, (-120.0) * M_PI / 180.0);

        pusher_x = debug_value_reader::readFloat("pusher_x");
        pusher_y = debug_value_reader::readFloat("pusher_y");
        const double pusher_width = debug_value_reader::readDouble("pusher_width");
        const double pusher_height = debug_value_reader::readDouble("pusher_height");

        pusher_frequency = debug_value_reader::readFloat("pusher_frequency");
        pusher_amplitude = debug_value_reader::readFloat("pusher_amp");
        pusher_p = debug_value_reader::readFloat("pusher_p");
        pusher_d = debug_value_reader::readFloat("pusher_d");
        // float pusher_p;
        // float pusher_d;

        pusher_ = createDynamicBoxShape(Vec2d{pusher_x, pusher_y}, Vec2d{pusher_width, pusher_height}, 0.0);

        flipper_p = debug_value_reader::readFloat("flipper_p");
        flipper_d = debug_value_reader::readFloat("flipper_d");

        float x_pos = -6.0f;
        float y_pos = 6.0f;

        for (size_t k = 0; k < kNumShapes; k++)
        {
            // Between -6 and 6
            x_pos += 1.2;
            if (x_pos > 6.0)
            {
                x_pos = -6.0;
                y_pos += 1.2;
            }

            const float rot = static_cast<float>(rand() & 1001) / 1000.0f - 0.5f;
            shapes_.emplace_back(createDynamicBoxShape(Vec2d{x_pos, y_pos}, Vec2d{1.0, 1.0}, rot));
        }

        flipper_offset = debug_value_reader::readDouble("flipper_offset");
        flipper_ = createDynamicBoxShape(
            Vec2d{debug_value_reader::readDouble("flipper_x"), debug_value_reader::readDouble("flipper_y")},
            Vec2d{debug_value_reader::readDouble("flipper_width"), debug_value_reader::readDouble("flipper_height")},
            flipper_offset);

        flipper_frequency = debug_value_reader::readDouble("flipper_frequency");
        flipper_amplitude = debug_value_reader::readDouble("flipper_amplitude");

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

        b2RevoluteJointDef joint_def0;
        joint_def0.bodyA = wall_body;
        joint_def0.bodyB = flipper_.body_handle;
        joint_def0.localAnchorA =
            b2Vec2{debug_value_reader::readFloat("flipper_x"), debug_value_reader::readFloat("flipper_y")};
        joint_def0.localAnchorB = b2Vec2{0.0, 0.0};
        joint_def0.enableLimit = false;

        b2Joint* joint0 = world_.CreateJoint(&joint_def0);
    }

    ~Joints() {}

    void step()
    {
        world_.Step(timeStep, velocityIterations, positionIterations);
        std::vector<PropertySet> props;

        for (size_t k = 0; k < kNumShapes; k++)
        {
            const properties::Transform transform = shapes_[k].getTransform();
            props.emplace_back(shapes_[k].id, transform);
        }

        {
            const properties::Transform transform = funnel_left_.getTransform();
            props.emplace_back(funnel_left_.id, transform);
        }

        {
            const properties::Transform transform = funnel_right_.getTransform();
            props.emplace_back(funnel_right_.id, transform);
        }

        {
            const properties::Transform transform = pusher_.getTransform();
            props.emplace_back(pusher_.id, transform);
        }

        {
            const properties::Transform transform = flipper_.getTransform();
            props.emplace_back(flipper_.id, transform);
        }

        setProperties(props);

        const double phi_left = std::cos(t * 2.0) * 0.08f;
        const double phi_right = std::sin(t * 2.0) * 0.08f;
        funnel_left_.body_handle->SetTransform(funnel_left_.body_handle->GetPosition(),
                                               (120.0) * M_PI / 180.0 + phi_left);
        funnel_right_.body_handle->SetTransform(funnel_right_.body_handle->GetPosition(),
                                                (-120.0) * M_PI / 180.0 + phi_right);

        // Pusher
        const float pusher_reference_pos = std::sin(t * pusher_frequency) * pusher_amplitude + pusher_x;
        const float pusher_pos_error = pusher_reference_pos - pusher_.body_handle->GetPosition().x;
        const float pusher_vel = pusher_.body_handle->GetLinearVelocity().x;

        const float u_pusher = pusher_pos_error * pusher_p + pusher_vel * pusher_d;

        pusher_.body_handle->ApplyForce(b2Vec2(u_pusher, 0.0f), pusher_.body_handle->GetPosition(), true);

        // Flipper
        const double flipper_reference_angle = std::cos(t * flipper_frequency) * flipper_amplitude + flipper_offset;

        const float flipper_ang_error = flipper_reference_angle - flipper_.body_handle->GetAngle();
        const float flipper_ang_vel = flipper_.body_handle->GetAngularVelocity();
        const float u = flipper_ang_error * flipper_p + flipper_ang_vel * flipper_d;

        flipper_.body_handle->ApplyTorque(u, true);

        Vector<float> x0(2), y0(2);
        Vector<float> x1(2), y1(2);
        x0(0) = debug_value_reader::readDouble("flipper_x");
        y0(0) = debug_value_reader::readDouble("flipper_y");

        x0(1) = x0(0) + 8.0 * std::cos(flipper_reference_angle);
        y0(1) = y0(0) + 8.0 * std::sin(flipper_reference_angle);

        plot(x0, y0, properties::Color::RED, properties::ID250);

        x1(0) = pusher_reference_pos;
        y1(0) = -16.0;

        x1(1) = pusher_reference_pos;
        y1(1) = -8.0;

        plot(x1, y1, properties::Color::RED, properties::ID249);

        t += 0.05f;
    }
};

void testBasic();

}  // namespace joints

#endif  // DEMOS_TESTS_JOINTS_JOINTS_H_