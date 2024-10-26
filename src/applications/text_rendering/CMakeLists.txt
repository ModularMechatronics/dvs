cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(text_rendering)

include_directories(${REPO_DIR}/src/interfaces/cpp)

set(CPP_SOURCE_FILES main.cpp
                     gui_pane.cpp
                     shader.cpp
                     text_rendering.cpp
                     main_window.cpp)

add_executable(text_rendering ${CPP_SOURCE_FILES})
target_link_libraries(text_rendering ${wxWidgets_LIBRARIES}
                                    ${OPENGL_LIBRARIES}
                                    ${GLUT_LIBRARY}
                                    ${PLATFORM_LIBRARIES}
                                    /Users/danielpi/work/dvs/src/externals/freetype/build/libfreetype.a)
