#include <fejix_runtime/fejix.h>

#include <fejix_platform/x11/opengl3/opengl3.h>
#include <fejix_drawer/opengl3/opengl3.h>

#include <malloc.h>


static void destroy(
    struct FjBackendInstanceContext *
); 
static uint32_t initWindow(
    struct FjBackendInstanceContext *, struct FjWindow *
); 
static void destroyWindow(
    struct FjBackendInstanceContext *, struct FjWindow *win
); 
static uint32_t draw(
    struct FjBackendInstanceContext *, struct FjWindow *, uint32_t W, uint32_t H
);
static uint32_t present(
    struct FjBackendInstanceContext *, struct FjWindow *win
);


uint32_t _fjBackendInitInstance_opengl3(struct FjBackendParams *ctx)
{
    struct FjInstance *instance;
    struct FjBackendInstanceContext *instanceContext;
    struct FjBackendInstanceData_opengl3 *instanceData;

    instance = ctx->instance;
    instanceContext = &instance->instanceContext;

    instanceData = malloc(sizeof(*instanceData));
    if (!instanceData)
        return FJ_ERR_MALLOC_FAIL;

    instanceContext->instanceData = instanceData;

    if (!gladLoaderLoadGLX(instance->xDisplay, instance->xDefaultScreen))
        return FJ_ERR_WMAPI_FAIL;

    // Don't even think about initializing OpenGL without an existing context!
    /* if (!gladLoaderLoadGL())
        return FJ_ERR_BACKEND_FAIL; */

    const int visualAttribs[] = {
        GLX_X_RENDERABLE,  True,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RENDER_TYPE,   GLX_RGBA_BIT,
        GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
        GLX_RED_SIZE,     8,
        GLX_GREEN_SIZE,   8,
        GLX_BLUE_SIZE,    8,
        GLX_ALPHA_SIZE,   8,
        GLX_DEPTH_SIZE,   24,
        GLX_STENCIL_SIZE, 8,
        GLX_DOUBLEBUFFER, True,
        // TODO Multisampling (antialiasing)
        //GLX_SAMPLE_BUFFERS  , 1,
        //GLX_SAMPLES         , 4,
        None
    };

    GLXFBConfig *framebufferConfigs = NULL;
    int numFbConfigs = 0;

    framebufferConfigs = glXChooseFBConfig(
        instance->xDisplay,
        instance->xDefaultScreen,
        visualAttribs,
        &numFbConfigs
    );

    // TODO Select the right framebuffer configuration
    instanceData->framebufferConfig = framebufferConfigs[0];

    glXGetFBConfigAttrib(
        instance->xDisplay,
        instanceData->framebufferConfig,
        GLX_VISUAL_ID,
        &instance->windowVisualId
    );

    xcb_free_colormap(instance->connection, instance->colormapId);

    xcb_create_colormap(
        instance->connection,
        XCB_COLORMAP_ALLOC_NONE,
        instance->colormapId,
        instance->screen->root,
        instance->windowVisualId
    );

    instanceContext->backendId = FJ_BACKEND_OPENGL3;

    instanceContext->destroy = &destroy;
    instanceContext->initWindow = &initWindow;
    instanceContext->destroyWindow = &destroyWindow;
    instanceContext->draw = &draw;
    instanceContext->present = &present;

    return FJ_OK;
}



void destroy(struct FjBackendInstanceContext *instanceContext)
{
    instanceContext->backendId = FJ_BACKEND_NONE;

    if (instanceContext->instanceData != NULL)
        free(instanceContext->instanceData);

    gladLoaderUnloadGLX();
}



uint32_t initWindow(
    struct FjBackendInstanceContext *instanceContext,
    struct FjWindow *win
)
{
    struct FjInstance *instance;
    struct FjBackendInstanceData_opengl3 *instanceData;
    struct FjBackendWindowContext_opengl3 *windowContext;
    struct FjBackendDrawContext_opengl3 *drawContext;

    instance = win->instance;
    instanceData = instanceContext->instanceData;

    windowContext = malloc(sizeof(*windowContext));
    if (!windowContext)
        return FJ_ERR_MALLOC_FAIL;

    drawContext = malloc(sizeof(*drawContext));
    if (!drawContext) {
        free(windowContext);
        return FJ_ERR_MALLOC_FAIL;
    }

    win->windowContext = windowContext;
    win->drawContext = drawContext;

    windowContext->glctx = glXCreateNewContext(
        instance->xDisplay,
        instanceData->framebufferConfig,
        GLX_RGBA_TYPE,
        0,
        True
    );

    if (!windowContext->glctx) {
        free(windowContext);
        free(drawContext);
        return FJ_ERR_BACKEND_FAIL;
    }


    windowContext->glxwin = glXCreateWindow(
        instance->xDisplay,
        instanceData->framebufferConfig,
        win->windowId,
        NULL // Attribute list
    );

    if (!windowContext->glxwin) {
        glXDestroyContext(instance->xDisplay, windowContext->glctx);
        free(windowContext);
        free(drawContext);
        return FJ_ERR_BACKEND_FAIL;
    }

    glXMakeContextCurrent(
        instance->xDisplay,
        windowContext->glxwin,
        windowContext->glxwin,
        windowContext->glctx
    );

    if (glClear == NULL) {
        if (!gladLoaderLoadGL()) {
            glXDestroyContext(instance->xDisplay, windowContext->glctx);
            glXDestroyWindow(instance->xDisplay, windowContext->glxwin);
            free(drawContext);
            free(windowContext);
            win->windowContext = NULL;
            win->drawContext = NULL;
            return FJ_ERR_WMAPI_FAIL;
        }
    }

    return _fjDrawContextInit_opengl3(drawContext);
}


void destroyWindow(
    struct FjBackendInstanceContext *instanceContext,
    struct FjWindow *win
)
{
    struct FjBackendWindowContext_opengl3 *windowContext = win->windowContext;
    struct FjBackendDrawContext_opengl3 *drawContext = win->drawContext;

    if (windowContext) {
        glXDestroyWindow(win->instance->xDisplay, windowContext->glxwin);
        glXDestroyContext(win->instance->xDisplay, windowContext->glctx);
        free(windowContext);
    }

    if (drawContext) {
        _fjDrawContextDestroy_opengl3(drawContext);
        free(drawContext);
    }
}


uint32_t draw(
    struct FjBackendInstanceContext *instanceContext,
    struct FjWindow *win,
    uint32_t W,
    uint32_t H
)
{
    struct FjBackendWindowContext_opengl3 *windowContext = win->windowContext;

    glXMakeContextCurrent(
        win->instance->xDisplay,
        windowContext->glxwin,
        windowContext->glxwin,
        windowContext->glctx
    );

    glViewport(0, 0, W, H);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _fjDraw_opengl3(win->drawContext);

    glXMakeContextCurrent(
        win->instance->xDisplay,
        0, 0, 0
    );

    return FJ_OK;
}


uint32_t present(
    struct FjBackendInstanceContext *instanceContext,
    struct FjWindow *win
)
{
    struct FjBackendWindowContext_opengl3 *windowContext = win->windowContext;

    glXMakeContextCurrent(
        win->instance->xDisplay,
        windowContext->glxwin,
        windowContext->glxwin,
        windowContext->glctx
    );

    glXSwapBuffers(win->instance->xDisplay, windowContext->glxwin);

    glXMakeContextCurrent(
        win->instance->xDisplay,
        0, 0, 0
    );

    return FJ_OK;
}