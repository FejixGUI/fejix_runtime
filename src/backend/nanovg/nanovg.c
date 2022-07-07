#include <fejix_runtime/fejix.h>
#include <fejix_runtime/internal/backend.h>

#include <fejix_private/nanovg/nanovg.h>
#include <fejix_private/opengl3_generic.h>

#include <math.h>
#include <malloc.h>

#include <nanovg.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>

struct FjBackendWindowData_nanovg {
    struct FjBackendWindowData_opengl3 *glData;

    struct NVGcontext *nvgctx;
    uint32_t width;
    uint32_t height;
};



static void fjBackendDestroyApp_nanovg(struct FjBackend *self)
{
    free(self->appData);
}

static uint32_t fjBackendInitWindow_nanovg(struct FjBackend *self, struct FjWindow *win)
{
    struct FjBackendWindowData_nanovg *winData;
    winData = malloc(sizeof(*winData));
    if (!winData)
        return FJ_ERR_MALLOC_FAILED;

    struct FjBackendWindowData_opengl3 *glData;
    glData = malloc(sizeof(*glData));
    if (!glData) {
        free(winData);
        return FJ_ERR_MALLOC_FAILED;
    }

    winData->glData = glData;
    winData->nvgctx = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);

    return FJ_OK;
}


static void fjBackendDestroyWindow_nanovg(struct FjBackend *self, struct FjWindow *win)
{
    struct FjBackendWindowData_nanovg *winData = win->backendWindowData;

    nvgDeleteGL3(winData->nvgctx);

    free(winData->glData);
    free(winData);
}



static uint32_t fjBackendDrawWindow_nanovg(
    struct FjBackend *self,
    struct FjWindow *win,
    uint32_t width,
    uint32_t height
)
{
    glViewport(0, 0, width, height);
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);

    struct FjBackendWindowData_nanovg *winData = win->backendWindowData;

    NVGcontext *nvg = winData->nvgctx;
    nvgBeginFrame(nvg, width, height, 1);
    nvgRect(nvg, 0, 0, 100, 100);
    nvgFillColor(nvg, (NVGcolor) {1.f, 0.5f, 0.f, 1.f});
    nvgFill(nvg);
    nvgEndFrame(nvg);

    return FJ_OK;
}

void fjBackendPresentWindow_nanovg(struct FjBackend *self, struct FjWindow *win)
{
    fjBackendPresentWindow_opengl3(win, win->backendWindowData);
}


uint32_t fjBackendInitApp_nanovg(struct FjApp *app, struct FjBackend *backend, struct FjBackendParams *params)
{
    struct FjBackendAppData_opengl3 *glData;
    glData = malloc(sizeof(*glData));
    if (!glData)
        return FJ_ERR_MALLOC_FAILED;

    uint32_t status = fjBackendInitApp_opengl3(app, glData, params);
    if (status != FJ_OK)
        return status;

    backend->appData = glData;
    backend->backendId = FJ_BACKEND_NANOVG;
    backend->destroy = &fjBackendDestroyApp_nanovg;
    backend->initWindow = &fjBackendInitWindow_nanovg;
    backend->destroyWindow = &fjBackendDestroyWindow_nanovg;
    backend->drawWindow = &fjBackendDrawWindow_nanovg;
    backend->presentWindow = &fjBackendPresentWindow_nanovg;

    return FJ_OK;
}