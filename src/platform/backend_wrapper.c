#include <fejix_runtime/fejix.h>

#include <platform/backend_wrapper.h>


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


void fjBackendDestroy(struct FjInstance *instance)
{
    switch (instance->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                _fjBackendDestroy_gl3(instance);
#           else                
                return FJ_ERR_FEATURE_NOT_COMPILED;
#           endif
        break;
    }
}


uint32_t _fjBackendInitWindow(struct FjWindow *win)
{
    switch (win->instance->backend)
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
    switch (win->instance->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                _fjBackendDestroyWindow_gl3(win);
#           endif
        break;

        default: return;
    }
}


uint32_t _fjDrawBegin(struct FjWindow *win, uint32_t width, uint32_t height)
{
    switch (win->instance->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                return _fjDrawBegin_gl3(win, width, height);
#           endif
        break;

        default: break;
    }

    return FJ_OK;
}


void _fjDrawEnd(struct FjWindow *win)
{
    switch (win->instance->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                _fjDrawEnd_gl3(win);
#           endif
        break;

        default: return;
    }
}


uint32_t _fjDrawPresent(struct FjWindow *win)
{
    switch (win->instance->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                return _fjDrawPresent_gl3(win);
#           endif
        break;

        default: break;
    }

    return FJ_OK;
}