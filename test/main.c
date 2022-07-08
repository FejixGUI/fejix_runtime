#include <fejix_runtime/fejix.h>
#include <fejix_runtime/helper.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "debug.h"

#define arrlen(ARRAY) (sizeof(ARRAY) / sizeof(*(ARRAY)))


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


FjApp app = {0};
FjWindow win = {0};

FjAppParams appParams = {
    .backend0 = FJ_BACKEND_NANOVG,
    .backend1 = FJ_BACKEND_NONE
};

FjWindowParams winParams = {
    .width = 800,
    .height = 600,
    .minWidth = 100,
    .minHeight = 70,
    .isResizable = 1
};

int main() {
    printf(
        "Devtest is running Fejix v%d.%d.%d\n",
        FJ_VERSION_MAJOR, FJ_VERSION_MINOR, FJ_VERSION_PATCH
    );

    _(fjAppInit(&app, &appParams));

    _(fjAppInitWindow(&app, &win, &winParams));

    _(fjWindowSetTitle(&win, "Це працює!"));

    fjWindowSetVisible(&win, true);

    FjWindow *windows[] = { &win };
    fjAppSetWindows(&app, windows, arrlen(windows));
    fjLoop(&app, &handleEvent);

    fjWindowDestroy(&win);

    fjAppDestroy(&app);

    return 0;
}