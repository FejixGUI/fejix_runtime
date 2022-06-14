#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_winapi.h>

#include "fejix_backend_wrapper.h"

#include <malloc.h>


#define _FJ_WCLASS L"FEJIX_SUPER_WINDOW_CLASS"


static LRESULT CALLBACK _winProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
);

uint32_t fjInstanceInit(
    struct FjInstance *instance,
    FjBackendInitializer init
)
{
    instance->hInst = GetModuleHandle(NULL);

    WNDCLASSEX *wc = &instance->windowClass;
    wc->cbSize = sizeof(WNDCLASSEX);
    wc->cbClsExtra = 0;
    wc->cbWndExtra = 0;
    wc->hInstance = instance->hInst;
    wc->lpfnWndProc = &_winProc;
    wc->hIcon = NULL;
    wc->hIconSm = NULL;
    wc->hCursor = NULL;
    wc->hbrBackground = GetStockObject(GRAY_BRUSH);
    wc->lpszMenuName = NULL;
    wc->lpszClassName = _FJ_WCLASS;
    wc->style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    
    if (!RegisterClassEx(wc))
        return FJ_ERR_WMAPI_FAIL;

#ifdef FJ_DPI_AWARE
    // NVidia drivers hate this: 
    // SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
#endif

    struct FjBackendInitContext ctx;
    ctx.instance = instance;

    return init(&ctx);

}


void fjInstanceDestroy(struct FjInstance *instance)
{
    UnregisterClass(_FJ_WCLASS, instance->hInst);
}


uint32_t fjIntanceInitWindow(
    struct FjInstance *inst,
    struct FjWindow *win,
    const struct FjWindowParams *params
)
{
    win->instance = inst;

    DWORD style = WS_TILED
                | WS_SYSMENU /* The close/maximize/minimize button panel */
                | WS_CAPTION
                | WS_MINIMIZEBOX;

    win->hWnd = CreateWindow(
        _FJ_WCLASS,
        L"", /* Title */
        style,
        CW_USEDEFAULT, CW_USEDEFAULT, /* Coordinates */
        params->width, params->height,
        NULL, /* Parent */
        NULL, /* Menu */
        inst->hInst,
        NULL /* Additional application data */
    );

    if (!win->hWnd)
        return FJ_ERR_WMAPI_FAIL;

    // Because in the WinAPI event loop only hwnd will be accessible
    SetWindowLongPtr(win->hWnd, GWLP_USERDATA, (LONG_PTR) win);

    win->encodedTitle = NULL;

    return _fjBackendInitWindow(win);
}


void fjWindowDestroy(struct FjWindow *win)
{
    if (win->encodedTitle) {
        free(win->encodedTitle);
        win->encodedTitle = NULL;
    }

    _fjBackendDestroyWindow(win);

    DestroyWindow(win->hWnd);
}


void fjWindowSetVisible(struct FjWindow *win, uint32_t visible)
{
    ShowWindow(win->hWnd, visible);
}



uint32_t fjWindowSetTitle(struct FjWindow *win, const char *title)
{
    if (title == NULL)
        return FJ_ERR_INVALID_PARAM;

    int numOutputChars = MultiByteToWideChar(
        CP_UTF8,
        0, /* flags */
        title,
        -1, /* convert entire string */
        NULL,
        0 /* output size is unknown; asking for it */
    );

    int numOutputBytes = numOutputChars * sizeof(WCHAR);
    
    if (win->encodedTitle)
        win->encodedTitle = realloc(win->encodedTitle, numOutputBytes);
    else
        win->encodedTitle = malloc(numOutputBytes);

    int ret = MultiByteToWideChar(
        CP_UTF8,
        0, /* flags */
        title,
        -1, /* convert entire string */
        win->encodedTitle,
        numOutputChars /* output size is number of chars */
    );

    if (ret == 0)
        return FJ_ERR_INVALID_ENCODING; // Invalid Unicode

    if (!SetWindowText(win->hWnd, win->encodedTitle))
        return FJ_ERR_WMAPI_FAIL;

    return FJ_OK;
}


void fjLoop(
    struct FjInstance *instance,
    FjEventHandler eventHandler,
    struct FjWindow **windows,
    uint32_t numberOfWindows
)
{
    instance->handler = eventHandler;

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}



LRESULT CALLBACK _winProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
){
    LONG_PTR lpUserData = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (!lpUserData) {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    struct FjWindow *win = (struct FjWindow *) lpUserData;
    struct FjInstance *inst = win->instance;
    FjEventHandler handler = inst->handler;

    struct FjEvent ev = {0};

    switch (uMsg)
    {
    case WM_DESTROY: {
        ev.eventType = FJ_EVENT_CLOSE;
        if (handler(win, &ev) == FJ_EXIT)
            PostQuitMessage(0);
        return 0;
    }

#ifdef FJ_DPI_AWARE
    case WM_DPICHANGED: {
        return 0;
    }
#endif

    case WM_PAINT: {

        PAINTSTRUCT ps;
        win->hDC = BeginPaint(hwnd, &ps);

        _fjDrawBegin(win);

        glClearColor(1.0f, 0.7f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        _fjDrawEnd(win);

        _fjDrawPresent(win);

        return 0;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}