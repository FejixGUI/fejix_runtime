#ifndef _FEJIX_DRAWER_OPENGL3_H_
#define _FEJIX_DRAWER_OPENGL3_H_


#include <fejix_runtime/fejix_defines.h>
#include <glad/gl.h>


struct FjBackendDrawContext_opengl3 {
    GLuint program;
    GLuint shapeVBO;
    GLuint shapeVAO;
};

uint32_t _fjDrawContextInit_opengl3(struct FjBackendDrawContext_opengl3 *ctx);
void _fjDrawContextDestroy_opengl3(struct FjBackendDrawContext_opengl3 *ctx);
void _fjDraw_opengl3(struct FjBackendDrawContext_opengl3 *ctx);


#endif // _FEJIX_DRAWER_OPENGL3_H_