#ifndef _FEJIX_DRAWER_NANOVG_H_
#define _FEJIX_DRAWER_NANOVG_H_

#include <fejix_runtime/defines.h>

#include <glad/gl.h>

#include <nanovg.h>

struct FjDrawContext_nanovg {
    struct NVGcontext *nvgctx;
    uint32_t width;
    uint32_t height;
};

uint32_t _fjDrawContextInit_nanovg(struct FjDrawContext_nanovg *ctx);
void _fjDrawContextDestroy_nanovg(struct FjDrawContext_nanovg *ctx);

uint32_t _fjDraw_nanovg(struct FjDrawContext_nanovg *ctx);

#endif // _FEJIX_DRAWER_NANOVG_H_