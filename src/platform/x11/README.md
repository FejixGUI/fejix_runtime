This implementation of Fejix for the X11 protocol uses both XCB and Xlib libraries. It requires `<X11/Xlib-xcb.h>`, which includes the `<X11/Xlib.h>` and `<xcb/xcb.h>`

## Reading list

* [Xlib is outdated, so we use XCB](https://www.x.org/wiki/Documentation/).
* [But we must use Xlib in order to use OpenGL](https://xcb.freedesktop.org/opengl/).