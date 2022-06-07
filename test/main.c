#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_helper.h>

#include <stdio.h>
#include <stdlib.h>

#include "debug.h"


uint32_t initBackend(FjBackendInitContext *ctx)
{
    ctx->backend = FJ_BACKEND_OPENGL3;

    return fjBackendInit(ctx);
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
        .height = 600
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