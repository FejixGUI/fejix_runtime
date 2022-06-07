#include <fejix_runtime/fejix.h>

void fjInstanceSetWindows(
    struct FjInstance *instance,
    struct FjWindow **windows,
    uint32_t windowsLen
)
{
    instance->windows = windows;
    instance->windowsLen = windowsLen;
}