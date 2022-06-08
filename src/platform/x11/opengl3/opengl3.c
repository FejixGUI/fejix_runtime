#include <platform/backend_definitions/opengl3.h>



uint32_t _fjBackendInit_gl3(struct FjBackendInitContext *ctx)
{
    struct FjInstance *instance = ctx->instance;

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
    instance->gl3_framebufferConfig = framebufferConfigs[0];

    glXGetFBConfigAttrib(
        instance->xDisplay,
        instance->gl3_framebufferConfig,
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

    instance->backend = FJ_BACKEND_OPENGL3;

    return FJ_OK;
}



void _fjBackendDestroy_gl3(struct FjInstance *instance)
{
    gladLoaderUnloadGLX();
}



uint32_t _fjBackendInitWindow_gl3(struct FjWindow *win)
{
    struct FjInstance *instance = win->instance;


    win->glctx = glXCreateNewContext(
        instance->xDisplay,
        instance->gl3_framebufferConfig,
        GLX_RGBA_TYPE,
        0,
        True
    );

    if (!win->glctx)
        return FJ_ERR_BACKEND_FAIL;


    win->glxwin = glXCreateWindow(
        instance->xDisplay,
        instance->gl3_framebufferConfig,
        win->windowId,
        NULL // Attribute list
    );

    if (!win->glxwin) {
        glXDestroyContext(instance->xDisplay, win->glctx);

        return FJ_ERR_BACKEND_FAIL;
    }

    if (glClear == NULL) {
        glXMakeContextCurrent(
            win->instance->xDisplay,
            win->glxwin,
            win->glxwin,
            win->glctx
        );

        if (!gladLoaderLoadGL()) {
            glXDestroyContext(instance->xDisplay, win->glctx);
            glXDestroyWindow(win->instance->xDisplay, win->glxwin);
            return FJ_ERR_WMAPI_FAIL;
        }
    }

    return FJ_OK;    
}


void _fjBackendDestroyWindow_gl3(struct FjWindow *win)
{
    glXDestroyWindow(win->instance->xDisplay, win->glxwin);
    glXDestroyContext(win->instance->xDisplay, win->glctx);
}


uint32_t _fjDrawBegin_gl3(struct FjWindow *win, uint32_t W, uint32_t H)
{
    glXMakeContextCurrent(
        win->instance->xDisplay,
        win->glxwin,
        win->glxwin,
        win->glctx
    );

    glViewport(0, 0, W, H);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    return FJ_OK;
}


void _fjDrawEnd_gl3(struct FjWindow *win)
{
    glXMakeContextCurrent(
        win->instance->xDisplay,
        0, 0, 0
    );
}

uint32_t _fjDrawPresent_gl3(struct FjWindow *win)
{
    glXMakeContextCurrent(
        win->instance->xDisplay,
        win->glxwin,
        win->glxwin,
        win->glctx
    );

    glXSwapBuffers(win->instance->xDisplay, win->glxwin);

    glXMakeContextCurrent(
        win->instance->xDisplay,
        0, 0, 0
    );

    return FJ_OK;
}