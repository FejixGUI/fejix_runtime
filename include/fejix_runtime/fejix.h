#ifndef _FEJIX_H_
#define _FEJIX_H_


#include <fejix_runtime/fejix_defines.h>

#ifdef FJ_USE_WINAPI
#   include <fejix_runtime/fejix_winapi.h>
#elif FJ_USE_X11
#   include <fejix_runtime/fejix_xcb.h>
#endif

#ifdef FJ_USE_OPENGL
#   include <glad/glad.h>
#endif


struct FjEvent {
    uint32_t eventType;
    
    /* union {

    }; */
};


struct FjWindowParams {
    uint32_t width;
    uint32_t height;

    // unsigned resizable: 1;
};


struct FjInstanceInitContext {
    struct FjInstance *instance;
    uint32_t backend;
};

typedef uint32_t (*FjInstanceInitializer) (
    struct FjInstanceInitContext *initCtx
);


uint32_t fjInstanceInit(struct FjInstance *inst, FjInstanceInitializer init);
void fjInstanceDestroy(struct FjInstance *inst);

uint32_t fjIntanceInitWindow(
    struct FjInstance *inst,
    struct FjWindow *win,
    const struct FjWindowParams *params
);
void fjWindowDestroy(struct FjWindow *win);

void fjWindowSetShown(struct FjWindow *win, uint32_t is_shown);
uint32_t fjWindowSetTitle(struct FjWindow *win, const char *title);

typedef uint32_t (*FjEventHandler)(struct FjWindow *win, struct FjEvent *ev);

void fjLoop(
    struct FjInstance *inst,
    FjEventHandler handler,
    struct FjWindow **windows,
    uint32_t length
);


#endif // _FEJIX_H_