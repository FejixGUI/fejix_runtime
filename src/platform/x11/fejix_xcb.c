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
        "UTF8_STRING"
    };

    xcb_atom_t atoms[2];

    uint32_t result = _getAtoms(inst->connection, atom_names, atoms, 2);
    if (result != FJ_OK)
        return result;

    inst->atom_NET_WM_NAME = atoms[0];
    inst->atom_UTF8_STRING = atoms[1];

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

    win->window_id = xcb_generate_id(inst->connection);

    xcb_create_window(
        inst->connection,
        XCB_COPY_FROM_PARENT,
        win->window_id,
        inst->screen->root,
        0, 0,
        width, height,
        0, // border width,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        inst->screen->root_visual,
        0, NULL // masks
    );

    

    win->inst = inst;

    return FJ_OK;
}



void fjWindowDestroy(struct FjWindow *win)
{
    xcb_destroy_window(win->inst->connection, win->window_id);
}



void fjWindowSetShown(struct FjWindow *win, uint32_t is_shown)
{
    if (is_shown)
        xcb_map_window(win->inst->connection, win->window_id);
    else
        xcb_unmap_window(win->inst->connection, win->window_id);

    xcb_flush(win->inst->connection);
}


uint32_t fjWindowSetTitle(struct FjWindow *win, const char *title)
{
    xcb_change_property(
        win->inst->connection,
        XCB_PROP_MODE_REPLACE,
        win->window_id,
        win->inst->atom_NET_WM_NAME,
        win->inst->atom_UTF8_STRING,
        8, // 8-bit characters
        strlen(title),
        (const void *) title
    );

    return FJ_OK;
}