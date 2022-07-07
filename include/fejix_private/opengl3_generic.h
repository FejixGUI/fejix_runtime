#ifndef _FEJIX_PRIVATE_OPENGL3_H_
#define _FEJIX_PRIVATE_OPENGL3_H_

#include <fejix_runtime/definitions.h>

#ifdef FJ_USE_X11
#   include <fejix_private/x11/opengl3_defs.h> 
#endif

struct FjApp;
struct FjWindow;

uint32_t fjBackendInitApp_opengl3(
    struct FjApp *app,
    struct FjBackendAppData_opengl3 *appData,
    struct FjBackendParams *params
);

void fjBackendDestroyApp_opengl3(
    struct FjApp *app,
    struct FjBackendAppData_opengl3 *appData
);

uint32_t fjBackendInitWindow_opengl3(
    struct FjApp *app,
    struct FjWindow *win,
    struct FjBackendAppData_opengl3 *appData,
    struct FjBackendWindowData_opengl3 *winData
);

void fjBackendDestroyWindow_opengl3(
    struct FjWindow *win,
    struct FjBackendWindowData_opengl3 *winData
);

void fjBackendPrepareWindow_opengl3(
    struct FjWindow *win,
    struct FjBackendWindowData_opengl3 *winData
);

void fjBackendPresentWindow_opengl3(
    struct FjWindow *win,
    struct FjBackendWindowData_opengl3 *winData
);

#endif // _FEJIX_PRIVATE_OPENGL3_H_