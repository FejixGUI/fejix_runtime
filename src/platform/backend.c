#include <fejix_runtime/fejix.h>

#include <platform/backend_definitions/opengl3.h>


// Defined in fejix.h
uint32_t fjBackendInit(struct FjBackendInitContext *ctx)
{
    ctx->instance->backend.instance = ctx->instance;
    ctx->instance->backend.id = FJ_BACKEND_NONE;

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
