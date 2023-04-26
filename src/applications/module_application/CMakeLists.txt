cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

include_directories(${REPO_DIR}/src/interfaces/cpp)


project(module-lib)

add_library(module-lib SHARED module.cpp)

project(module-app)

set(CPP_SOURCE_FILES main.cpp
                     # module.cpp
                     main_window.cpp
                     dynamic_module.cpp
                     gui_elements_callbacks.cpp)

add_executable(module-app ${CPP_SOURCE_FILES})
target_link_libraries(module-app ${wxWidgets_LIBRARIES}
                                 ${PLATFORM_LIBRARIES})
