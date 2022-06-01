#include "opengl3/opengl3.h"



uint32_t _fjBackendInit_gl3(struct FjBackendInitContext *ctx)
{
    struct FjInstance *inst = ctx->instance;

    if (!gladLoaderLoadGLX(inst->xDisplay, inst->xDefaultScreen))
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
        inst->xDisplay,
        inst->xDefaultScreen,
        visualAttribs,
        &numFbConfigs
    );

    // TODO Select the right framebuffer configuration
    inst->gl3_framebufferConfig = framebufferConfigs[0];

    glXGetFBConfigAttrib(
        inst->xDisplay,
        inst->gl3_framebufferConfig,
        GLX_VISUAL_ID,
        &inst->windowVisualId
    );

    xcb_free_colormap(inst->connection, inst->colormapId);

    xcb_create_colormap(
        inst->connection,
        XCB_COLORMAP_ALLOC_NONE,
        inst->colormapId,
        inst->screen->root,
        inst->windowVisualId
    );

    inst->backend = FJ_BACKEND_OPENGL3;

    return FJ_OK;
}



void _fjBackendDestroy_gl3(struct FjInstance *inst)
{
    gladLoaderUnloadGLX();
}



uint32_t _fjBackendInitWindow_gl3(struct FjWindow *win)
{
    struct FjInstance *inst = win->inst;


    win->glctx = glXCreateNewContext(
        inst->xDisplay,
        inst->gl3_framebufferConfig,
        GLX_RGBA_TYPE,
        0,
        True
    );

    if (!win->glctx)
        return FJ_ERR_BACKEND_FAIL;


    win->glxwin = glXCreateWindow(
        inst->xDisplay,
        inst->gl3_framebufferConfig,
        win->windowId,
        NULL // Attribute list
    );

    if (!win->glxwin) {
        glXDestroyContext(inst->xDisplay, win->glctx);

        return FJ_ERR_BACKEND_FAIL;
    }

    if (glClear == NULL) {
        glXMakeContextCurrent(
            win->inst->xDisplay,
            win->glxwin,
            win->glxwin,
            win->glctx
        );

        if (!gladLoaderLoadGL()) {
            glXDestroyContext(inst->xDisplay, win->glctx);
            glXDestroyWindow(win->inst->xDisplay, win->glxwin);
            return FJ_ERR_WMAPI_FAIL;
        }
    }

    return FJ_OK;    
}


void _fjBackendDestroyWindow_gl3(struct FjWindow *win)
{
    glXDestroyWindow(win->inst->xDisplay, win->glxwin);
    glXDestroyContext(win->inst->xDisplay, win->glctx);
}


uint32_t _fjWindowBeginDrawing_gl3(struct FjWindow *win)
{
    glXMakeContextCurrent(
        win->inst->xDisplay,
        win->glxwin,
        win->glxwin,
        win->glctx
    );
}


void _fjWindowEndDrawing_gl3(struct FjWindow *win)
{
    glXMakeContextCurrent(
        win->inst->xDisplay,
        0, 0, 0
    );
}

uint32_t _fjWindowPresentDrawing_gl3(struct FjWindow *win)
{
    glXSwapBuffers(win->inst->xDisplay, win->glxwin);

    return FJ_OK;
}