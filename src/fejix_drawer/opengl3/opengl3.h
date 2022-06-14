#ifndef _FEJIX_DRAWER_OPENGL3_H_
#define _FEJIX_DRAWER_OPENGL3_H_


#include <fejix_runtime/fejix_defines.h>
#include <glad/gl.h>


struct FjBackendDrawContext_opengl3 {
    GLuint program;
    GLuint shapeVBO;
    GLuint shapeVAO;
};

uint32_t _fjOpengl3InitContext(struct FjBackendDrawContext_opengl3 *ctx);
void _fjOpengl3DestroyContext(struct FjBackendDrawContext_opengl3 *ctx);
void _fjOpengl3Draw(struct FjBackendDrawContext_opengl3 *ctx);


#endif // _FEJIX_DRAWER_OPENGL3_H_