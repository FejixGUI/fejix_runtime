#ifndef _FEJIX_H_
#define _FEJIX_H_


#include <fejix_runtime/fejix_global.h>


struct FjBox {
    int32_t x;
    int32_t y;
};

struct FjGeometry {
    struct FjBox box1; // Position OR minimum size
    struct FjBox box2; // Size OR maximum size
};

// Forward declarations for function prototypes
struct FjWidget;
struct FjRoot;

typedef void (*FjLayoutFn) (
    struct FjWidget *self,
    uint32_t layoutMode
);

typedef void (*FjDrawFn) (
    struct FjWidget *self,
    struct FjRoot *root
);

typedef void (*FjCursorHandlerFn) (
    struct FjWidget *self,
    uint32_t eventType,
    struct FjBox *position
);


struct FjWidget {
    struct FjGeometry constraints;  // Min/Max sizes
    struct FjGeometry geometry;     // Variable set of coordinates 

    void *data;                 // Widget's local data

    struct FjWidget *container; // Parent
    struct FjWidget **content;  // Children
    uint32_t contentLength;     // Number of children
    uint32_t contentIndex;      // Used during layout traversal

    // Functions
    FjLayoutFn layout;
    FjDrawFn draw;
    FjCursorHandlerFn handle;
};

enum FjLayoutMode {
    FJ_LAYOUT_MODE_MAX_SIZE,
    FJ_LAYOUT_MODE_MIN_SIZE,
    FJ_LAYOUT_MODE_FINAL,
};

enum FjCursorEventType {
    FJ_CURSOR_EVENT_HOVER,
    FJ_CURSOR_EVENT_UNHOVER,
    FJ_CURSOR_EVENT_MOVE,
    FJ_CURSOR_EVENT_CLICK_0,
    FJ_CURSOR_EVENT_CLICK_1,
    FJ_CURSOR_EVENT_CLICK_2,
};

struct FjColor {
    float r;
    float g;
    float b;
    float a;
};


struct FjRoot {
    struct FjColor background;
    struct FjBox size;

    uint32_t backend; // Chosen backend

    struct FjWidget *rootWidget;

#ifdef FJ_WMAPI_SDL
    SDL_Renderer *_sdlRenderer;
    SDL_Texture *_sdlTexture; // Back buffer
#endif

#if FJ_BK_HAS_CAIRO
    cairo_surface_t *_crSurface;
    cairo_t *_crContext;
#endif
};



#ifdef FJ_WMAPI_SDL
    uint32_t fjRootInit_Sdl(
        struct FjRoot *root,
        const uint32_t *parameters,
        SDL_Window *window
    );
#endif
void fjRootDestroy(struct FjRoot *root);
void fjRootSetColor(struct FjRoot *root, float R, float G, float B, float A);

void fjLayout(struct FjRoot *root);
uint32_t fjDraw(struct FjRoot *root);

#endif // _FEJIX_H_