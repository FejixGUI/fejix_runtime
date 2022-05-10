#ifndef _FEJIX_WINAPI_H_
#define _FEJIX_WINAPI_H_


#include <fejix_runtime/fejix_defines.h>

#include <windows.h>

struct FjInstance {
    HINSTANCE hInst;
    uint32_t *params;
};


struct FjWindow {
    HWND hWnd;
};


#endif // _FEJIX_WINAPI_H_