#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_x11.h>

static uint32_t init_opengl3(struct FjBackendInitContext *ctx); 
static void destroy_opengl3(struct FjInstance *inst);


uint32_t fjBackendInit(struct FjBackendInitContext *ctx)
{
    switch (ctx->backend) {
        case FJ_BACKEND_OPENGL3:
            return init_opengl3(ctx);
        break;

        default:
            return FJ_ERR_BACKEND_UNKNOWN;
    }
}


void fjBackendDestroy(struct FjInstance *inst)
{
    switch (inst->backend) {
        case FJ_BACKEND_OPENGL3:
            destroy_opengl3(inst);
        break;
    }
}



uint32_t init_opengl3(struct FjBackendInitContext *ctx)
{
    struct FjInstance *inst = ctx->instance;

    if (!gladLoaderLoadGLX(inst->xDisplay, inst->xDefaultScreen))
        return FJ_ERR_BACKEND_FAIL;

    // Don't even think about initializing OpenGL without an existing context!
    /* if (!gladLoaderLoadGL())
        return FJ_ERR_BACKEND_FAIL; */

    const int visualAttr[] = {
        GLX_DOUBLEBUFFER
    };

    // inst->windowVisualId = glXChooseVisual(disp, XDefaultScreen(disp), visualAttr);

    inst->backend = FJ_BACKEND_OPENGL3;

    return FJ_OK;
}

void destroy_opengl3(struct FjInstance *inst)
{
    gladLoaderUnloadGLX();
}