cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(main-app)

include_directories(${REPO_DIR}/cpp/dvs)
include_directories(${REPO_DIR}/cpp/communication)

set(CPP_SOURCE_FILES main.cpp
                     main_window.cpp)

add_executable(dvs ${CPP_SOURCE_FILES})
target_link_libraries(dvs ${wxWidgets_LIBRARIES})
