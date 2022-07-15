#include <fejix_runtime/fejix.h>

#include <fejix_private/definitions.h>
#include <fejix_private/x11/window_utils.h>

void fjWindowInitParams_x11(struct FjWindow *win, struct FjWindowParams *params)
{
    uint32_t
        minW = params->constraints.minW,
        minH = params->constraints.minH,
        maxW = params->constraints.maxW,
        maxH = params->constraints.maxH;

    if (!params->isResizable) {
        minW = win->width;
        minH = win->height;
        maxW = win->width;
        maxH = win->height;
    }

    xcb_size_hints_t sizeHints = {0};
    if (minW != 0 || minH != 0)
        xcb_icccm_size_hints_set_min_size(
            &sizeHints,
            minW,
            minH
        );

    if (maxW != 0 || maxH != 0)
        xcb_icccm_size_hints_set_max_size(
            &sizeHints,
            maxW,
            maxH
        );

    xcb_icccm_set_wm_size_hints(
        win->app->connection,
        win->windowId,
        XCB_ATOM_WM_NORMAL_HINTS,
        &sizeHints
    );
}



void fjWindowInitSyncCounter_x11(struct FjWindow *win)
{
    win->syncCounter = xcb_generate_id(win->app->connection);
    win->syncValue = (xcb_sync_int64_t) { 0, 0 };
    xcb_sync_create_counter(
        win->app->connection,
        win->syncCounter,
        win->syncValue
    );

    xcb_change_property(
        win->app->connection,
        XCB_PROP_MODE_REPLACE,
        win->windowId,
        win->app->atom_NET_WM_SYNC_REQUEST_COUNTER,
        XCB_ATOM_CARDINAL,
        SIZEOF_BITS(win->syncCounter),
        1,
        &win->syncCounter
    );
    xcb_flush(win->app->connection);
}



void fjWindowDestroySyncCounter_x11(struct FjWindow *win)
{
    xcb_sync_destroy_counter(win->app->connection, win->syncCounter);
}



void fjWindowIncrSyncCounter_x11(struct FjWindow *win)
{
    int64_t *val = (void *) &win->syncValue; // Yay! Unsafe code!
    val++;

    xcb_sync_set_counter(
        win->app->connection,
        win->syncCounter,
        win->syncValue
    );

    xcb_flush(win->app->connection);
}