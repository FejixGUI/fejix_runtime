#include <fejix_runtime/fejix_x11.h>
#include <fejix_runtime/fejix.h>

#include "fejix_backend_wrapper.h"

#include <string.h>
#include <malloc.h>

#include <stdio.h>


/// Length of a static array
#define LEN(ARR) (sizeof(ARR) / sizeof(*(ARR)))

#define SIZEOF_BITS(THING) (sizeof(THING) * 8)



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
            return FJ_ERR_WMAPI_FAIL;
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
        return FJ_ERR_WMAPI_FAIL;

    inst->xDefaultScreen = XDefaultScreen(inst->xDisplay);

    /* inst->connection = xcb_connect(NULL, NULL);

    if (xcb_connection_has_error(inst->connection) != 0)
    {
        xcb_disconnect(inst->connection);
        return FJ_ERR_WMAPI_FAIL;
    } */

    inst->connection = XGetXCBConnection(inst->xDisplay);
    if (!inst->connection) {
        XCloseDisplay(inst->xDisplay);
        return FJ_ERR_WMAPI_FAIL;
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

    int nAtoms = LEN(atom_names);
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

    struct FjBackendInitContext ctx = {0};
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
    fjBackendDestroy(inst);

    xcb_disconnect(inst->connection);
    // Xlib doesn't care about the opened display
    // because it was converted to an XCB connection
    // XCloseDisplay(inst->xDisplay);
}



uint32_t fjIntanceInitWindow(
    struct FjInstance *inst,
    struct FjWindow *win,
    const struct FjWindowParams *params
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
        LEN(protocols),
        protocols
    );

    win->syncCounter = xcb_generate_id(inst->connection);
    win->syncValue = (xcb_sync_int64_t) { 0, 0 };
    xcb_sync_create_counter(
        inst->connection,
        win->syncCounter,
        win->syncValue
    );

    xcb_change_property(
        win->instance->connection,
        XCB_PROP_MODE_REPLACE,
        win->windowId,
        win->instance->atom_NET_WM_SYNC_REQUEST_COUNTER,
        XCB_ATOM_CARDINAL,
        SIZEOF_BITS(win->syncCounter),
        1,
        &win->syncCounter
    );
    xcb_flush(inst->connection);

    return _fjBackendInitWindow(win);
}



void fjWindowDestroy(struct FjWindow *win)
{
    _fjBackendDestroyWindow(win);
    xcb_sync_destroy_counter(win->instance->connection, win->syncCounter);
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


/// Returns: pointer to the window from the list that has the specified ID.
// If such a window was not found, returns NULL
static struct FjWindow* findWindowById(
    struct FjInstance *inst,
    xcb_window_t id
)
{
    for (int i=0; i<inst->windowsLen; i++)
        if (inst->windows[i]->windowId == id)
            return inst->windows[i];

    return NULL;
}


static void incrSyncCounter(struct FjWindow *win)
{
    int64_t *val = (void *) &win->syncValue; // Yay! Unsafe code!
    val++;

    xcb_sync_set_counter(
        win->instance->connection,
        win->syncCounter,
        win->syncValue
    );

    xcb_flush(win->instance->connection);
}


void fjLoop(
    struct FjInstance *inst,
    FjEventHandler handle
)
{
    for (;;)
    {
        xcb_generic_event_t *event = xcb_wait_for_event(inst->connection);

        if (event == NULL)
            return;

        struct FjWindow *win = NULL;
        struct FjEvent ev = {0};
        int canHandle = 0;
        
        switch (event->response_type & FJ_XCB_EVENT_MASK) 
        {
            case XCB_EXPOSE:
            {
                xcb_expose_event_t *exposeEvent = (void *) event;
                win = findWindowById(inst, exposeEvent->window);
                if (!win) break;

                incrSyncCounter(win);
                _fjDrawPresent(win);
                incrSyncCounter(win);
            }
            break;

            case XCB_CONFIGURE_NOTIFY:
            {
                xcb_configure_notify_event_t *configEvent = (void *) event;
                win = findWindowById(inst, configEvent->window);
                if (!win) break;

                uint32_t W = configEvent->width;
                uint32_t H = configEvent->height;

                _fjDrawBegin(win, win->width, win->height);
                // TODO draw the window here
                _fjDrawEnd(win);

                if (win->width != W || win->height != H) {
                    win->width = W;
                    win->height = H;
                    ev.eventType = FJ_EVENT_RESIZE;
                    ev.resizeEvent.width = W;
                    ev.resizeEvent.height = H;
                    canHandle = 1;
                }

            }
            break;

            case XCB_CLIENT_MESSAGE:
            {
                xcb_client_message_event_t *clientEvent = (void *) event;
                uint32_t *data = clientEvent->data.data32;

                win = findWindowById(inst, clientEvent->window);
                if (!win) break;
                
                uint32_t msgType = data[0];
                if (msgType == inst->atom_WM_DELETE_WINDOW)
                {
                    ev.eventType = FJ_EVENT_CLOSE;
                    canHandle = 1;
                }
                else if (msgType == inst->atom_NET_WM_SYNC_REQUEST)
                {
                    win->syncValue.lo = data[2];
                    win->syncValue.hi = data[3];
                }

            }
            break;

            default: break;
        }

        free(event);

        if (canHandle && win != NULL) {
            uint32_t response = handle(win, &ev);

            if (response == FJ_EXIT)
                return;
        }
    }
}