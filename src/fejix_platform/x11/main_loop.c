#include <fejix_runtime/fejix.h>

#include <fejix_layout.h>
#include <fejix_platform/x11/window_utils.h>

#include <malloc.h>

// Why isn't it included in XCB???
#define FJ_XCB_EVENT_MASK (~0x80)



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


void fjLoop(
    struct FjInstance *inst,
    FjEventHandler handle
)
{
    struct FjBackendInstanceContext *backend = &inst->instanceContext;

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

                // if (exposeEvent->count == 0) {
                _fjLayout(win->root, win->width, win->height);
                backend->draw(backend, win, win->width, win->height);

                _fjWindowIncrSyncCounter(win);
                backend->present(backend, win);
                _fjWindowIncrSyncCounter(win);
                // }
            }
            break;

            case XCB_CONFIGURE_NOTIFY:
            {
                xcb_configure_notify_event_t *configEvent = (void *) event;
                win = findWindowById(inst, configEvent->window);
                if (!win) break;

                uint32_t W = configEvent->width;
                uint32_t H = configEvent->height;


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