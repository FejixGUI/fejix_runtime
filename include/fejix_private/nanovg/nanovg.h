#ifndef _FEJIX_PRIVATE_NANOVG_H_
#define _FEJIX_PRIVATE_NANOVG_H_

#include <fejix_runtime/definitions.h>
#include <fejix_runtime/internal/backend.h>

#include <fejix_private/opengl/opengl3_generic.h>

#include <nanovg.h>

struct FjBackendWindowData_nanovg {
    struct FjDrawContext drawContext;

    struct FjBackendWindowData_opengl3 *glWinData;
    struct NVGcontext *nvgctx;

    uint32_t width;
    uint32_t height;
    NVGcolor color;
};

uint32_t fjBackendInitApp_nanovg(
    struct FjApp *app,
    struct FjBackend *backend, 
    struct FjBackendParams *params
);

#endif // _FEJIX_PRIVATE_NANOVG_H_