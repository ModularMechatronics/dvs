
find_package(wxWidgets COMPONENTS core base gl adv)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -D__WXGTK__")

set(WX_CUSTOM_INC_PATH ${REPO_DIR}/cpp/externals/wxwidgets/include)
set(WX_CUSTOM_LIB_PATH ${REPO_DIR}/cpp/externals/wxwidgets/lib)

set(PLATFORM_LIBRARIES -lstdc++fs)