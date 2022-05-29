#ifndef _FEJIX_XCB_H_
#define _FEJIX_XCB_H_


#include <fejix_runtime/fejix_defines.h>

#include <xcb/xcb.h>



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