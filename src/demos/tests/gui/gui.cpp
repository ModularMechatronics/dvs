#include "tests/gui/gui.h"

namespace gui_test
{

void testBasic()
{
    const std::string project_file_path = "../../project_files/demo/gui_demo0.dvs";

    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();
    axis({-5.0, -5.0, 0.0}, {5.0, 5.0, 10.0});

    const auto plot_data = [](const double v0, const double v1, const double v2) -> void {
        const size_t n_elems = 30U;

        const std::pair<Matrix<double>, Matrix<double>> uv_mats =
            meshGrid<double>(1.0, 5.0, 1.0, 10.0, n_elems, n_elems);
        Matrix<double> u{uv_mats.first}, v{uv_mats.second};

        const Matrix<double> x = elementWiseMultiply(v0 * u, sin(v));
        const Matrix<double> y = elementWiseMultiply(-u * v2, cos(v));
        const Matrix<double> z = v * v1;

        surf(x, y, z, properties::EdgeColor(0, 0, 0), properties::ColorMap::JET_BRIGHT);
        softClearView();
    };

    dvs::gui::registerGuiCallback("slider0", [&plot_data](const dvs::gui::SliderHandle& gui_element_handle) -> void {
        const dvs::gui::SliderHandle slider1 = dvs::gui::getGuiElementHandle<dvs::gui::SliderHandle>("slider1");
        const dvs::gui::SliderHandle slider2 = dvs::gui::getGuiElementHandle<dvs::gui::SliderHandle>("slider2");

        const double v0 = static_cast<double>(gui_element_handle.getValue()) / 100.0;
        const double v1 = static_cast<double>(slider1.getValue()) / 100.0;
        const double v2 = static_cast<double>(slider2.getValue()) / 100.0;

        plot_data(v0, v1, v2);
    });

    dvs::gui::registerGuiCallback("slider1", [&plot_data](const dvs::gui::SliderHandle& gui_element_handle) -> void {
        const dvs::gui::SliderHandle slider0 = dvs::gui::getGuiElementHandle<dvs::gui::SliderHandle>("slider0");
        const dvs::gui::SliderHandle slider2 = dvs::gui::getGuiElementHandle<dvs::gui::SliderHandle>("slider2");

        const double v0 = static_cast<double>(slider0.getValue()) / 100.0;
        const double v1 = static_cast<double>(gui_element_handle.getValue()) / 100.0;
        const double v2 = static_cast<double>(slider2.getValue()) / 100.0;

        plot_data(v0, v1, v2);
    });

    dvs::gui::registerGuiCallback("slider2", [&plot_data](const dvs::gui::SliderHandle& gui_element_handle) -> void {
        const dvs::gui::SliderHandle slider0 = dvs::gui::getGuiElementHandle<dvs::gui::SliderHandle>("slider0");
        const dvs::gui::SliderHandle slider1 = dvs::gui::getGuiElementHandle<dvs::gui::SliderHandle>("slider1");

        const double v0 = static_cast<double>(slider0.getValue()) / 100.0;
        const double v1 = static_cast<double>(slider1.getValue()) / 100.0;
        const double v2 = static_cast<double>(gui_element_handle.getValue()) / 100.0;

        plot_data(v0, v1, v2);
    });

    /*dvs::gui::registerGuiCallback("button0", [](const dvs::gui::ButtonHandle& gui_element_handle) -> void {
        std::cout << "Callback function: \"button0\" pressed: " << gui_element_handle.getIsPressed() << std::endl;
    });

    dvs::gui::registerGuiCallback("checkbox0", [](const dvs::gui::CheckboxHandle& gui_element_handle) -> void {
        std::cout << "Callback function: \"checkbox0\" pressed: " << gui_element_handle.getIsChecked() << std::endl;
    });*/

    dvs::gui::startGuiReceiveThread();

    std::cout << "Instructions:" << std::endl << "\"q\": Exits application" << std::endl;

    // Client application loop
    while (true)
    {
        std::string input;
        std::getline(std::cin, input);

        if (input == "q")
        {
            break;
        }
    }
}

}  // namespace gui_test
