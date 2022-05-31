#ifndef _FEJIX_XCB_H_
#define _FEJIX_XCB_H_


#include <fejix_runtime/fejix_defines.h>

#include <xcb/xcb.h>

#ifdef FJ_USE_OPENGL
#   include <glad/glad_glx.h>
#endif


// Why isn't it included in XCB???
// It was so annoying to figure this out!
#define FJ_XCB_EVENT_MASK (~0x80)


struct FjInstance {
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_atom_t 
        atom_NET_WM_NAME,
        atom_UTF8_STRING,
        atom_WM_DELETE_WINDOW,
        atom_WM_PROTOCOLS;
};


struct FjWindow {
    xcb_window_t windowId;
    struct FjInstance *inst;
    void *data;
};


#endif // _FEJIX_XCB_H_