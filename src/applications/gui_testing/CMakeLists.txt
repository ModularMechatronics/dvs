cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(gui_test)

include_directories(${REPO_DIR}/src/interfaces/cpp)

set(CPP_SOURCE_FILES main.cpp
                     gui_pane.cpp
                     gui_pane_events.cpp
                     gui_element.cpp
                     shader.cpp
                     button.cpp
                     main_window.cpp)

add_executable(gui_test ${CPP_SOURCE_FILES})
target_link_libraries(gui_test ${wxWidgets_LIBRARIES}
                                    ${OPENGL_LIBRARIES}
                                    ${GLUT_LIBRARY}
                                    ${PLATFORM_LIBRARIES}
                                    ${REPO_DIR}/src/externals/freetype/build/libfreetype.a)
