#ifndef FEJIX_RUNTIME_OPENGL_H_
#define FEJIX_RUNTIME_OPENGL_H_


#include <fejix_runtime/definitions.h>


struct Fj_Opengl_AppData;
struct Fj_Opengl_WindowData;


uint32_t fj_Opengl_AppDataInit(
    struct FjApp *app,
    struct Fj_Opengl_AppData *appData
);

/// @param params Must be initialised with fjAppInitWindowParams
uint32_t fj_Opengl_EditWindowParams(
    struct FjApp *app,
    struct Fj_Opengl_AppData *appData,
    struct FjWindowParams *params
);

uint32_t fj_Opengl_WindowDataInit(
    struct FjApp *app,
    struct FjWindow *win,
    struct Fj_Opengl_WindowData *winData
);

/// Makes OpenGL context current
void fj_Opengl_PrepareWindow(
    struct FjApp *app,
    struct FjWindow *win,
    struct Fj_Opengl_AppData *appData,
    struct Fj_Opengl_WindowData *winData
);

/// Swaps frame buffers
void fj_Opengl_PresentWindow(
    struct FjApp *app,
    struct FjWindow *win,
    struct Fj_Opengl_AppData *appData,
    struct Fj_Opengl_WindowData *winData
);


void fj_Opengl_WindowDataDestroy(
    struct FjApp *app,
    struct FjWindow *win,
    struct Fj_Opengl_AppData *appData,
    struct Fj_Opengl_WindowData *winData
);

void fj_Opengl_AppDataDestroy(
    struct FjApp *app,
    struct Fj_Opengl_AppData *appData
);


#endif // FEJIX_RUNTIME_OPENGL_H_