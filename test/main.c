#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include <fejix_runtime/fejix.h>


const int32_t inst_params[] = {
    FJ_IPARAM_END
};


const int32_t win_params[] = {
    FJ_WPARAM_WIDTH, 800,
    FJ_WPARAM_HEIGHT, 600,
    FJ_WPARAM_END
};


int main() {
    struct FjInstance inst;
    assert(fjInstanceInit(&inst, inst_params) == FJ_OK);

    struct FjWindow win;
    assert(fjWindowInit(&inst, &win, win_params) == FJ_OK);

    fjWindowSetTitle(&win, "Это работает!");

    fjWindowSetShown(&win, 1);

    usleep(3 * 1000000);

    fjWindowDestroy(&win);

    fjInstanceDestroy(&inst);

    return 0;
}