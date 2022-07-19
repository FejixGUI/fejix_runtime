#include <fejix_runtime/fejix_runtime.h>
#include <fejix_runtime/typedefs.h>

#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xproto.h>

#include <malloc.h>

static
FjStatus getSizeHints(FjWindow *win, xcb_size_hints_t *sizeHints)
{
    xcb_get_property_cookie_t propCoockie = xcb_icccm_get_wm_size_hints(
        win->app->connection,
        win->windowId,
        XCB_ATOM_WM_NORMAL_HINTS
    );

    xcb_generic_error_t *err = NULL;

    xcb_icccm_get_wm_size_hints_reply(
        win->app->connection,
        propCoockie,
        sizeHints,
        &err
    );

    if (err != NULL) {
        free(err);
        return FJ_ERR_WMAPI_FAILED;
    }

    return FJ_OK;
}


FjStatus fj_Varsize_SetWindowSize(
    FjWindow *win, uint32_t width, uint32_t height
)
{
    uint32_t valueList[] = {
        width,
        height
    };

    xcb_configure_window(
        win->app->connection,
        win->windowId,
        XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
        valueList
    );

    win->width = width;
    win->height = height;

    return FJ_OK;
}


FjStatus fj_Varsize_SetWindowSizeMin(
    FjWindow *win, uint32_t minWidth, uint32_t minHeight
)
{
    xcb_size_hints_t sizeHints = {0};

    if (getSizeHints(win, &sizeHints) != FJ_OK)
        return FJ_ERR_WMAPI_FAILED;
    
    xcb_icccm_size_hints_set_min_size(&sizeHints, minWidth, minHeight);
    xcb_icccm_set_wm_size_hints(
        win->app->connection,
        win->windowId,
        XCB_ATOM_WM_NORMAL_HINTS,
        &sizeHints
    );

    return FJ_OK;
}


FjStatus fj_Varsize_SetWindowSizeMax(
    FjWindow *win, uint32_t maxWidth, uint32_t maxHeight
)
{
    xcb_size_hints_t sizeHints = {0};

    if (getSizeHints(win, &sizeHints) != FJ_OK)
        return FJ_ERR_WMAPI_FAILED;
    
    xcb_icccm_size_hints_set_max_size(&sizeHints, maxWidth, maxHeight);
    xcb_icccm_set_wm_size_hints(
        win->app->connection,
        win->windowId,
        XCB_ATOM_WM_NORMAL_HINTS,
        &sizeHints
    );

    return FJ_OK;
}