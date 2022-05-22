#ifndef _FEJIX_GLOBAL_H_
#define _FEJIX_GLOBAL_H_


#include <stdint.h>


// Versions
#define FJ_V_MAJOR 0
#define FJ_V_MINOR 0
#define FJ_V_PATCH 1

// Error codes
#define FJ_OK                   0
#define FJ_ERR_MALLOC_FAIL      1
#define FJ_ERR_WMAPI_FAIL       2
#define FJ_ERR_BK_FAIL          3
#define FJ_ERR_UNKNOWN_PARAM    4
#define FJ_ERR_UNKNOWN_BK       5
#define FJ_ERR_BK_NOT_COMPILED  6

// Parameters for backend initialization
#define FJ_PARAM_END            0
#define FJ_PARAM_INIT_BK        1

// Backend identifiers
#define FJ_BK_NONE      0
#define FJ_BK_CAIRO     1
#define FJ_BK_OPENGL3   2

// Helper macros which make life easier
#define FJ_BK_HAS_CAIRO     defined(FJ_BK_USE_CAIRO)
#define FJ_BK_HAS_OPENGL3   defined(FJ_BK_USE_OPENGL3)

// Total number of backends
#define FJ_BK_TOTAL (0      \
    + FJ_BK_HAS_CAIRO       \
    + FJ_BK_HAS_OPENGL3     \
)



// Backend- and WmApi-specific configuration
// #ifdef FJ_WMAPI_SDL
// #   if FJ_BK_HAS_CAIRO
// #       define FJ_WMAPI_SDL_NEEDS_BACK_BUFFER
// #   endif
// #endif // FJ_WMAPI_SDL



// Include backends

#if FJ_BK_HAS_CAIRO
#   include <cairo/cairo.h>
#endif

#if FJ_BK_HAS_OPENGL3
#   error "Not implemented: OpenGL3 backend" 
#endif


// Include libs for window management

#ifdef FJ_WMAPI_SDL
#   include <SDL2/SDL.h>
#endif


#endif // _FEJIX_GLOBAL_H_