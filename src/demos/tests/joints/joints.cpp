#include "tests/joints/joints.h"

namespace joints
{

void testBasic()
{
    const std::string project_file_path = "../../project_files/joints.dvs";

    // const ImageRGBA<std::uint8_t> circle = createCircle(100, 255, 0, 0);
    // const ImageRGBA<std::uint8_t> rectangle = createRectangle(100, 200, 255, 255, 0);

    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();

    view(0, 90);
    axis({-20.0, -20.0, -20.0}, {20.0, 20.0, 20.0});

    Joints joints{};

    for (size_t k = 0; k < 1000; k++)
    {
        joints.step();
        usleep(1000 * 10);
    }
    // imShow(rectangle);
    /*imShow(circle, properties::ID3);
    imShow(circle, properties::ID2);
    imShow(circle, properties::ID1);
    imShow(circle, properties::ID0);

    using Tr = properties::Transform;

    setProperties(properties::ID1,
                  Tr{diagMatrix<double>(Vec3d{1.0, 1.0, 1.0}), unitMatrix<double>(3, 3), {100.0, 0.0, 0.0}});
    setProperties(properties::ID2,
                  Tr{diagMatrix<double>(Vec3d{1.0, 1.0, 1.0}), unitMatrix<double>(3, 3), {200.0, 0.0, 0.0}});
    setProperties(properties::ID3,
                  Tr{diagMatrix<double>(Vec3d{1.0, 1.0, 1.0}), unitMatrix<double>(3, 3), {300.0, 0.0, 0.0}});*/
}

void testBasic2()
{
    const std::string project_file_path = "../../project_files/joints.dvs";

    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();

    view(0, 90);
    axis({-20.0, -20.0, -20.0}, {20.0, 20.0, 20.0});

    Joints joints{};

    for (size_t k = 0; k < 1000; k++)
    {
        joints.step();
        usleep(1000 * 10);
    }
}

void testBasic3()
{
    /*const std::string project_file_path = "../../project_files/joints.dvs";

    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();

    view(0, 90);

    double offset = 0.0;

    ShapeImages shape_images{};

    axis({-2.0, -2.0, -10.0}, {2.0, 2.0, 10.0});

    const auto img_view = shape_images.getDamper();
    imShow(img_view, properties::ID0);

    double phi = 0.0;

    const Vec3d scale_vec{2.0 / static_cast<double>(img_view.width()), 2.0 /
    static_cast<double>(img_view.width()), 1.0};

    const properties::Transform transform = getTransform(img_view, scale_vec, 0.0, 0.0, 0.0);
    setProperties(properties::ID0, transform);

    // Pentagon
    const std::vector<Point2d> pentagon_points = {
        Point2d{-0.984375, 0.21875},
        Point2d{0, 0.9375},
        Point2d{0.984375, 0.2},
        Point2d{0.609375, -0.9375},
        Point2d{0.609375, -0.9375}
    };

    // Circle
    const Point2d circle_center{0.0, 0.0};
    const double circle_radius{1.0};

    // Damper
    const Point2d damper_left_point{-1.0, 0.0};
    const Point2d damper_right_point{1.0, 0.0};

    // Rounded square
    const Point2d rounded_square_center{0.0, 0.0};
    const double rounded_square_side_length{2.0};

    // Square
    const Point2d square_center{0.0, 0.0};
    const double square_side_length{2.0};

    // Spring
    const Point2d spring_left_point{-0.917969, 0.0};
    const Point2d spring_right_point{0.917969, 0.0};

    for(size_t k = 0; k < 100; ++k)
    {
        const double x = std::sin(phi);
        const properties::Transform transform = getTransform(img_view, scale_vec, phi, x, 0.0);
        setProperties(properties::ID0, transform);

        usleep(1000 * 100);
        phi += 0.1;
    }*/
}

}  // namespace joints
