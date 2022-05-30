#include <stdio.h>
#include <stdlib.h>

#include <fejix_runtime/fejix.h>
#include "myassert.h"

const int32_t inst_params[] = {
    FJ_IPARAM_END
};


const int32_t win_params[] = {
    FJ_WPARAM_WIDTH, 800,
    FJ_WPARAM_HEIGHT, 600,
    FJ_WPARAM_END
};


uint32_t handleEvent(struct FjWindow *win, struct FjEvent *ev)
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

struct FjInstance inst = {0};
struct FjWindow win = {0};

int main() {
    massert(fjInstanceInit(&inst, inst_params) == FJ_OK);

    massert(fjWindowInit(&inst, &win, win_params) == FJ_OK);

    massert(fjWindowSetTitle(&win, "Це працює!") == FJ_OK);

    win.inst = &inst;
    fjWindowSetShown(&win, 1);

    struct FjWindow *windows[] = { &win };
    fjLoop(&inst, &handleEvent, windows, 1);

    fjWindowDestroy(&win);

    fjInstanceDestroy(&inst);

    return 0;
}