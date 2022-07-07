// This file includes everything that is platform-independent

#include <fejix_runtime/fejix.h>


void fjAppSetWindows(
    struct FjApp *app,
    struct FjWindow **windows,
    uint32_t windowsLen
)
{
    app->windows = windows;
    app->windowsLen = windowsLen;
}


