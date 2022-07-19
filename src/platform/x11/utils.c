#include <fejix_runtime/fejix_runtime.h>
#include <fejix_runtime/typedefs.h>

#include <fejix_runtime_dev/definitions.h>
#include <fejix_runtime_dev/x11/utils.h>



void fj_X11_WindowInitSyncCounter(struct FjWindow *win)
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
        FJ_SIZEOF_BITS(win->syncCounter),
        1,
        &win->syncCounter
    );
    xcb_flush(win->app->connection);
}



void fj_X11_WindowDestroySyncCounter(struct FjWindow *win)
{
    xcb_sync_destroy_counter(win->app->connection, win->syncCounter);
}



void fj_X11_WindowIncrSyncCounter(struct FjWindow *win)
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