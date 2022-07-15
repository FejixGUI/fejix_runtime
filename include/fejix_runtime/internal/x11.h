#ifndef _FEJIX_X11_H_
#define _FEJIX_X11_H_


#include <fejix_runtime/definitions.h>
#include <fejix_runtime/backend.h>
#include <fejix_runtime/events.h>

#include <X11/Xlib-xcb.h>
#include <xcb/sync.h>
#include <xcb/xcb_icccm.h>

#ifdef FJ_USE_OPENGL3
#   include <glad/glx.h>

// We are going to include <glad/gl.h> to load OpenGL.
// If we included <GL/glx.h>, it would include <GL/gl.h>
// which is incompatible with <glad/gl.h> 
// #   include <GL/glx.h>   // <-- So, we do not use this
#endif





struct FjApp {
    struct FjWindow **windows;
    uint32_t windowsLen;

    struct FjBackend backend;

    // Xlib stuff
    Display *xDisplay;
    int32_t xDefaultScreen;

    // Xcb things
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_atom_t 
        atom_NET_WM_NAME,
        atom_UTF8_STRING,
        atom_WM_PROTOCOLS,
        atom_WM_DELETE_WINDOW,
        atom_NET_WM_SYNC_REQUEST,
        atom_NET_WM_SYNC_REQUEST_COUNTER;

    xcb_visualid_t windowVisualId;
    xcb_colormap_t colormapId;

};


struct FjWindow {
    /// @defgroup generic Generic fields
    /// @{
    struct FjApp *app;
    struct FjWidget *root;
    void *data;

    uint32_t width;
    uint32_t height;

    FjBackendWindowData *backendWindowData;
    /// @}

    xcb_window_t windowId;

    /// Used for synchronizing window redrawing
    /// See https://fishsoup.net/misc/wm-spec-synchronization.html
    /// Xcb has autogenerated this from an XML specification. Why no docs??
    /// I figured out how to use the counter while reading the Qt 4 code:
    /// https://dreamswork.github.io/qt4/qxcbwindow_8cpp_source.html#l00563
    /// Thanks to Qt's devs!
    xcb_sync_counter_t syncCounter;
    xcb_sync_int64_t syncValue;
};


#endif // _FEJIX_X11_H_