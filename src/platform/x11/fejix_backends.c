#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_x11.h>

#include "fejix_backends.h"


uint32_t fjBackendInit(struct FjBackendInitContext *ctx)
{
    switch (ctx->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                return _fjBackendInit_gl3(ctx);
#           else
                return FJ_ERR_FEATURE_NOT_COMPILED;
#           endif
        break;

        default:
            return FJ_ERR_BACKEND_UNKNOWN;
    }

    return FJ_OK;
}


void fjBackendDestroy(struct FjInstance *inst)
{
    switch (inst->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                _fjBackendDestroy_gl3(inst);
#           else                
                return FJ_ERR_FEATURE_NOT_COMPILED;
#           endif
        break;
    }
}


uint32_t _fjBackendInitWindow(struct FjWindow *win)
{
    switch (win->inst->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                _fjBackendInitWindow_gl3(win);
#           endif
        break;

        default:
            return FJ_ERR_BACKEND_UNKNOWN;
    }
}


void _fjBackendDestroyWindow(struct FjWindow *win)
{
    switch (win->inst->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                _fjBackendDestroyWindow_gl3(win);
#           endif
        break;

        default: return;
    }
}


uint32_t _fjWindowBeginDrawing(struct FjWindow *win)
{
    switch (win->inst->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                return _fjWindowBeginDrawing_gl3(win);
#           endif
        break;

        default: break;
    }

    return FJ_OK;
}


void _fjWindowEndDrawing(struct FjWindow *win)
{
    switch (win->inst->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                _fjWindowEndDrawing_gl3(win);
#           endif
        break;

        default: return;
    }
}


uint32_t _fjWindowPresentDrawing(struct FjWindow *win)
{
    switch (win->inst->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                return _fjWindowPresentDrawing_gl3(win);
#           endif
        break;

        default: break;
    }

    return FJ_OK;
}