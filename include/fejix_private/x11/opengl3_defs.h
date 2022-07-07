#ifndef _FEJIX_X11_OPENGL3_H_
#define _FEJIX_X11_OPENGL3_H_

#include <glad/gl.h>
#include <glad/glx.h>

struct FjBackendAppData_opengl3 {
    GLXFBConfig framebufferConfig;
};


struct FjBackendWindowData_opengl3 {
    GLXWindow glxwin;
    GLXContext glctx;
};


#endif // _FEJIX_X11_OPENGL3_H_