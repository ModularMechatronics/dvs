
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/Users/annotelldaniel/work/repos/dvs/src/externals/wxwidgets/lib/wx/include/osx_cocoa-unicode-static-3.1 -I/Users/annotelldaniel/work/repos/dvs/src/externals/wxwidgets/include -D_FILE_OFFSET_BITS=64 -D__WXMAC__ -D__WXOSX__ -D__WXOSX_COCOA__")
link_directories(/usr/local/Cellar/libpng/1.6.39/lib)

set(wxWidgets_LIBRARIES libwx_osx_cocoau-3.1.a
                        libwx_osx_cocoau_gl-3.1.a
                        libwxpng-3.1.a
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