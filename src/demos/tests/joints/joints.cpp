#include "tests/joints/joints.h"


namespace joints
{

void testBasic()
{
    const std::string project_file_path = "../../project_files/joints.dvs";

    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();

    view(0, 90);

    double offset = 0.0;

    ShapeImages shape_images{};
    
    const double offset_x = static_cast<double>(shape_images.getCircle().width()) / 2.0;
    const double offset_y = static_cast<double>(shape_images.getCircle().height()) / 2.0;

    axis({-2.0, -2.0, -10.0}, {2.0, 2.0, 10.0});

    imShow(shape_images.getCircle(), properties::ID0);

    /*imShow(shape_images.getCircle(), properties::Transform{scale, unitMatrix<double>(3, 3), {0, offset, 0}});
    offset += shape_images.getCircle().height() * scale(0, 0) + 1.0;
    imShow(shape_images.getDamper(), properties::Transform{scale, unitMatrix<double>(3, 3), {0, offset, 0}});
    offset += shape_images.getDamper().height() * scale(0, 0) + 1.0;
    imShow(shape_images.getPentagon(), properties::Transform{scale, unitMatrix<double>(3, 3), {0, offset, 0}});
    offset += shape_images.getPentagon().height() * scale(0, 0) + 1.0;
    imShow(shape_images.getRoundedSquare(), properties::Transform{scale, unitMatrix<double>(3, 3), {0, offset, 0}});
    offset += shape_images.getRoundedSquare().height() * scale(0, 0) + 1.0;
    imShow(shape_images.getSpring(), properties::Transform{scale, unitMatrix<double>(3, 3), {0, offset, 0}});
    offset += shape_images.getSpring().height() * scale(0, 0) + 1.0;
    imShow(shape_images.getSquare(), properties::Transform{scale, unitMatrix<double>(3, 3), {0, offset, 0}});
    offset += shape_images.getSquare().height() * scale(0, 0) + 1.0;
    imShow(shape_images.getStar(), properties::Transform{scale, unitMatrix<double>(3, 3), {0, offset, 0}});
    offset += shape_images.getStar().height() * scale(0, 0) + 1.0;*/

    double phi = 0.0;

    const auto img_view = shape_images.getCircle();

    const double width = static_cast<double>(img_view.width()) / 2.0;

    const Vec3d scale_vec{1.0 / width, 1.0 / width, 0};
    const auto scale = diagMatrix<double>(scale_vec);
    const Vec3d center_of_rotation = Vec3d{static_cast<double>(img_view.width()) / 2.0,
        static_cast<double>(img_view.height()) / 2.0,
        0.0}.elementWiseMultiply(scale_vec);

    for(size_t k = 0; k < 100; ++k)
    {
        const auto rot_mat = rotationMatrixZ<double>(phi);
        const auto translation_vec = -rotationMatrixZ<double>(-phi) * center_of_rotation;
        setProperties(properties::ID0, properties::Transform{scale, rot_mat, translation_vec});
        usleep(1000 * 100);
        phi += 0.1;
    }

    

}

}
