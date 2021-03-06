#ifndef FEJIX_RUNTIME_DEV_X11_UTILS_H_
#define FEJIX_RUNTIME_DEV_X11_UTILS_H_

#include <fejix_runtime/definitions.h>


void fj_X11_WindowInitSyncCounter(struct FjWindow *win);

void fj_X11_WindowDestroySyncCounter(struct FjWindow *win);

void fj_X11_WindowIncrSyncCounter(struct FjWindow *win);

#endif // FEJIX_RUNTIME_DEV_X11_UTILS_H_