#include <fejix_runtime/fejix.h>


void fjRootSetColor(struct FjRoot *root, float R, float G, float B, float A)
{
    root->background.r = R;
    root->background.g = G;
    root->background.b = B;
    root->background.a = A;
}


#ifdef FJ_WMAPI_SDL

uint32_t fjRootInit_Sdl(struct FjRoot *root, SDL_Renderer *renderer)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    root->_sdlTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );

    if (!root->_sdlTexture)
        return FJ_ERR_WMAPI_FAIL;

    void *pixels;
    int pitch;
    SDL_LockTexture(root->_sdlTexture, NULL, &pixels, &pitch);

#if FJ_BK_HAS_CAIRO
    root->_crSurface = cairo_image_surface_create_for_data(
        (unsigned char *) pixels,
        CAIRO_FORMAT_ARGB32,
        width, height, pitch
    );

    root->_crContext = cairo_create(root->_crSurface);
#endif // FJ_BK_HAS_CAIRO

    SDL_UnlockTexture(root->_sdlTexture);

    return FJ_OK;
}


void fjRootDestroy_Sdl(struct FjRoot *root)
{

#if FJ_BK_HAS_CAIRO
    cairo_destroy(root->_crContext);
    cairo_surface_destroy(root->_crSurface);
#endif // FJ_BK_HAS_CAIRO

    SDL_DestroyTexture(root->_sdlTexture);
}


uint32_t fjDraw_Sdl(struct FjRoot *root, SDL_Renderer *renderer)
{
    int w, h;
    SDL_GetRendererOutputSize(renderer, &w, &h);

    void *_a;
    int _b;
    SDL_LockTexture(root->_sdlTexture, NULL, &_a, &_b);

#if FJ_BK_HAS_CAIRO
    cairo_t *cr = root->_crContext;
    struct FjColor *bg = &root->background;
    cairo_set_source_rgba(cr, bg->r, bg->g, bg->b, bg->a);
    cairo_rectangle(cr, 0, 0, w, h);
    cairo_fill(cr);
#endif

    SDL_UnlockTexture(root->_sdlTexture);

    SDL_RenderCopy(renderer, root->_sdlTexture, NULL, NULL);
}

#endif // FJ_WMAPI_SDL