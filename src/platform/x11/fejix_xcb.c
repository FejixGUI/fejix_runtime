#include <fejix_runtime/fejix_xcb.h>

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


uint32_t fjInstanceInit(struct FjInstance *inst, const int32_t *params)
{
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

    return FJ_OK;
}



void fjInstanceDestroy(struct FjInstance *inst)
{
    xcb_disconnect(inst->connection);
}



uint32_t fjWindowInit(
    struct FjInstance *inst,
    struct FjWindow *win,
    const int32_t *params
)
{
    uint32_t width = 640, height = 480;

    for (int i=0; i<FJ_PARAM_MAX; i++)
    {
        switch (params[i]) {
        case FJ_WPARAM_END:
            i = FJ_PARAM_MAX;
            continue;

        case FJ_WPARAM_WIDTH:
            width = params[++i];
            continue;

        case FJ_WPARAM_HEIGHT:
            height = params[++i];
            continue;
        }
    }

    win->windowId = xcb_generate_id(inst->connection);

    uint32_t propertiesMask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t properties[] = {
        inst->screen->black_pixel,
        ((uint32_t) 0x01FFFFFF) // Select ALL meaningful events
    };

    xcb_create_window(
        inst->connection,
        XCB_COPY_FROM_PARENT,
        win->windowId,
        inst->screen->root,
        0, 0, // X and Y, often ignored
        width, height,
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
        32,
        1,
        &inst->atom_WM_DELETE_WINDOW
    );

    win->inst = inst;

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
        8, // 8-bit characters
        strlen(title),
        (const void *) title
    );

    xcb_flush(win->inst->connection);

    return FJ_OK;
}


void fjLoop(
    struct FjInstance *inst,
    struct FjWindow **windows,
    uint32_t length
)
{
    xcb_generic_event_t *event;

    while ((event = xcb_wait_for_event(inst->connection)))
    {
        switch (event->response_type)
        {
                        case XCB_CLIENT_MESSAGE:
            {
                xcb_client_message_event_t *client_event;
                client_event = (xcb_client_message_event_t *) event;

                if (client_event->data.data32[0] == inst->atom_WM_DELETE_WINDOW)
                    return;
            }
            break;

            default: break;
        }
    }
}