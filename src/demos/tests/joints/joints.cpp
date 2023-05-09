#include "tests/joints/joints.h"


namespace joints
{

properties::Transform getTransform(const ImageRGBAConstView<uint8_t>& img, const Vec3d& scale_vec, const double phi, const double x_offset, const double y_offset)
{
    const double image_width = img.width();
    const double image_height = img.height();

    const auto scale_mat = diagMatrix<double>(scale_vec);
    const Vec3d center_of_rotation = Vec3d{image_width / 2.0, image_height / 2.0, 0.0}.elementWiseMultiply(scale_vec);

    const Matrix<double> rot_mat = rotationMatrixZ<double>(phi);
    const Vec3<double> translation_vec = -rotationMatrixZ<double>(-phi) * center_of_rotation + Vec3d{x_offset, y_offset, 0.0};

    return properties::Transform{scale_mat, rot_mat, translation_vec};
}

void testBasic()
{
    const std::string project_file_path = "../../project_files/joints.dvs";

    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();

    view(0, 90);

    double offset = 0.0;

    ShapeImages shape_images{};

    axis({-2.0, -2.0, -10.0}, {2.0, 2.0, 10.0});

    imShow(shape_images.getCircle(), properties::ID0);

    double phi = 0.0;

    const auto img_view = shape_images.getCircle();

    const Vec3d scale_vec{2.0 / static_cast<double>(img_view.width()), 2.0 / static_cast<double>(img_view.width()), 1.0};

    for(size_t k = 0; k < 100; ++k)
    {
        const double x = std::sin(phi);
        const properties::Transform transform = getTransform(img_view, scale_vec, phi, x, 0.0);
        setProperties(properties::ID0, transform);

        usleep(1000 * 100);
        phi += 0.1;
    }
}

}
