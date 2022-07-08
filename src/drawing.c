#include <fejix_runtime/fejix.h>

#include <fejix_private/drawing.h>
#include <fejix_private/traverse_widgets.h>

static void drawWidget(struct FjWidget *wgt, void *data)
{
    if (wgt->draw != NULL)
        wgt->draw(wgt, (struct FjDrawContext *) data);
}

uint32_t fjDraw(struct FjWindow *win)
{
    struct FjBackend *backend = &win->app->backend;

    fjTraverseWidgets(
        win->root,
        &drawWidget,
        NULL,
        backend->getWindowDrawContext(backend, win)
    );
}