#ifndef _FEJIX_H_
#define _FEJIX_H_


#include <fejix_runtime/fejix_global.h>


#if FJ_BK_TOTAL == 0
#error "[Generated by "fejix.h"] No backend specified!\
 Define FJ_BK_USE_<backend-name> to specify what backend you want to use.\
 Available backend names: CAIRO, OPENGL3."
#endif


struct FjBox {
    int32_t width;
    int32_t height;
};

struct FjRect {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

struct FjWidget {
    struct FjBox  boxMin;
    struct FjBox  boxMax;
    struct FjRect geometry;

    struct FjWidget *container; // Parent
    struct FjWidget **content;  // Children
    uint32_t contentLength;     // Number of children
};


struct FjColor {
    float r;
    float g;
    float b;
    float a;
};


struct FjRoot {
    struct FjColor background;

    uint32_t backend; // Chosen backend

    struct FjWidget *rootWidget;

    uint32_t *paramList;

#ifdef FJ_WMAPI_SDL
    SDL_Texture *_sdlTexture; // Back buffer
#endif

#if FJ_BK_HAS_CAIRO
    cairo_surface_t *_crSurface;
    cairo_t *_crContext;
#endif
};

void fjRootSetColor(struct FjRoot *root, float R, float G, float B, float A);

#ifdef FJ_WMAPI_SDL
    uint32_t fjRootInit_Sdl(struct FjRoot *root, SDL_Renderer *renderer);
    void fjRootDestroy_Sdl(struct FjRoot *root);

    uint32_t fjDraw_Sdl(struct FjRoot *root, SDL_Renderer *renderer);
#endif


#endif // _FEJIX_H_