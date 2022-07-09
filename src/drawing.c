#include <fejix_runtime/fejix.h>

#include <fejix_private/drawing.h>
#include <fejix_private/traverse_widgets.h>

static int drawWidget(struct FjWidget *wgt, int down, void *data)
{
    if (wgt->layout == NULL)
        return 1; // Do not draw the widget and its content

    if (wgt->draw != NULL)
        if (down)
            wgt->draw(wgt, (struct FjDrawContext *) data);

    return 0; // Continue traversal
}

uint32_t fjDraw(struct FjWindow *win)
{
    struct FjBackend *backend = &win->app->backend;

    fjTraverseWidgets(
        win->root,
        &drawWidget,
        backend->getWindowDrawContext(backend, win)
    );
}