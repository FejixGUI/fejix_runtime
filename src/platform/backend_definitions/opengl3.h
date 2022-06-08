#ifndef _FEJIX_BACKEND_DEF_OPENGL3_H_
#define _FEJIX_BACKEND_DEF_OPENGL3_H_

#include <fejix_runtime/fejix.h>

uint32_t _fjBackendInit_gl3(struct FjBackendInitContext *ctx); 

void _fjBackendDestroy_gl3(struct FjInstance *instance); 

uint32_t _fjBackendInitWindow_gl3(struct FjWindow *win); 

void _fjBackendDestroyWindow_gl3(struct FjWindow *win); 

uint32_t _fjDrawBegin_gl3(struct FjWindow *win, uint32_t W, uint32_t H);

void _fjDrawEnd_gl3(struct FjWindow *win);

uint32_t _fjDrawPresent_gl3(struct FjWindow *win);


#endif // _FEJIX_BACKEND_DEF_OPENGL3_H_