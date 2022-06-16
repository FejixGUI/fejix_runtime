// This file includes everything that is platform-independent

#include <fejix_runtime/fejix.h>


void fjInstanceSetWindows(
    struct FjInstance *instance,
    struct FjWindow **windows,
    uint32_t windowsLen
)
{
    instance->windows = windows;
    instance->windowsLen = windowsLen;
}


uint32_t fjBackendInit(struct FjBackendParams *ctx)
{
    ctx->instance->instanceContext.instance = ctx->instance;
    ctx->instance->instanceContext.backendId = FJ_BACKEND_NONE;

    switch (ctx->backend)
    {
        case FJ_BACKEND_OPENGL3:
#           ifdef FJ_USE_OPENGL3
                return _fjBackendInitInstance_opengl3(ctx);
#           else
                return FJ_ERR_FEATURE_NOT_COMPILED;
#           endif
        break;

        default:
            return FJ_ERR_BACKEND_UNKNOWN;
    }

    return FJ_OK;
}


