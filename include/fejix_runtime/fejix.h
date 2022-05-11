/**
 * @file fejix.h
 * @brief Core features of Fejix runtime
 * @version 0.0.1
 * @date 2022-05-31
 * 
 * This file contains the core features of Fejix runtime. 
 * 
 */

#ifndef _FEJIX_H_
#define _FEJIX_H_


#include <fejix_runtime/fejix_defines.h>

#ifdef FJ_USE_WINAPI
#   include <fejix_runtime/fejix_winapi.h>
#elif FJ_USE_X11
#   include <fejix_runtime/fejix_x11.h>
#endif

#ifdef FJ_USE_OPENGL3
#   include <glad/gl.h>
#endif



struct FjBackendInitContext {
    struct FjInstance *instance;
    uint32_t backend;
};


typedef uint32_t (*FjBackendInitializer) (
    struct FjBackendInitContext *initCtx
);



struct FjWindowParams {
    uint32_t width;
    uint32_t height;

    // unsigned resizable: 1;
};




/**
 * @brief Initializes the windowing API and graphical backend associated with
 * the instance.
 * 
 * @details Initializer must look for, select and initilize a backend itself.
 * 
 * @param instance The Fejix instance to initialize.
 * @param initializer Mandatory backend initializer function
 * @return uint32_t status, FJ_OK if succeeds, error code otherwise.
 */
uint32_t fjInstanceInit(
    struct FjInstance *instance,
    FjBackendInitializer initializer
);

void fjInstanceDestroy(
    struct FjInstance *instance
);

uint32_t fjBackendInit(
    struct FjBackendInitContext *ctx
);

void fjBackendDestroy(
    struct FjInstance *instance
);

uint32_t fjIntanceInitWindow(
    struct FjInstance *instance,
    struct FjWindow *window,
    const struct FjWindowParams *params
);

void fjWindowDestroy(
    struct FjWindow *window
);

void fjWindowSetShown(
    struct FjWindow *window,
    uint32_t shown
);

/**
 * @brief Sets window title.
 * 
 * @param window Window
 * @param title UTF-8 string ending with NULL-terminator
 * @return uint32_t FJ_OK or error code
 */
uint32_t fjWindowSetTitle(
    struct FjWindow *window,
    const char *title
);


/**
 * @brief Runs main GUI loop.
 * 
 * @param instance Fejix instance
 * @param eventHandler Handler for all global events
 * @param windows Array of windows which will be renderered to
 * @param numberOfWindows Length of windows array
 */
void fjLoop(
    struct FjInstance *instance,
    FjEventHandler eventHandler,
    struct FjWindow **windows,
    uint32_t numberOfWindows
);


#endif // _FEJIX_H_