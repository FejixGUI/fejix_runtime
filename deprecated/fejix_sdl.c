#include <fejix_runtime/fejix.h>
#include <stdio.h>

#if FJ_BK_HAS_CAIRO

static uint32_t _fjInitCairo(struct FjRoot *root, SDL_Window *win)
{
    root->_sdlRenderer = SDL_CreateRenderer(win, -1, 0);

    if (!root->_sdlRenderer)
        return FJ_ERR_WMAPI_FAIL;

    int width, height;
    SDL_GetRendererOutputSize(root->_sdlRenderer, &width, &height);
    root->size.x = width;
    root->size.y = height;

    root->_sdlTexture = SDL_CreateTexture(
        root->_sdlRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );

    if (!root->_sdlTexture)
        return FJ_ERR_WMAPI_FAIL;

    void *pixels;
    int pitch;
    SDL_LockTexture(root->_sdlTexture, NULL, &pixels, &pitch);

    root->_crSurface = cairo_image_surface_create_for_data(
        (unsigned char *) pixels,
        CAIRO_FORMAT_ARGB32,
        width, height, pitch
    );

    root->_crContext = cairo_create(root->_crSurface);

    if (!root->_crContext) {
        SDL_DestroyTexture(root->_sdlTexture);
        SDL_DestroyRenderer(root->_sdlRenderer);
        return FJ_ERR_BK_FAIL;
    }

    SDL_UnlockTexture(root->_sdlTexture);

    root->backend = FJ_BK_CAIRO;

    return FJ_OK;
}



static void _fjDestroyCairo(struct FjRoot *root)
{
    cairo_destroy(root->_crContext);
    cairo_surface_destroy(root->_crSurface);

    if (root->_sdlTexture)
        SDL_DestroyTexture(root->_sdlTexture);

    if (root->_sdlRenderer)
        SDL_DestroyRenderer(root->_sdlRenderer);
}

#endif // FJ_BK_HAS_CAIRO





uint32_t fjRootInit_Sdl(
    struct FjRoot *root,
    const uint32_t *params,
    SDL_Window *window
)
{
    uint32_t status = FJ_OK;

    if (params != NULL) {
        int i = 0;
#define NEXT_PARAM params[i++]
        while (params[i] != FJ_PARAM_END && i < 1024)
        {
            switch (NEXT_PARAM)
            {
                case FJ_PARAM_INIT_BK:
                {   
                    uint32_t backend_id = NEXT_PARAM;

                    if (root->backend != FJ_BK_NONE)
                        continue;

                    switch (backend_id)
                    {
                        case FJ_BK_CAIRO:
#if FJ_BK_HAS_CAIRO
                            status = _fjInitCairo(root, window);
#else
                            return FJ_ERR_BK_NOT_COMPILED;
#endif                            
                        break;

                        default:
                            return FJ_ERR_UNKNOWN_BK;
                    }
                    
                    break;
                }

                default:
                    return FJ_ERR_UNKNOWN_PARAM;
            }
        }
#undef NEXT_PARAM
    }

    

    return status;
}


void fjRootDestroy(struct FjRoot *root)
{
#if FJ_BK_HAS_CAIRO
    if (root->backend == FJ_BK_CAIRO) {
        _fjDestroyCairo(root);
    }
#endif
}


uint32_t fjDraw(struct FjRoot *root)
{
    
#if FJ_BK_HAS_CAIRO
    int width, height;
    SDL_GetRendererOutputSize(root->_sdlRenderer, &width, &height);

    void *_a;
    int _b;
    SDL_LockTexture(root->_sdlTexture, NULL, &_a, &_b);
    
    if (root->backend == FJ_BK_CAIRO) {
        cairo_t *cr = root->_crContext;
        struct FjColor *bg = &root->background;
        cairo_set_source_rgba(cr, bg->r, bg->g, bg->b, bg->a);
        cairo_rectangle(cr, 0, 0, width, height);
        cairo_fill(cr);
    }

    SDL_UnlockTexture(root->_sdlTexture);

    SDL_RenderCopy(root->_sdlRenderer, root->_sdlTexture, NULL, NULL);

    SDL_RenderPresent(root->_sdlRenderer);
#endif
    
}