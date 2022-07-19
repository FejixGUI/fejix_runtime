#include <fejix_runtime/fejix_runtime.h>
#include <fejix_runtime/typedefs.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "debug.h"
#include "fejix_runtime/internal/platform.h"

#define arrlen(ARRAY) (sizeof(ARRAY) / sizeof((ARRAY)[0]))


uint32_t handleEvent(FjWindow *win, FjEvent *ev)
{
    switch (ev->eventType)
    {
        case FJ_EVENT_CLOSE:
            puts("Did you just close the window?");
            fjWindowSetVisible(win, false);
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


FjApp app;
FjWindow win;
FjWindowParams params;


int main(void) {
    printf(
        "Devtest is running Fejix v%s\n",
        FEJIX_RUNTIME_VERSION_STRING
    );

    _(fjAppInit(&app));

    _(fjWindowParamsInit(&app, &params));
    _(fjWindowInit(&app, &win, &params));

    fj_Varsize_SetWindowSize(&win, 800, 600);
    fj_Varsize_SetWindowSizeMin(&win, 200, 150);
    fj_Varsize_SetWindowSizeMax(&win, 1000, 900);

    fjWindowSetVisible(&win, true);

    FjWindow *windows[] = { &win };
    fjAppSetWindows(&app, windows, arrlen(windows));
    fjLoop(&app, &handleEvent);

    fjWindowDestroy(&win);

    fjWindowParamsDestroy(&app, &params);
    fjAppDestroy(&app);

    return 0;
}