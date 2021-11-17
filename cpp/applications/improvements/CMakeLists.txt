cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(main-app)

include_directories(${REPO_DIR}/cpp/main_application)
include_directories(${REPO_DIR}/interfaces/cpp)

set(EXT_CPP_FILES ${REPO_DIR}/cpp/main_application/opengl_low_level/3d/opengl_3d_functions.cpp
                  ${REPO_DIR}/cpp/main_application/opengl_low_level/opengl_text.cpp
                  ${REPO_DIR}/cpp/main_application/opengl_low_level/2d/opengl_2d_functions.cpp
                  ${REPO_DIR}/cpp/main_application/opengl_low_level/2d/opengl_2d_shape_primitives.cpp
                  ${REPO_DIR}/cpp/main_application/io_devices/keyboard_state.cpp
                  ${REPO_DIR}/cpp/main_application/io_devices/mouse_state.cpp
                  ${REPO_DIR}/cpp/main_application/axes/axes_interactor.cpp
                  ${REPO_DIR}/cpp/main_application/axes/axes_painter.cpp
                  ${REPO_DIR}/cpp/main_application/axes/structures/axes_limits.cpp
                  ${REPO_DIR}/cpp/main_application/axes/structures/axes_settings.cpp
                  ${REPO_DIR}/cpp/main_application/axes/structures/coordinate_converter.cpp
                  ${REPO_DIR}/cpp/main_application/axes/structures/view_angles.cpp
                  ${REPO_DIR}/cpp/main_application/axes/plot_box/plot_box_coordinate_arrows.cpp
                  ${REPO_DIR}/cpp/main_application/axes/plot_box/plot_box_grid_numbers.cpp
                  ${REPO_DIR}/cpp/main_application/axes/plot_box/plot_box_grid.cpp
                  ${REPO_DIR}/cpp/main_application/axes/plot_box/plot_box_outline.cpp
                  ${REPO_DIR}/cpp/main_application/axes/plot_box/plot_box_walls.cpp
                  ${REPO_DIR}/cpp/main_application/misc/number_formatting.cpp
                  ${REPO_DIR}/cpp/main_application/misc/color_map.cpp
                  ${REPO_DIR}/cpp/main_application/globals.cpp
                  ${REPO_DIR}/cpp/main_application/tab_view.cpp
                  ${REPO_DIR}/cpp/main_application/window_view.cpp
                  ${REPO_DIR}/cpp/main_application/events.cpp
                  ${REPO_DIR}/cpp/main_application/plot_data.cpp
                  ${REPO_DIR}/cpp/main_application/plot_functions/plot_functions.cpp
                  ${REPO_DIR}/cpp/main_application/plot_window_gl_pane.cpp
                  ${REPO_DIR}/cpp/main_application/shader.cpp)

set(CPP_SOURCE_FILES main.cpp
                     main_window.cpp
                     gl_canvas.cpp
                     gl_canvas_misc.cpp
                     axes_renderer.cpp
                     vbo_wrapper.cpp)

add_executable(improvements ${CPP_SOURCE_FILES} ${EXT_CPP_FILES})
target_link_libraries(improvements ${wxWidgets_LIBRARIES}
                          ${OPENGL_LIBRARIES}
                          ${GLUT_LIBRARY}
                          ${PLATFORM_LIBRARIES})
