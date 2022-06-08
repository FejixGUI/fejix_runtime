#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_helper.h>

#include <stdio.h>
#include <stdlib.h>

#include "debug.h"


uint32_t initBackend(FjBackendInitContext *ctx)
{
    ctx->backend = FJ_BACKEND_OPENGL3;

    uint32_t status = fjBackendInit(ctx);

    if (status != FJ_OK)
        puts("Error: cannot initialize OpenGL 3!");

    return status;
}


uint32_t handleEvent(FjWindow *win, FjEvent *ev)
{
    switch (ev->eventType)
    {
        case FJ_EVENT_CLOSE:
            puts("Did you just close the window?");
            fjWindowSetVisible(win, 0);
            return FJ_EXIT;
        break;

        case FJ_EVENT_RESIZE:
            // The following code makes resizing jerky
            printf(
                "I am resized: %dx%d\n",
                ev->resizeEvent.width,
                ev->resizeEvent.height
            );
        break;
    }

    return FJ_OK;
}


int main() {
    
    FjInstance inst = {0};
    FjWindow win = {0};

    _(fjInstanceInit(&inst, &initBackend));

    FjWindowParams wparams = {
        .width = 800,
        .height = 600,
        // .minWidth = 100,
        // .minHeight = 70,
        // .maxWidth = 1000,
        // .maxHeight = 900,
        .isResizable = 0
    };

    _(fjIntanceInitWindow(&inst, &win, &wparams));

    _(fjWindowSetTitle(&win, "Це працює!"));

    fjWindowSetVisible(&win, 1);

    FjWindow *windows[] = { &win };
    fjInstanceSetWindows(&inst, windows, 1);
    fjLoop(&inst, &handleEvent);

    fjWindowDestroy(&win);

    fjInstanceDestroy(&inst);

    return 0;
}