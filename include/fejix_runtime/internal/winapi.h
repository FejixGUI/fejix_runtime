#ifndef _FEJIX_WINAPI_H_
#define _FEJIX_WINAPI_H_


#include <fejix_runtime/fejix_defines.h>

// How on Earth do we have a piece of software that doesn't know about Unicode?
// Note that later in code we will use the fact that WinAPI uses Unicode.
#ifndef UNICODE
#   define UNICODE
#endif
#ifndef _UNICODE
#   define _UNICODE
#endif

#include <windows.h>


// Windows 10 v1607, Anniversary Update (a.k.a. Redstone 1, RS1)
#if WDK_NTDDI_VERSION >= 0x0A000002
#   define FJ_DPI_AWARE
    // Older versions of Windows may also support DPI awereness.
    // However, I am not sure if it will compile and run correctly
#endif



/**
 * @brief The main entity of Fejix runtime.
 * 
 * Do not define multiple instances in one thread
 * 
 * @todo Multi-thread
 * @todo Multi-app
 * 
 */
struct FjApp {
    HINSTANCE hInst;
    WNDCLASSEX windowClass;
    uint32_t backend;

    FjEventHandler handler;

#ifdef FJ_USE_OPENGL3
    PIXELFORMATDESCRIPTOR gl3_pixelFormatDesc;
    int32_t gl3_pixelFormat;
#endif
};


struct FjWindow {
    struct FjApp *app;

    HWND hWnd;
    HDC hDC; ///< Temporary internal value
    LPWSTR encodedTitle; ///< Must be freed!

#ifdef FJ_USE_OPENGL3
    HGLRC glctx; 
#endif
};


#endif // _FEJIX_WINAPI_H_