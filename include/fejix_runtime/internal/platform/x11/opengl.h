#ifndef FEJIX_RUNTIME_X11_OPENGL_H_
#define FEJIX_RUNTIME_X11_OPENGL_H_

/*
 * We are going to include <glad/gl.h> to load OpenGL.
 * If we included <GL/glx.h>, it would include <GL/gl.h>
 * which is incompatible with <glad/gl.h> 
 * So, we use Glad's own GLX implementation
 */

#include <glad/gl.h>
#include <glad/glx.h>

#include <fejix_runtime/internal/features/opengl.h>


struct FjBackendAppData_opengl3 {
    GLXFBConfig framebufferConfig;
};


struct FjBackendWindowData_opengl3 {
    GLXWindow glxwin;
    GLXContext glctx;
};


#endif // FEJIX_RUNTIME_X11_OPENGL_H_