#include <fejix_runtime/fejix.h>

uint32_t _fjBackendInit_gl3(struct FjBackendParams *ctx)
{
    ctx->app->backend = FJ_BACKEND_OPENGL3;
    return FJ_OK;
}



void _fjBackendDestroy_gl3(struct FjApp *app)
{
    // Empty
}



uint32_t _fjBackendInitWindow_gl3(struct FjWindow *win)
{
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 32;
    pfd.dwFlags = PFD_SUPPORT_OPENGL
                | PFD_DOUBLEBUFFER
                | PFD_DRAW_TO_WINDOW;

    HDC hdc = GetDC(win->hWnd);
    
    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pixelFormat, &pfd);

    win->glctx = wglCreateContext(hdc);

    if (!win->glctx)
        return FJ_ERR_WMAPI_FAILED;

    if (glClear == NULL) {
        wglMakeCurrent(hdc, win->glctx);

        if (!gladLoaderLoadGL())
            return FJ_ERR_BACKEND_FAIL;
    }

    ReleaseDC(win->hWnd, hdc);

    return FJ_OK;
}


void _fjBackendDestroyWindow_gl3(struct FjWindow *win)
{
    wglDeleteContext(win->glctx);
}



uint32_t _fjDrawBegin_gl3(struct FjWindow *win)
{
    if (!wglMakeCurrent(win->hDC, win->glctx))
        return FJ_ERR_BACKEND_FAIL;

    return FJ_OK;
}



void _fjDrawEnd_gl3(struct FjWindow *win)
{
    wglMakeCurrent(win->hDC, NULL);
}



uint32_t _fjDrawPresent_gl3(struct FjWindow *win)
{
    if (!SwapBuffers(win->hDC))
        return FJ_ERR_WMAPI_FAILED;

    return FJ_OK;
}