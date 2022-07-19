#include <fejix_runtime/fejix_runtime.h>
#include <fejix_runtime/typedefs.h>


void fjAppSetWindows(FjApp *app, FjWindow **windows, uint32_t windowsLen) {
    app->windows = windows;
    app->windowsLen = windowsLen;
}