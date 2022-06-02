#include <fejix_runtime/fejix_x11.h>
#include <fejix_runtime/fejix.h>

#include "fejix_backend_wrapper.h"

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
            return FJ_ERR_WMAPI_FAIL;
        }

        atoms[i] = reply->atom;
        free(reply);
    }

    return FJ_OK;
}


uint32_t fjInstanceInit(struct FjInstance *instance, FjBackendInitializer init)
{
    if (init == NULL)
        return FJ_ERR_INVALID_PARAM;
        
    instance->xDisplay = XOpenDisplay(NULL);

    if (!instance->xDisplay)
        return FJ_ERR_WMAPI_FAIL;

    instance->xDefaultScreen = XDefaultScreen(instance->xDisplay);

    /* instance->connection = xcb_connect(NULL, NULL);

    if (xcb_connection_has_error(instance->connection) != 0)
    {
        xcb_disconnect(instance->connection);
        return FJ_ERR_WMAPI_FAIL;
    } */

    instance->connection = XGetXCBConnection(instance->xDisplay);
    if (!instance->connection) {
        XCloseDisplay(instance->xDisplay);
        return FJ_ERR_WMAPI_FAIL;
    }

    XSetEventQueueOwner(instance->xDisplay, XCBOwnsEventQueue);

    instance->screen = xcb_setup_roots_iterator(
        xcb_get_setup(instance->connection)
    ).data;

    // Get some atoms
    const char *atom_names[] = {
        "_NET_WM_NAME",
        "UTF8_STRING",
        "WM_DELETE_WINDOW",
        "WM_PROTOCOLS"
    };

    xcb_atom_t atoms[4];

    uint32_t result = getAtoms(instance->connection, atom_names, atoms, 4);
    if (result != FJ_OK)
        return result;

    instance->atom_NET_WM_NAME = atoms[0];
    instance->atom_UTF8_STRING = atoms[1];
    instance->atom_WM_DELETE_WINDOW = atoms[2];
    instance->atom_WM_PROTOCOLS = atoms[3];

    // This may be reassigned later by backend initializer
    instance->windowVisualId = instance->screen->root_visual;

    instance->colormapId = xcb_generate_id(instance->connection);

    xcb_create_colormap(
        instance->connection,
        XCB_COLORMAP_ALLOC_NONE,
        instance->colormapId,
        instance->screen->root,
        instance->windowVisualId
    );

    // Initialize backend

    struct FjBackendInitContext ctx = {0};
    ctx.instance = instance;

    uint32_t status = init(&ctx);

    if (status != FJ_OK) {
        fjInstanceDestroy(instance);
        return status;
    }

    return FJ_OK;
}



void fjInstanceDestroy(struct FjInstance *instance)
{
    fjBackendDestroy(instance);

    xcb_disconnect(instance->connection);
    // Xlib doesn't care about the opened display
    // because it was converted to an XCB connection
    // XCloseDisplay(instance->xDisplay);
}



uint32_t fjIntanceInitWindow(
    struct FjInstance *instance,
    struct FjWindow *win,
    const struct FjWindowParams *params
)
{
    win->instance = instance;

    win->windowId = xcb_generate_id(instance->connection);

    uint32_t propertiesMask = XCB_CW_BACK_PIXEL
                            | XCB_CW_EVENT_MASK
                            | XCB_CW_COLORMAP;

    uint32_t properties[] = {
        instance->screen->black_pixel,
        (uint32_t) 0x01FFFFFF, // Select ALL meaningful events
        instance->colormapId
    };

    xcb_create_window(
        instance->connection,
        XCB_COPY_FROM_PARENT,
        win->windowId,
        instance->screen->root,
        0, 0, // X and Y, often ignored
        params->width, params->height,
        0, // border width
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        instance->windowVisualId,
        propertiesMask,
        properties
    );

    xcb_change_property(
        instance->connection,
        XCB_PROP_MODE_REPLACE,
        win->windowId,
        instance->atom_WM_PROTOCOLS,
        XCB_ATOM_ATOM,
        sizeof(xcb_atom_t) * 8,
        1,
        &instance->atom_WM_DELETE_WINDOW
    );

    // fjWindowSetShown(win, 1);

    return _fjBackendInitWindow(win);
}



void fjWindowDestroy(struct FjWindow *win)
{
    _fjBackendDestroyWindow(win);
    xcb_destroy_window(win->instance->connection, win->windowId);
}



void fjWindowSetShown(struct FjWindow *win, uint32_t shown)
{
    if (shown)
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
        sizeof(*title) * 8,
        strlen(title),
        title
    );

    xcb_flush(win->instance->connection);

    return FJ_OK;
}


/// Returns: pointer to the window from the list that has the specified ID.
// If such a window was not found, returns NULL
static struct FjWindow* findWindowById(
    struct FjWindow **windows,
    int length,
    xcb_window_t id
)
{
    for (int i=0; i<length; i++)
        if (windows[i]->windowId == id)
            return windows[i];

    return NULL;
}


void fjLoop(
    struct FjInstance *instance,
    FjEventHandler handle,
    struct FjWindow **windows,
    uint32_t length
)
{
    for (;;)
    {
        xcb_generic_event_t *event = xcb_wait_for_event(instance->connection);

        if (event == NULL)
            return;

        struct FjWindow *win = NULL;
        struct FjEvent ev;
        int canHandle = 0;

        switch (event->response_type & FJ_XCB_EVENT_MASK) 
        {
            case XCB_EXPOSE:
            {
                xcb_expose_event_t *expose_event;
                expose_event = (xcb_expose_event_t *) event;
                win = findWindowById(windows, length, expose_event->window);
                _fjWindowBeginDrawing(win);
                glClearColor(1.0, 0.7, 0.0, 1.0);
                glClear(GL_COLOR_BUFFER_BIT);
                _fjWindowPresentDrawing(win);
                _fjWindowEndDrawing(win);
            }
            break;

            case XCB_CLIENT_MESSAGE:
            {
                xcb_client_message_event_t *client_event;
                client_event = (xcb_client_message_event_t *) event;

                win = findWindowById(windows, length, client_event->window);
                if (!win) break;

                if (client_event->data.data32[0] == instance->atom_WM_DELETE_WINDOW)
                {
                    ev.eventType = FJ_EVENT_CLOSE;
                    canHandle = 1;
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