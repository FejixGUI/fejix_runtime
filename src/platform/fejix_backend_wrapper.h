#ifndef _FEJIX_BACKENDS_H_
#define _FEJIX_BACKENDS_H_


#include <fejix_runtime/fejix.h>

#ifdef FJ_USE_OPENGL3
#   include "backend_def/opengl3.h"
#endif

uint32_t _fjBackendInitWindow(struct FjWindow *win); 
void _fjBackendDestroyWindow(struct FjWindow *win); 

uint32_t _fjDrawBegin(struct FjWindow *win, uint32_t width, uint32_t height);
uint32_t _fjDrawPresent(struct FjWindow *win);
void _fjDrawEnd(struct FjWindow *win);



#endif // _FEJIX_BACKENDS_H_