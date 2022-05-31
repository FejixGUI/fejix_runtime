#include <fejix_runtime/fejix_xcb.h>
#include <fejix_runtime/fejix.h>

#include <string.h>
#include <malloc.h>

static uint32_t _getAtoms(
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


uint32_t fjInstanceInit(struct FjInstance *inst, FjInstanceInitializer init)
{
    if (init == NULL)
        return FJ_ERR_INVALID_PARAM;
        

    inst->connection = xcb_connect(NULL, NULL);

    if (xcb_connection_has_error(inst->connection) != 0)
    {
        xcb_disconnect(inst->connection);
        return FJ_ERR_WMAPI_FAIL;
    }

    inst->screen = xcb_setup_roots_iterator(
        xcb_get_setup(inst->connection)
    ).data;

    // Get some atoms
    const char *atom_names[] = {
        "_NET_WM_NAME",
        "UTF8_STRING",
        "WM_DELETE_WINDOW",
        "WM_PROTOCOLS"
    };

    xcb_atom_t atoms[4];

    uint32_t result = _getAtoms(inst->connection, atom_names, atoms, 4);
    if (result != FJ_OK)
        return result;

    inst->atom_NET_WM_NAME = atoms[0];
    inst->atom_UTF8_STRING = atoms[1];
    inst->atom_WM_DELETE_WINDOW = atoms[2];
    inst->atom_WM_PROTOCOLS = atoms[3];

    // Initialize backend

    struct FjInstanceInitContext ctx = {0};
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
    xcb_disconnect(inst->connection);
}



uint32_t fjIntanceInitWindow(
    struct FjInstance *inst,
    struct FjWindow *win,
    const struct FjWindowParams *params
)
{
    win->inst = inst;

    win->windowId = xcb_generate_id(inst->connection);

    uint32_t propertiesMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t properties[] = {
        inst->screen->black_pixel,
        // Select ALL meaningful events
        ((uint32_t) 0x01FFFFFF)
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
        inst->screen->root_visual,
        propertiesMask,
        properties
    );

    xcb_change_property(
        inst->connection,
        XCB_PROP_MODE_REPLACE,
        win->windowId,
        inst->atom_WM_PROTOCOLS,
        XCB_ATOM_ATOM,
        sizeof(xcb_atom_t) * 8,
        1,
        &inst->atom_WM_DELETE_WINDOW
    );

    return FJ_OK;
}



void fjWindowDestroy(struct FjWindow *win)
{
    xcb_destroy_window(win->inst->connection, win->windowId);
}



void fjWindowSetShown(struct FjWindow *win, uint32_t is_shown)
{
    if (is_shown)
        xcb_map_window(win->inst->connection, win->windowId);
    else
        xcb_unmap_window(win->inst->connection, win->windowId);


    xcb_flush(win->inst->connection);
}


uint32_t fjWindowSetTitle(struct FjWindow *win, const char *title)
{
    xcb_change_property(
        win->inst->connection,
        XCB_PROP_MODE_REPLACE,
        win->windowId,
        win->inst->atom_NET_WM_NAME,
        win->inst->atom_UTF8_STRING,
        sizeof(*title) * 8,
        strlen(title),
        title
    );

    xcb_flush(win->inst->connection);

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
    struct FjInstance *inst,
    FjEventHandler handle,
    struct FjWindow **windows,
    uint32_t length
)
{
    for (;;)
    {
        xcb_generic_event_t *event = xcb_wait_for_event(inst->connection);

        if (event == NULL)
            return;

        struct FjWindow *win = NULL;
        struct FjEvent ev;
        int canHandle = 0;

        switch (event->response_type & FJ_XCB_EVENT_MASK) 
        {
            case XCB_EXPOSE:
                
            break;

            case XCB_CLIENT_MESSAGE:
            {
                xcb_client_message_event_t *client_event;
                client_event = (xcb_client_message_event_t *) event;

                win = findWindowById(windows, length, client_event->window);
                if (!win) break;

                if (client_event->data.data32[0] == inst->atom_WM_DELETE_WINDOW)
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