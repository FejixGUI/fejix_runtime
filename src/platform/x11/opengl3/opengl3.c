#include <fejix_runtime/fejix.h>

#include <fejix_private/opengl3_generic.h>

#include <malloc.h>



uint32_t fjBackendInitApp_opengl3(
    struct FjApp *app,
    struct FjBackendAppData_opengl3 *appData,
    struct FjBackendParams *params
)
{
    if (!gladLoaderLoadGLX(app->xDisplay, app->xDefaultScreen))
        return FJ_ERR_WMAPI_FAILED;

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
        app->xDisplay,
        app->xDefaultScreen,
        visualAttribs,
        &numFbConfigs
    );

    // TODO Select the right framebuffer configuration
    appData->framebufferConfig = framebufferConfigs[0];

    glXGetFBConfigAttrib(
        app->xDisplay,
        appData->framebufferConfig,
        GLX_VISUAL_ID,
        &app->windowVisualId
    );

    xcb_free_colormap(app->connection, app->colormapId);

    xcb_create_colormap(
        app->connection,
        XCB_COLORMAP_ALLOC_NONE,
        app->colormapId,
        app->screen->root,
        app->windowVisualId
    );

    return FJ_OK;
}



void fjBackendDestroyApp_opengl3(
    struct FjApp *app,
    struct FjBackendAppData_opengl3 *appData
)
{
    gladLoaderUnloadGLX();
}



uint32_t fjBackendInitWindow_opengl3(
    struct FjApp *app,
    struct FjWindow *win,
    struct FjBackendAppData_opengl3 *appData,
    struct FjBackendWindowData_opengl3 *winData
)
{
    winData->glctx = glXCreateNewContext(
        app->xDisplay,
        appData->framebufferConfig,
        GLX_RGBA_TYPE,
        0,
        True
    );

    if (!winData->glctx)
        return FJ_ERR_BACKEND_FAILED;

    winData->glxwin = glXCreateWindow(
        app->xDisplay,
        appData->framebufferConfig,
        win->windowId,
        NULL // Attribute list
    );

    if (!winData->glxwin) {
        glXDestroyContext(app->xDisplay, winData->glctx);
        return FJ_ERR_BACKEND_FAILED;
    }

    glXMakeContextCurrent(
        app->xDisplay,
        winData->glxwin,
        winData->glxwin,
        winData->glctx
    );

    if (glClear == NULL) {
        if (!gladLoaderLoadGL()) {
            glXDestroyContext(app->xDisplay, winData->glctx);
            glXDestroyWindow(app->xDisplay, winData->glxwin);
            return FJ_ERR_WMAPI_FAILED;
        }
    }

    return FJ_OK;
}


void fjBackendDestroyWindow_opengl3(
    struct FjWindow *win,
    struct FjBackendWindowData_opengl3 *winData
)
{
    glXDestroyWindow(win->app->xDisplay, winData->glxwin);
    glXDestroyContext(win->app->xDisplay, winData->glctx);
}



void fjBackendPrepareWindow_opengl3(
    struct FjWindow *win,
    struct FjBackendWindowData_opengl3 *winData
)
{
    glXMakeContextCurrent(
        win->app->xDisplay,
        winData->glxwin,
        winData->glxwin,
        winData->glctx
    );
}



/// Only a prepared window can be presented to the screen
void fjBackendPresentWindow_opengl3(
    struct FjWindow *win,
    struct FjBackendWindowData_opengl3 *winData
)
{
    glXSwapBuffers(win->app->xDisplay, winData->glxwin);
}