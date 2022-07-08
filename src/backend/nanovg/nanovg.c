#include <fejix_runtime/fejix.h>
#include <fejix_runtime/internal/backend.h>

#include <fejix_private/nanovg/nanovg.h>
#include <fejix_private/nanovg/drawing.h>
#include <fejix_private/opengl3_generic.h>

#include <math.h>
#include <malloc.h>

#include <nanovg.h>

#ifdef FJ_USE_OPENGL3
#   define NANOVG_GL3_IMPLEMENTATION
#endif
#include <nanovg_gl.h>

#define SELF struct FjBackend *self

static void fjBackendDestroyApp_nanovg(SELF)
{
    fjBackendDestroyApp_opengl3(self->app, self->appData);
    free(self->appData);
}

static uint32_t fjBackendInitWindow_nanovg(SELF, struct FjWindow *win)
{
    struct FjBackendWindowData_nanovg *winData;
    winData = malloc(sizeof(*winData));
    if (!winData)
        return FJ_ERR_MALLOC_FAILED;

    struct FjBackendWindowData_opengl3 *glWinData;
    glWinData = malloc(sizeof(*glWinData));
    if (!glWinData) {
        free(winData);
        return FJ_ERR_MALLOC_FAILED;
    }

    struct FjBackendAppData_opengl3 *glAppData = self->appData;
    uint32_t status = fjBackendInitWindow_opengl3(self->app, win, glAppData, glWinData);
    if (status != FJ_OK) {
        free(winData);
        free(glWinData);
        return status;
    }

    winData->glWinData = glWinData;
    winData->nvgctx = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);

    win->backendWindowData = winData;

    winData->drawContext.windowData = winData;
    fjDrawContextInit_nanovg(&winData->drawContext);

    return FJ_OK;
}


static void fjBackendDestroyWindow_nanovg(SELF, struct FjWindow *win)
{
    struct FjBackendWindowData_nanovg *winData = win->backendWindowData;

    fjBackendDestroyWindow_opengl3(win, winData->glWinData);
    nvgDeleteGL3(winData->nvgctx);

    free(winData->glWinData);
    free(winData);
}



static uint32_t fjBackendPrepareWindow_nanovg(
    SELF,
    struct FjWindow *win,
    uint32_t width,
    uint32_t height
)
{
    struct FjBackendWindowData_nanovg *winData = win->backendWindowData;

    fjBackendPrepareWindow_opengl3(win, winData->glWinData);

    glViewport(0, 0, width, height);
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);

    nvgBeginFrame(winData->nvgctx, width, height, 1);

    winData->width = width;
    winData->height = height;

    return FJ_OK;
}

void fjBackendPresentWindow_nanovg(SELF, struct FjWindow *win)
{
    struct FjBackendWindowData_nanovg  *winData = win->backendWindowData;
    nvgEndFrame(winData->nvgctx);
    fjBackendPresentWindow_opengl3(win, winData->glWinData);
}


struct FjDrawContext *
fjBackendGetWindowDrawContext_nanovg(SELF, struct FjWindow *win)
{
    struct FjBackendWindowData_nanovg  *winData = win->backendWindowData;
    return &winData->drawContext;
}


uint32_t fjBackendInitApp_nanovg(
    struct FjApp *app,
    struct FjBackend *backend,
    struct FjBackendParams *params
)
{
    uint32_t status;

    struct FjBackendAppData_opengl3 *glData;
    glData = malloc(sizeof(*glData));
    if (!glData)
        return FJ_ERR_MALLOC_FAILED;

    status = fjBackendInitApp_opengl3(app, glData, params);
    if (status != FJ_OK) {
        free(glData);
        return status;
    }

    status = fjBackendInitApp_opengl3(app, glData, params);
    if (status != FJ_OK)
        return status;

    backend->appData = glData;
    backend->backendId = FJ_BACKEND_NANOVG;
    backend->destroy = &fjBackendDestroyApp_nanovg;
    backend->initWindow = &fjBackendInitWindow_nanovg;
    backend->destroyWindow = &fjBackendDestroyWindow_nanovg;
    backend->prepareWindow = &fjBackendPrepareWindow_nanovg;
    backend->presentWindow = &fjBackendPresentWindow_nanovg;
    backend->getWindowDrawContext = &fjBackendGetWindowDrawContext_nanovg;

    return FJ_OK;
}