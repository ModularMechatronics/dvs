
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FILE_OFFSET_BITS=64 -D__WXMAC__ -D__WXOSX__ -D__WXOSX_COCOA__")

#  -mmacosx-version-min=10.1.5 --sysroot=/Users/danielpi/MacOSX-SDKs/MacOSX10.5.sdk
# -x objective-c++ -framework Foundation
# set(CMAKE_OSX_DEPLOYMENT_TARGET "10.1" CACHE STRING "Minimum macOS version")
link_directories(/usr/local/Cellar/libpng/1.6.43/lib)
link_directories(/usr/local/Cellar/libpng/1.6.39/lib)
link_directories(/usr/local/Cellar/libpng/1.6.44/lib)

set(wxWidgets_LIBRARIES libwx_osx_cocoau-3.1.a
                        libwx_osx_cocoau_gl-3.1.a
                        libwxexpat-3.1.a
                        libwxpng-3.1.a
                        libwxregexu-3.1.a
                        libwxscintilla-3.1.a
                        libwxzlib-3.1.a)
# libwxscintilla-3.1.a
# libwxregexu-3.1.a
# libwxexpat-3.1.a
# 

set(PLATFORM_LIBRARIES pthread
                       iconv
                       png
                       z
                       "-framework System"
                       "-framework IOKit"
                       "-framework OpenGL"
                       "-framework AGL"
                       "-framework Carbon"
                       "-framework Cocoa"
                       "-framework QuartzCore")