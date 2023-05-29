#include "tests/joints/joints.h"

namespace joints
{

void testBasic()
{
    const std::string project_file_path = "../../project_files/joints.dvs";

    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();
    waitForFlush();

    view(0, 90);
    axis({-16.0, -32.0, -20.0}, {40.0, 24.0, 20.0});

    Joints joints{};

    for (size_t k = 0; k < 1000; k++)
    {
        joints.step();
        flushCurrentElement();
        usleep(1000 * 10);
        // deletePlotObject(properties::ID250);
        // deletePlotObject(properties::ID249);
    }
}

}  // namespace joints
