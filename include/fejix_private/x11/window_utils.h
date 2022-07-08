#ifndef _FEJIX_X11_WINDOW_UTILS_H_
#define _FEJIX_X11_WINDOW_UTILS_H_

#include <fejix_runtime/fejix.h>

void fjWindowInitParams_x11(struct FjWindow *win, struct FjWindowParams *params);

void fjWindowInitSyncCounter_x11(struct FjWindow *win);

void fjWindowDestroySyncCounter_x11(struct FjWindow *win);

void fjWindowIncrSyncCounter_x11(struct FjWindow *win);

#endif // _FEJIX_X11_WINDOW_UTILS_H_