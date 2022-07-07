#include <fejix_runtime/fejix_x11.h>
#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_backend.h>

#include <fejix_private_defines.h>
#include <fejix_platform/x11/window_utils.h>

#include <string.h>
#include <malloc.h>





static uint32_t getAtoms(
    xcb_connection_t *con,
    const char **names,
    xcb_atom_t *atoms,
    int length
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


uint32_t fjInstanceInit(struct FjInstance *inst, FjBackendInitializer init)
{
    if (init == NULL)
        return FJ_ERR_INVALID_PARAM;
        
    inst->xDisplay = XOpenDisplay(NULL);

    if (!inst->xDisplay)
        return FJ_ERR_WMAPI_FAILED;

    inst->xDefaultScreen = XDefaultScreen(inst->xDisplay);

    inst->connection = XGetXCBConnection(inst->xDisplay);
    if (!inst->connection) {
        XCloseDisplay(inst->xDisplay);
        return FJ_ERR_WMAPI_FAILED;
    }

    XSetEventQueueOwner(inst->xDisplay, XCBOwnsEventQueue);

    inst->screen = xcb_setup_roots_iterator(
        xcb_get_setup(inst->connection)
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

    int nAtoms = STATIC_LEN(atom_names);
    xcb_atom_t atoms[nAtoms];

    uint32_t result = getAtoms(inst->connection, atom_names, atoms, nAtoms);
    if (result != FJ_OK)
        return result;

    inst->atom_NET_WM_NAME = atoms[0];
    inst->atom_UTF8_STRING = atoms[1];
    inst->atom_WM_PROTOCOLS = atoms[2];
    inst->atom_WM_DELETE_WINDOW = atoms[3];
    inst->atom_NET_WM_SYNC_REQUEST = atoms[4];
    inst->atom_NET_WM_SYNC_REQUEST_COUNTER = atoms[5];

    // This may be reassigned later by backend initializer
    inst->windowVisualId = inst->screen->root_visual;

    inst->colormapId = xcb_generate_id(inst->connection);

    xcb_create_colormap(
        inst->connection,
        XCB_COLORMAP_ALLOC_NONE,
        inst->colormapId,
        inst->screen->root,
        inst->windowVisualId
    );

    // Initialize backend

    inst->instanceContext.backendId = FJ_BACKEND_NONE;

    struct FjBackendParams ctx = {0};
    ctx.instance = inst;

    uint32_t status = init(&ctx);

    if (status != FJ_OK) {
        fjInstanceDestroy(inst);
        return status;
    }

    return FJ_OK;
}



void fjInstanceDestroy(struct FjInstance *inst)
{
    if (inst->instanceContext.backendId != FJ_BACKEND_NONE)
        inst->instanceContext.destroy(&inst->instanceContext);

    xcb_disconnect(inst->connection);
    // Xlib doesn't care about the opened display
    // because it was converted to an XCB connection
    // XCloseDisplay(inst->xDisplay);
}



uint32_t fjIntanceInitWindow(
    struct FjInstance *inst,
    struct FjWindow *win,
    struct FjWindowParams *params
)
{
    win->instance = inst;

    win->windowId = xcb_generate_id(inst->connection);

    uint32_t propertiesMask = XCB_CW_BACK_PIXEL
                            | XCB_CW_EVENT_MASK
                            | XCB_CW_COLORMAP;

    uint32_t properties[] = {
        inst->screen->black_pixel,
        XCB_EVENT_MASK_EXPOSURE
            | XCB_EVENT_MASK_STRUCTURE_NOTIFY,
        inst->colormapId
    };

    xcb_create_window(
        inst->connection,
        XCB_COPY_FROM_PARENT,
        win->windowId,
        inst->screen->root,
        0, 0, // X and Y, often ignored
        params->width, params->height,
        0, // border width
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        inst->windowVisualId,
        propertiesMask,
        properties
    );

    win->width = params->width;
    win->height = params->height;

    _fjWindowInitParams(win, params);

    xcb_atom_t protocols[] = {
        inst->atom_NET_WM_SYNC_REQUEST,
        inst->atom_WM_DELETE_WINDOW
    };

    xcb_change_property(
        inst->connection,
        XCB_PROP_MODE_REPLACE,
        win->windowId,
        inst->atom_WM_PROTOCOLS,
        XCB_ATOM_ATOM,
        SIZEOF_BITS(*protocols),
        STATIC_LEN(protocols),
        protocols
    );

    _fjWindowInitSyncCounter(win);

    win->root = NULL;

    return inst->instanceContext.initWindow(&inst->instanceContext, win);
}



void fjWindowDestroy(struct FjWindow *win)
{
    struct FjBackendInstanceContext *bk = &win->instance->instanceContext;
    bk->destroyWindow(bk, win);
    _fjWindowDestroySyncCounter(win);
    xcb_destroy_window(win->instance->connection, win->windowId);
}



void fjWindowSetVisible(struct FjWindow *win, uint32_t visible)
{
    if (visible)
        xcb_map_window(win->instance->connection, win->windowId);
    else
        xcb_unmap_window(win->instance->connection, win->windowId);


    xcb_flush(win->instance->connection);
}


uint32_t fjWindowSetTitle(struct FjWindow *win, const char *title)
{
    xcb_change_property(
        win->instance->connection,
        XCB_PROP_MODE_REPLACE,
        win->windowId,
        win->instance->atom_NET_WM_NAME,
        win->instance->atom_UTF8_STRING,
        SIZEOF_BITS(*title),
        strlen(title),
        title
    );

    xcb_flush(win->instance->connection);

    return FJ_OK;
}