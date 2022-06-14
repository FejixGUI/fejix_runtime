#ifndef _FEJIX_X11_WINDOW_UTILS_H_
#define _FEJIX_X11_WINDOW_UTILS_H_

#include <fejix_runtime/fejix.h>

void _fjWindowInitParams(struct FjWindow *win, struct FjWindowParams *params);

void _fjWindowInitSyncCounter(struct FjWindow *win);

void _fjWindowDestroySyncCounter(struct FjWindow *win);

void _fjWindowIncrSyncCounter(struct FjWindow *win);

#endif // _FEJIX_X11_WINDOW_UTILS_H_