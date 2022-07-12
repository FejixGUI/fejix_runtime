#include <fejix_runtime/fejix.h>

#include <fejix_private/traverse_widgets.h>

void fjAppSetWindows(
    struct FjApp *app,
    struct FjWindow **windows,
    uint32_t windowsLen
)
{
    app->windows = windows;
    app->windowsLen = windowsLen;
}

