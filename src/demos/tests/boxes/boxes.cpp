#include "tests/boxes/boxes.h"

using namespace reactphysics3d;

namespace boxes
{

struct PointsAndIndices
{
    Vector<Point3f> points;
    Vector<IndexTriplet> indices;
};

void drawGrid()
{
    const float grid_step = 2.5;
    const float grid_size = 200.0;

    std::vector<float> x_vec, y_vec;

    float x_pos = -grid_size / 2.0f, y_pos = -grid_size / 2.0f;

    const size_t n_its = static_cast<float>(grid_size / grid_step);

    for (size_t k = 0; k < n_its; k++)
    {
        // Left point
        x_vec.push_back(x_pos);
        y_vec.push_back(y_pos);

        // Right point
        if ((k % 2) == 0)
        {
            x_pos += grid_size;
        }
        else
        {
            x_pos -= grid_size;
        }

        x_vec.push_back(x_pos);
        y_vec.push_back(y_pos);

        y_pos += grid_step;
    }

    x_pos = -grid_size / 2.0f;
    y_pos = -grid_size / 2.0f;

    for (size_t k = 0; k < n_its; k++)
    {
        // Left point
        x_vec.push_back(x_pos);
        y_vec.push_back(y_pos);

        // Right point
        if ((k % 2) == 0)
        {
            y_pos += grid_size;
        }
        else
        {
            y_pos -= grid_size;
        }

        x_vec.push_back(x_pos);
        y_vec.push_back(y_pos);

        x_pos += grid_step;
    }

    Vector<float> x{x_vec}, y{y_vec};

    plot(x, y, properties::Color(255, 0, 0), properties::ZOffset(-1.0f), properties::LineWidth(1.0f));
}

PointsAndIndices generateCube(const float size = 1.0f)
{
    PointsAndIndices pts_indices{};

    // clang-format off
    pts_indices.points = Vector<Point3f>{VectorInitializer<Point3f>{
        {size, size, size},
        {size, -size, size},
        {-size, -size, size},
        {-size, size, size},
        {size, size, -size},
        {size, -size, -size},
        {-size, -size, -size},
        {-size, size, -size},
    }};

    pts_indices.indices = Vector<IndexTriplet>{VectorInitializer<IndexTriplet>{
        {0, 2, 1}, // Upper xy
        {0, 3, 2},
        {4, 5, 6}, // Lower xy
        {4, 6, 7},
        {0, 1, 4}, // Upper yz
        {1, 5, 4},
        {2, 6, 3}, // Lower yz
        {6, 7, 3},
        {0, 7, 3}, // Upper xz
        {0, 4, 7},
        {6, 2, 1}, // Lower xz
        {6, 1, 5},
    }};
    // clang-format on

    return pts_indices;
}

const Vector3 kCubeExtents(0.1, 0.1, 0.1);

Matrix<float> r3dMatToMat(const Matrix3x3& mat)
{
    Matrix<float> m(3, 3);

    for(size_t r = 0; r < 3; r++)
    {
        for(size_t c = 0; c < 3; c++)
        {
            m(r, c) = mat[r][c];
        }
    }

    return m;
}

Matrix3x3 matToR3dMat(const Matrix<float>& mat)
{
    Matrix3x3 m;

    for(size_t r = 0; r < 3; r++)
    {
        for(size_t c = 0; c < 3; c++)
        {
            m[r][c] = mat(r, c);
        }
    }

    return m;
}

RigidBody* placeCubeAt(PhysicsWorld* world, PhysicsCommon& physicsCommon, const Quaternion& orientation, const Vector3& position, const Vector3& size = kCubeExtents)
{
    // Create a rigid body in the world
    BoxShape* boxShape = physicsCommon.createBoxShape(size);

    const Transform transform(position, orientation);
    RigidBody* body = world->createRigidBody(transform);

    Collider* collider_body = body->addCollider(boxShape, Transform{Vector3(0, 0, 0), Quaternion::identity()});
    Material& material = collider_body->getMaterial(); 
    
    // Change the bounciness of the collider 
    material.setBounciness(0.1); 
    
    // Change the friction coefficient of the collider 
    material.setFrictionCoefficient(0.2);

    return body;
}

void testBasic()
{
    const std::string project_file_path = "../../project_files/boxes.dvs";
    openProjectFile(project_file_path);

    PhysicsCommon physicsCommon;

    PhysicsWorld::WorldSettings settings; 
    settings.defaultVelocitySolverNbIterations = 20; 
    settings.isSleepingEnabled = false; 
    settings.gravity = Vector3(0, 0.0, 0); 

    // Create a physics world
    PhysicsWorld* world = physicsCommon.createPhysicsWorld(settings);

    
    const Vector3 halfExtents(10.0, 0.1, 10.0);
    BoxShape* boxShapeGround = physicsCommon.createBoxShape(halfExtents); 

    /*Vector3 ground_position(0, 0, 0);
    Transform ground_transform(ground_position, Quaternion::identity());
    RigidBody* ground_body = world->createRigidBody(ground_transform);
    ground_body->setType(BodyType::STATIC);

    Collider* collider; 
    collider = ground_body->addCollider(boxShapeGround, Transform{Vector3(0, 0, 0), Quaternion::identity()});*/

    float timeStep = 1.0f / 600.0f;

    setCurrentElement("p_view_0");
    clearView();
    globalIllumination({2.0, 2.0, 2.0});
    disableScaleOnRotation();
    waitForFlush();

    const float bullet_size = 0.8f;

    PointsAndIndices cube_pts_indices = generateCube();
    PointsAndIndices cube_pts_indices_10 = generateCube(bullet_size);
    const properties::FaceColor cube_color{242, 152, 124};
    properties::Transform cube_transform{diagMatrix<double>({0.15, 0.15, 1.0}), rotationMatrixX<double>(0.0), {0.0, 0.0, 2.0}};

    std::vector<RigidBody*> bodies;
    const size_t num_rows = 5;
    const size_t num_cols = 5;
    const size_t num_cubes_z = 7;

    const auto rand_func = [] () -> std::uint8_t {
        const std::size_t v = 150U;
        return (rand() % v) + (256U - v);
        };

    size_t idx = 0;
    const float o_offset = 1.0f;
    for(size_t r = 0; r < num_rows; r++)
    {
        const float rf = r;

        for(size_t c = 0; c < num_cols; c++)
        {
            const float cf = c;

            for(size_t z = 0; z < num_cubes_z; z++)
            {
                const float zf = z;
                RigidBody* body = placeCubeAt(world, physicsCommon, Quaternion::identity(), Vector3{
                    1.001f * rf * 2.0f * kCubeExtents.x,
                    1.001f * kCubeExtents.y * 2.0f + zf * 2.0f * kCubeExtents.y,
                    1.001f * cf * 2.0f * kCubeExtents.z});
                bodies.push_back(body);

                drawMesh(cube_pts_indices.points,
                        cube_pts_indices.indices,
                        cube_color,
                        properties::EdgeColor::NONE,
                        properties::FaceColor(rand_func(), rand_func(), rand_func()),
                        static_cast<internal::ItemId>(idx),
                        cube_transform);
                assert(idx < 254);
                idx++;
            }
        }
    }

    const Quaternion bullet_rotation{Quaternion::fromEulerAngles(M_PI / debug_value_reader::readFloat("div_x"),
                                                                 M_PI / debug_value_reader::readFloat("div_y"),
                                                                 M_PI / debug_value_reader::readFloat("div_z"))};

    RigidBody* bullet_body = placeCubeAt(world, physicsCommon, bullet_rotation, Vector3{
                    0.375f,
                    0.75f,
                    8.0f},
                    Vector3(bullet_size, bullet_size, bullet_size));
    bullet_body->setMass(bullet_body->getMass() * 100.0f);

    drawMesh(cube_pts_indices_10.points,
            cube_pts_indices_10.indices,
            cube_color,
            properties::EdgeColor::NONE,
            properties::ID254,
            cube_transform);
    float axis_zoom_scale = 2.0f;
    axis({-axis_zoom_scale, -axis_zoom_scale, -axis_zoom_scale}, {axis_zoom_scale, axis_zoom_scale, axis_zoom_scale});

    float azimuth = -52.0f;
    float azimuth_ref = -52.0f;
    // axis({-3.0, 0.0, -4.0}, {5.0, 8.0, 4.0});
    // view(-48, 17);
    view(azimuth, 14);
    // view(-73, 43);
    bullet_body->setLinearVelocity(Vector3{0.0, 0.0, -10.0});
    // bullet_body->setAngularVelocity(Vector3{50.0, 50.0, 50.0});

    bool run_simulation = true;
    const auto ps = bullet_body->getTransform().getPosition();
    Vec3f axes_center{ps.x, ps.y, ps.z};
    Vec3f axes_center_filtered = axes_center;
    bool center_on_bullet = true;

    drawGrid();

    // Step the simulation a few steps
    for (int i = 0; i < 5000; i++)
    {
        if(i < 350)
        {
            // axes_center = Vec3f(0.0, 0.0, 0.0);
        }
        else if (i < 500)
        {

        }
        else if((i < 700) && ((timeStep - 1.0 / 60000.0) > 0.0f))
        {
            timeStep -= 1.0 / 70000.0;
        }
        else if (i < 1000)
        {
            // azimuth += 0.1f;
            azimuth_ref += 0.5f;
            const float h = 0.99f;
            azimuth = azimuth_ref * (1.0f - h) + azimuth * h;
            view(azimuth, 14);
            run_simulation = false;
        }
        else if(i < 1500)
        {
            // azimuth_ref += 0.5f;
            const float h = 0.99f;
            azimuth = azimuth_ref * (1.0f - h) + azimuth * h;
            timeStep = std::min(timeStep + 1.0f / 70000.0f, 1.0f / 600.0f);
            view(azimuth, 14);

            run_simulation = true;
        }
        else if(i == 1600)
        {
            center_on_bullet = false;
            // axes_center = Vec3f(0.0)
        }
        else
        {
            // azimuth_ref += 0.5f;
            const float h = 0.99f;
            azimuth = azimuth_ref * (1.0f - h) + azimuth * h;
            timeStep = std::min(timeStep + 1.0f / 7000.0f, 1.0f / 60.0f);
            view(azimuth, 14);

            run_simulation = true;
        }

        if(run_simulation)
        {
            world->update(timeStep);
        }

        std::vector<PropertySet> props;

        for(size_t k = 0; k < bodies.size(); k++)
        {
            RigidBody* body = bodies[k];

            // Get the updated position of the body
            const Transform& transform = body->getTransform();
            const Vector3& position = transform.getPosition();
            const Matrix<float> rot_mat = r3dMatToMat(transform.getOrientation().getMatrix()); 

            props.emplace_back(static_cast<internal::ItemId>(k), properties::Transform{diagMatrix<double>({0.1, 0.1, 0.1}), rot_mat, {position.x, position.z, position.y}});
        }

        const Transform& transform = bullet_body->getTransform();
        const Vector3& position = transform.getPosition();
        if(center_on_bullet)
        {
            axes_center = Vec3f(position.x, position.y, position.z);
        }
        else
        {
            // axis({position.x -4.0, position.z -4.0, position.y -4.0}, {position.x + 4.0, position.z +4.0, position.y + 4.0});
        }
        const float h = 0.95f;
        const float h1 = 1.0f - h;
        azimuth = azimuth_ref * (1.0f - h) + azimuth * h;
        
        axes_center_filtered.x = axes_center.x * h1 + axes_center_filtered.x * h;
        axes_center_filtered.z = axes_center.z * h1 + axes_center_filtered.z * h;
        axes_center_filtered.y = axes_center.y * h1 + axes_center_filtered.y * h;

        axis({
                axes_center_filtered.x -axis_zoom_scale,
                axes_center_filtered.z -axis_zoom_scale,
                axes_center_filtered.y -axis_zoom_scale},
                {axes_center_filtered.x + axis_zoom_scale,
                 axes_center_filtered.z +axis_zoom_scale,
                 axes_center_filtered.y + axis_zoom_scale});
        const Matrix<float> rot_mat_bullet = r3dMatToMat(transform.getOrientation().getMatrix());

        props.emplace_back(properties::ID254, properties::Transform{diagMatrix<double>({1.0, 1.0, 1.0}), rot_mat_bullet, {position.x, position.z, position.y}});

        setProperties(props);
        std::cout << i << std::endl;
        flushCurrentElement();
        // std::cin.ignore();
        // usleep(1000 * 10);
        // softClearView();
    }

    
    // waitForFlush();
    // axesSquare();
    // disableScaleOnRotation();
    // setAxesBoxScaleFactor({1.0, 1.0, 1.0});
}

}  // namespace boxes
