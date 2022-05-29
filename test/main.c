#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <fejix_runtime/fejix.h>

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
            puts("Window being closed");
            fjWindowSetShown(win, 0);
            return FJ_EXIT;
        break;
    }

    return FJ_OK;
}


int main() {
    struct FjInstance inst;
    assert(fjInstanceInit(&inst, inst_params) == FJ_OK);

    struct FjWindow win;
    assert(fjWindowInit(&inst, &win, win_params) == FJ_OK);

    assert(fjWindowSetTitle(&win, "Это работает!") == FJ_OK);

    fjWindowSetShown(&win, 1);

    struct FjWindow *windows[] = { &win };
    fjLoop(&inst, &handleEvent, windows, 1);

    fjWindowDestroy(&win);

    fjInstanceDestroy(&inst);

    return 0;
}