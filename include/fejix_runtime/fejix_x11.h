#ifndef _FEJIX_X11_H_
#define _FEJIX_X11_H_


#include <fejix_runtime/fejix_defines.h>

#include <X11/Xlib-xcb.h>

#ifdef FJ_USE_OPENGL3
#   include <glad/glx.h>
#endif


// Why isn't it included in XCB???
// It was so annoying to figure this out!
#define FJ_XCB_EVENT_MASK (~0x80)


struct FjInstance {
    // Xlib stuff
    Display *xDisplay;
    int32_t xDefaultScreen;

    // Xcb things
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_atom_t 
        atom_NET_WM_NAME,
        atom_UTF8_STRING,
        atom_WM_DELETE_WINDOW,
        atom_WM_PROTOCOLS;

    xcb_visualid_t windowVisualId;
    xcb_colormap_t colormapId;

    uint32_t backend;

#ifdef FJ_USE_OPENGL3
    GLXFBConfig gl3_framebufferConfig;
#endif

};


struct FjWindow {
    xcb_window_t windowId;
    struct FjInstance *inst;
    void *data;

#ifdef FJ_USE_OPENGL3
    GLXWindow glxwin;
    GLXContext glctx;
#endif
};


#endif // _FEJIX_X11_H_