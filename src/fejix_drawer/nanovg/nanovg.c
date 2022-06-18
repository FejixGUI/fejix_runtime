#include <fejix_drawer/nanovg/nanovg.h>

#include <math.h>
#define NANOVG_GL3_IMPLEMENTATION
#include <nanovg_gl.h>


uint32_t _fjDrawContextInit_nanovg(struct FjDrawContext_nanovg *ctx)
{
    ctx->nvgctx = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);
    return 0;
}


void _fjDrawContextDestroy_nanovg(struct FjDrawContext_nanovg *ctx)
{
    nvgDeleteGL3(ctx->nvgctx);
}

uint32_t _fjDraw_nanovg(struct FjDrawContext_nanovg *ctx)
{
    glViewport(0, 0, ctx->width, ctx->height);
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);

    NVGcontext *nvg = ctx->nvgctx;
    nvgBeginFrame(nvg, ctx->width, ctx->height, 1);
    nvgRect(nvg, 0, 0, 100, 100);
    nvgFillColor(nvg, (NVGcolor) {1.f, 0.5f, 0.f, 1.f});
    nvgFill(nvg);
    nvgEndFrame(nvg);

    return 0;
}