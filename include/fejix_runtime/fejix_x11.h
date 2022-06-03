#ifndef _FEJIX_X11_H_
#define _FEJIX_X11_H_


#include <fejix_runtime/fejix_defines.h>

#include <X11/Xlib-xcb.h>

#ifdef FJ_USE_OPENGL3
#   include <glad/glx.h>

// We are going to include <glad/gl.h> to load OpenGL
// If we included <GL/glx.h> it would include <GL/gl.h>
// which is incompatible with <glad/gl.h> 
// #   include <GL/glx.h>   // <-- So, we do not use this
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
    struct FjInstance *instance;
    void *data;

    uint32_t width;
    uint32_t height;

#ifdef FJ_USE_OPENGL3
    GLXWindow glxwin;
    GLXContext glctx;
#endif
};


#endif // _FEJIX_X11_H_