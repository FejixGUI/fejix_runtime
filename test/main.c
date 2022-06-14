#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_helper.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "debug.h"

#ifndef __cplusplus
#   define true 1
#   define false 0
#endif

#define arrlen(ARRAY) (sizeof(ARRAY) / sizeof(*(ARRAY)))


uint32_t initBackend(FjBackendParams *ctx)
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
        {
            char title[32];
            sprintf(
                title,
                "%" PRIu32 "x" "%" PRIu32,
                ev->resizeEvent.width,
                ev->resizeEvent.height
            );
            fjWindowSetTitle(win, title);
        }
        break;
    }

    return FJ_OK;
}


FjInstance inst = {0};
FjWindow win = {0};



int main() {
    _(fjInstanceInit(&inst, &initBackend));

    FjWindowParams wparams = {
        .width = 800,
        .height = 600,
        .minWidth = 100,
        .minHeight = 70,
        .isResizable = 1
    };

    _(fjIntanceInitWindow(&inst, &win, &wparams));

    _(fjWindowSetTitle(&win, "Це працює!"));

    fjWindowSetVisible(&win, true);

    FjWindow *windows[] = { &win };
    fjInstanceSetWindows(&inst, windows, arrlen(windows));
    fjLoop(&inst, &handleEvent);

    fjWindowDestroy(&win);

    fjInstanceDestroy(&inst);

    return 0;
}