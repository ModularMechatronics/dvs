
set(PLATFORM_LIBRARIES pthread
                       png
                       X11
                       Xxf86vm
                       Xext
                       ${CMAKE_DL_LIBS}
                       gtk-x11-2.0
                       gdk-x11-2.0
                       stdc++fs
                       pango-1.0
                       pangocairo-1.0
                       cairo
                       gdk_pixbuf-2.0
                       glib-2.0
                       gio-2.0
                       gobject-2.0
                       SM
                       ${wxWidgets_LIBRARIES})