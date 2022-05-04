cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(main-app)

include_directories(${REPO_DIR}/cpp/main_application)
include_directories(${REPO_DIR}/interfaces/cpp)

set(CPP_SOURCE_FILES main.cpp
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

                     plot_objects/plot_object_base/plot_object_base.cpp
                     plot_objects/draw_arrow/draw_arrow.cpp
                     plot_objects/draw_line3d/draw_line3d.cpp
                     plot_objects/draw_mesh/draw_mesh.cpp
                     plot_objects/draw_plane_xy/draw_plane_xy.cpp
                     plot_objects/draw_plane_xz/draw_plane_xz.cpp
                     plot_objects/draw_plane_yz/draw_plane_yz.cpp
                     plot_objects/draw_polygon_4_points/draw_polygon_4_points.cpp
                     plot_objects/draw_tiles/draw_tiles.cpp
                     plot_objects/draw_triangles_3d/draw_triangles_3d.cpp
                     plot_objects/im_show/im_show.cpp
                     plot_objects/plot2_collection/plot2_collection.cpp
                     plot_objects/plot2d/plot2d.cpp
                     plot_objects/plot3d/plot3d.cpp
                     plot_objects/quiver/quiver.cpp
                     plot_objects/scatter/scatter.cpp
                     plot_objects/scatter3/scatter3.cpp
                     plot_objects/stairs/stairs.cpp
                     plot_objects/surf/surf.cpp

                     globals.cpp

                     tray_icon.cpp
                     tab_view.cpp
                     window_view.cpp
                     events.cpp
                     layout_tools_window.cpp
                     plot_data.cpp
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
