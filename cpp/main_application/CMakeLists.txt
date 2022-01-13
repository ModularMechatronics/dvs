cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(main-app)

include_directories(${REPO_DIR}/cpp/main_application)
include_directories(${REPO_DIR}/interfaces/cpp)

set(CPP_SOURCE_FILES main.cpp
                     opengl_low_level/3d/opengl_3d_functions.cpp
                     opengl_low_level/opengl_text.cpp
                     opengl_low_level/2d/opengl_2d_functions.cpp
                     opengl_low_level/2d/opengl_2d_shape_primitives.cpp
                     io_devices/keyboard_state.cpp
                     io_devices/mouse_state.cpp
                     axes/axes_interactor.cpp

                     axes/axes_renderer.cpp
                     axes/plot_box_walls.cpp
                     axes/plot_box_silhouette.cpp
                     axes/grid_numbers.cpp
                     axes/plot_box_grid.cpp
                     axes/text_rendering.cpp

                     shader.cpp

                     axes/structures/axes_limits.cpp
                     axes/structures/axes_settings.cpp
                     axes/structures/view_angles.cpp

                     misc/number_formatting.cpp
                     misc/color_map.cpp

                     globals.cpp

                     tab_view.cpp
                     window_view.cpp
                     events.cpp
                     layout_tools_window.cpp
                     plot_data.cpp
                     plot_functions/plot_functions.cpp
                     plot_window_gl_pane.cpp
                     main_window.cpp
                     main_window_receive.cpp
                     main_window_gui.cpp)


add_executable(dvs ${CPP_SOURCE_FILES})
target_link_libraries(dvs ${wxWidgets_LIBRARIES}
                          ${OPENGL_LIBRARIES}
                          ${GLUT_LIBRARY}
                          ${PLATFORM_LIBRARIES}
                          /Users/danielpi/work/dvs/cpp/externals/freetype/build/libfreetype.a)
