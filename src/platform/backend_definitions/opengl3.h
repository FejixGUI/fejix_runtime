#ifndef _FEJIX_BACKEND_DEF_OPENGL3_H_
#define _FEJIX_BACKEND_DEF_OPENGL3_H_

#include <fejix_runtime/fejix_defines.h>

struct FjBackendInitContext;
struct _FjBackend;

uint32_t _fjBackendInit_gl3(struct FjBackendInitContext *ctx); 

void _fjBackendDestroy_gl3(struct _FjBackend *); 

uint32_t _fjBackendInitWindow_gl3(struct _FjBackend *, struct FjWindow *); 

void _fjBackendDestroyWindow_gl3(struct _FjBackend *, struct FjWindow *win); 

uint32_t _fjBackendDraw_gl3(
    struct _FjBackend *,
    struct FjWindow *,
    uint32_t W,
    uint32_t H
);

uint32_t _fjBackendPresent_gl3(struct _FjBackend *, struct FjWindow *win);


#endif // _FEJIX_BACKEND_DEF_OPENGL3_H_