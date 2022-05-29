#ifndef _FEJIX_XCB_H_
#define _FEJIX_XCB_H_


#include <fejix_runtime/fejix_defines.h>

#include <xcb/xcb.h>



struct FjInstance {
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_atom_t atom_NET_WM_NAME;
    xcb_atom_t atom_UTF8_STRING;
};


struct FjWindow {
    struct FjInstance *inst;
    xcb_window_t window_id;
};


#endif // _FEJIX_XCB_H_