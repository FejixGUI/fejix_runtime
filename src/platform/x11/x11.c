#include <fejix_runtime/fejix_runtime.h>
#include <fejix_runtime/typedefs.h>
#include <fejix_runtime/internal/platform/x11.h>

#include <fejix_runtime_dev/definitions.h>
#include <fejix_runtime_dev/x11/utils.h>

#include <string.h>
#include <malloc.h>





static
uint32_t getAtoms(
    xcb_connection_t *con, const char **names, xcb_atom_t *atoms, int length
)
{
    xcb_intern_atom_cookie_t coockies[length];

    for (int i=0; i<length; i++)
        coockies[i] = xcb_intern_atom(con, 0, strlen(names[i]), names[i]);

    for (int i=0; i<length; i++)
    {
        xcb_intern_atom_reply_t *reply;
        xcb_generic_error_t *error = NULL;
        reply = xcb_intern_atom_reply(con, coockies[i], &error);

        if (error != NULL) {
            return FJ_ERR_WMAPI_FAILED;
        }

        atoms[i] = reply->atom;
        free(reply);
    }

    return FJ_OK;
}


uint32_t fjAppInit(FjApp *app)
{        
    app->xDisplay = XOpenDisplay(NULL);

    if (!app->xDisplay)
        return FJ_ERR_WMAPI_FAILED;

    app->xDefaultScreen = XDefaultScreen(app->xDisplay);

    app->connection = XGetXCBConnection(app->xDisplay);
    if (!app->connection) {
        XCloseDisplay(app->xDisplay);
        return FJ_ERR_WMAPI_FAILED;
    }

    XSetEventQueueOwner(app->xDisplay, XCBOwnsEventQueue);

    app->screen = xcb_setup_roots_iterator(
        xcb_get_setup(app->connection)
    ).data;

    // Get some atoms
    const char *atom_names[] = {
        "_NET_WM_NAME",
        "UTF8_STRING",
        "WM_PROTOCOLS",
        "WM_DELETE_WINDOW",
        "_NET_WM_SYNC_REQUEST",
        "_NET_WM_SYNC_REQUEST_COUNTER",
    };

    int nAtoms = FJ_STATIC_LEN(atom_names);
    xcb_atom_t atoms[nAtoms];

    uint32_t result = getAtoms(app->connection, atom_names, atoms, nAtoms);
    if (result != FJ_OK)
        return result;

    app->atom_NET_WM_NAME = atoms[0];
    app->atom_UTF8_STRING = atoms[1];
    app->atom_WM_PROTOCOLS = atoms[2];
    app->atom_WM_DELETE_WINDOW = atoms[3];
    app->atom_NET_WM_SYNC_REQUEST = atoms[4];
    app->atom_NET_WM_SYNC_REQUEST_COUNTER = atoms[5];

    // This may be reassigned later by backend initializer
    app->windowVisualId = app->screen->root_visual;

    app->colormapId = xcb_generate_id(app->connection);

    xcb_create_colormap(
        app->connection,
        XCB_COLORMAP_ALLOC_NONE,
        app->colormapId,
        app->screen->root,
        app->windowVisualId
    );

    // Initialize backend
    // struct FjBackendParams backendParams = {0};
    // backendParams.app = app;
    // backendParams.backend0 = params->backend0;
    // backendParams.backend1 = params->backend1;

    // uint32_t status = fjBackendInitApp(app, &app->backend, &backendParams);

    // if (status != FJ_OK) {
    //     fjAppDestroy(app);
    //     return status;
    // }

    return FJ_OK;
}



void fjAppDestroy(struct FjApp *app)
{
    // if (app->backend.backendId != FJ_BACKEND_NONE)
    //     app->backend.destroy(&app->backend);

    xcb_disconnect(app->connection);
    // Xlib doesn't care about the opened display
    // because it was converted to an XCB connection
    // // XCloseDisplay(app->xDisplay);
}



uint32_t fjAppInitWindow(FjApp *app, FjWindow *win, FjWindowParams *params)
{
    win->app = app;

    win->windowId = xcb_generate_id(app->connection);

    uint32_t propertiesMask = XCB_CW_BACK_PIXEL
                            | XCB_CW_EVENT_MASK
                            | XCB_CW_COLORMAP;

    uint32_t properties[] = {
        app->screen->black_pixel,
        XCB_EVENT_MASK_EXPOSURE
            | XCB_EVENT_MASK_STRUCTURE_NOTIFY,
        app->colormapId
    };

    xcb_create_window(
        app->connection,
        XCB_COPY_FROM_PARENT,
        win->windowId,
        app->screen->root,
        0, 0, // X and Y, often ignored
        params->width, params->height,
        0, // border width
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        app->windowVisualId,
        propertiesMask,
        properties
    );

    win->width = params->width;
    win->height = params->height;

    fj_X11_WindowInitParams(win, params);

    xcb_atom_t protocols[] = {
        app->atom_NET_WM_SYNC_REQUEST,
        app->atom_WM_DELETE_WINDOW
    };

    xcb_change_property(
        app->connection,
        XCB_PROP_MODE_REPLACE,
        win->windowId,
        app->atom_WM_PROTOCOLS,
        XCB_ATOM_ATOM,
        FJ_SIZEOF_BITS(*protocols),
        FJ_STATIC_LEN(protocols),
        protocols
    );

    fj_X11_WindowInitSyncCounter(win);

    win->root = NULL;

    return app->backend.initWindow(&app->backend, win);
}



void fjWindowDestroy(FjWindow *win)
{
    struct FjBackend *bk = &win->app->backend;
    bk->destroyWindow(bk, win);
    fj_X11_WindowDestroySyncCounter(win);
    xcb_destroy_window(win->app->connection, win->windowId);
}



void fjWindowSetVisible(FjWindow *win, uint32_t visible)
{
    if (visible)
        xcb_map_window(win->app->connection, win->windowId);
    else
        xcb_unmap_window(win->app->connection, win->windowId);


    xcb_flush(win->app->connection);
}


uint32_t fjWindowSetTitle(FjWindow *win, const char *title)
{
    xcb_change_property(
        win->app->connection,
        XCB_PROP_MODE_REPLACE,
        win->windowId,
        win->app->atom_NET_WM_NAME,
        win->app->atom_UTF8_STRING,
        FJ_SIZEOF_BITS(*title),
        strlen(title),
        title
    );

    xcb_flush(win->app->connection);

    return FJ_OK;
}