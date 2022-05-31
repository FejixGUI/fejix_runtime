#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_helper.h>

#include <stdio.h>
#include <stdlib.h>

#include "myassert.h"


uint32_t initInstance(FjBackendInitContext *ctx)
{
    return FJ_OK;
}


uint32_t handleEvent(FjWindow *win, FjEvent *ev)
{
    switch (ev->eventType)
    {
        case FJ_EVENT_CLOSE:
            puts("Did you just close the window?");
            fjWindowSetShown(win, 0);
            return FJ_EXIT;
        break;
    }

    return FJ_OK;
}


int main() {
    
    FjInstance inst = {0};
    FjWindow win = {0};

    massert(fjInstanceInit(&inst, &initInstance) == FJ_OK);

    FjWindowParams wparams = {
        .width = 800,
        .height = 600
    };

    massert(fjIntanceInitWindow(&inst, &win, &wparams) == FJ_OK);

    massert(fjWindowSetTitle(&win, "Це працює!") == FJ_OK);

    fjWindowSetShown(&win, 1);

    FjWindow *windows[] = { &win };
    fjLoop(&inst, &handleEvent, windows, 1);

    fjWindowDestroy(&win);

    fjInstanceDestroy(&inst);

    return 0;
}