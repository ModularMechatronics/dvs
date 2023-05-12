#ifndef DEMOS_TESTS_JOINTS_JOINTS_H_
#define DEMOS_TESTS_JOINTS_JOINTS_H_

#include "dvs/dvs.h"

#include <Box2D/Box2D.h>
// #include <box2d/b2_prismatic_joint.h>
// src/externals/box2d/include/box2d/b2_prismatic_joint.h

using namespace dvs;

namespace joints
{

inline properties::Transform getTransform(const ImageRGBAConstView<uint8_t>& img, const Vec3d& scale_vec, const double phi, const double x_offset, const double y_offset)
{
    const double image_width = img.width();
    const double image_height = img.height();

    const auto scale_mat = diagMatrix<double>(scale_vec);
    const Vec3d center_of_rotation = Vec3d{image_width / 2.0, image_height / 2.0, 0.0}.elementWiseMultiply(scale_vec);

    const Matrix<double> rot_mat = rotationMatrixZ<double>(phi);
    const Vec3<double> translation_vec = -rotationMatrixZ<double>(-phi) * center_of_rotation + Vec3d{x_offset, y_offset, 0.0};

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
                switch(ch)
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
            const bool cond = (output_img(r, c, 0) == 255) &&
                (output_img(r, c, 1) == 255) &&
                (output_img(r, c, 2) == 255);
            if(cond)
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

public:
    Joints() : gravity_{0.0f, -10.0f}, world_{gravity_}, ground_body_{nullptr}, ground_box_{}
    {

        b2Body* wall_body = createStaticCollisionlessBody({0.0, -10.0}, {50.0, 50.0});

        // ground_body_ = createStaticBody({0.0, -10.0}, {50.0, 1.0}, -0.1);

        spinner_body_ = createStaticBody({8.0, 0.0}, {1.0, 20.0}, 0.0);
        dynamic_body_ = createDynamicBoxBody({0.0, 4.0}, {1.0, 1.0}, 0.0);

        // Spring joint
        b2DistanceJointDef joint_def0, joint_def1;

        joint_def0.Initialize(wall_body, dynamic_body_, b2Vec2{-1.0, 1.0}, b2Vec2{-4.0, 4.0});
        joint_def0.length = 2.0f;
 
        // joint_def0.bodyA = wall_body;
        // joint_def0.bodyB = dynamic_body_;
        joint_def0.collideConnected = false;
        joint_def0.frequencyHz = 10.0f;
        joint_def0.dampingRatio = 0.1f;

        joint_def1.Initialize(wall_body, dynamic_body_, b2Vec2{1.0, 1.0}, b2Vec2{4.0, 4.0});
        joint_def1.length = 2.0f;
 
        // joint_def1.bodyA = wall_body;
        // joint_def1.bodyB = dynamic_body_;
        joint_def1.collideConnected = false;
        joint_def1.frequencyHz = 10.0f;
        joint_def1.dampingRatio = 0.1f;

        b2Joint* joint0 = world_.CreateJoint(&joint_def0);
        b2Joint* joint1 = world_.CreateJoint(&joint_def1);

        // Prism joint
        b2PrismaticJointDef joint_def2;
        joint_def2.Initialize (wall_body, dynamic_body_, b2Vec2{0.0, 0.0}, b2Vec2{0.0, 1.0});
        b2Joint* joint2 = world_.CreateJoint(&joint_def2);
    }
    ~Joints(){}

    void step(const ImageRGBAConstView<uint8_t>& square_img_view)
    {
        const Vec3d scale_vec0{2.0 / static_cast<double>(square_img_view.width()), 2.0 / static_cast<double>(square_img_view.width()), 1.0};

        const Vec3d scale_vec1{2.0 / static_cast<double>(square_img_view.width()), 20.0 * 2.0 / static_cast<double>(square_img_view.width()), 1.0};

        world_.Step(timeStep, velocityIterations, positionIterations);
        spinner_body_->SetTransform(spinner_body_->GetPosition(), spinner_body_->GetAngle() + 0.01f);
        const b2Vec2 position = dynamic_body_->GetPosition();
        float angle = dynamic_body_->GetAngle();

        const properties::Transform transform0 = getTransform(square_img_view, scale_vec0, angle, position.x, position.y);
        setProperties(properties::ID0, transform0);

        const properties::Transform transform1 = getTransform(square_img_view, scale_vec1, -spinner_body_->GetAngle(), spinner_body_->GetPosition().x, spinner_body_->GetPosition().y);
        setProperties(properties::ID1, transform1);

    }
};

class ShapeImages
{
private:

    ImageRGBA<uint8_t> circle_;
    ImageRGBA<uint8_t> damper_;
    ImageRGBA<uint8_t> pentagon_;
    ImageRGBA<uint8_t> rounded_square_;
    ImageRGBA<uint8_t> spring_;
    ImageRGBA<uint8_t> square_;

public:
    ShapeImages()
    {
        const std::string bin_path = "../demos/tests/joints/images/";

        readShapeImage(bin_path + "circle.bin", circle_);
        readShapeImage(bin_path + "damper.bin", damper_);
        readShapeImage(bin_path + "pentagon.bin", pentagon_);
        readShapeImage(bin_path + "rounded_square.bin", rounded_square_);
        readShapeImage(bin_path + "spring.bin", spring_);
        readShapeImage(bin_path + "square.bin", square_);
    }

    ImageRGBAConstView<uint8_t> getCircle()
    {
        return circle_.constView();
    }

    ImageRGBAConstView<uint8_t> getDamper()
    {
        return damper_.constView();
    }

    ImageRGBAConstView<uint8_t> getPentagon()
    {
        return pentagon_.constView();
    }

    ImageRGBAConstView<uint8_t> getRoundedSquare()
    {
        return rounded_square_.constView();
    }

    ImageRGBAConstView<uint8_t> getSpring()
    {
        return spring_.constView();
    }

    ImageRGBAConstView<uint8_t> getSquare()
    {
        return square_.constView();
    }

};

void testBasic();



}



#endif // DEMOS_TESTS_JOINTS_JOINTS_H_