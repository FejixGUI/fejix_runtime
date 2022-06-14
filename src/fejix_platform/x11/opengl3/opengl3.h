#ifndef _FEJIX_X11_OPENGL3_H_
#define _FEJIX_X11_OPENGL3_H_

#include <fejix_drawer/opengl3/opengl3.h>

#include <glad/gl.h>
#include <glad/glx.h>

struct FjBackendInstanceData_opengl3 {
    GLXFBConfig framebufferConfig;
};


struct FjBackendWindowContext_opengl3 {
    GLXWindow glxwin;
    GLXContext glctx;
};


#endif // _FEJIX_X11_OPENGL3_H_