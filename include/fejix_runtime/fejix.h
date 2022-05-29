#ifndef _FEJIX_H_
#define _FEJIX_H_


#include <fejix_runtime/fejix_defines.h>

#ifdef FJ_USE_WINAPI
#   include <fejix_runtime/fejix_winapi.h>
#elif FJ_USE_XCB
#   include <fejix_runtime/fejix_xcb.h>
#endif



uint32_t fjInstanceInit(struct FjInstance *inst, const int32_t *params);
void fjInstanceDestroy(struct FjInstance *inst);

uint32_t fjWindowInit(
    struct FjInstance *inst,
    struct FjWindow *win,
    const int32_t *params
);
void fjWindowDestroy(struct FjWindow *win);

void fjWindowSetShown(struct FjWindow *win, uint32_t is_shown);
uint32_t fjWindowSetTitle(struct FjWindow *win, const char *title);

void fjLoop(
    struct FjInstance *inst,
    struct FjWindow **windows,
    uint32_t length
);


#endif // _FEJIX_H_