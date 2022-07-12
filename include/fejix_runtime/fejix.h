/**
 * @file fejix.h
 * @brief Core features of Fejix runtime
 * 
 * This file contains the core features of Fejix runtime.
 * 
 */

#ifndef _FEJIX_H_
#define _FEJIX_H_

#ifdef __cplusplus
    extern "C" {
#endif


#include <fejix_runtime/definitions.h>

#ifdef FJ_USE_WINAPI
#   include <fejix_runtime/internal/winapi.h>
#elif FJ_USE_X11
#   include <fejix_runtime/internal/x11.h>
#endif






/**
 * @brief Initializes the windowing API and graphical backend associated with
 * the app.
 * 
 * @details Initializer must look for, select and initilize a backend itself.
 * 
 * @param app The Fejix GUI application to initialize.
 * @param params Application parameters
 * @return uint32_t status, FJ_OK if succeeds, error code otherwise.
 */
uint32_t fjAppInit(
    struct FjApp *app,
    struct FjAppParams *params
);

void fjAppDestroy(
    struct FjApp *app
);

uint32_t fjAppInitWindow(
    struct FjApp *app,
    struct FjWindow *window,
    struct FjWindowParams *params
);

void fjWindowDestroy(
    struct FjWindow *window
);

/**
 * @brief Sets the list of windows that are to be processed in the main loop.
 * 
 * @param windows Pointer to the array of pointers to windows (can be changed
 * dynamically)
 * @param windowsLen Length of the array 
 */
void fjAppSetWindows(
    struct FjApp *app,
    struct FjWindow **windows,
    uint32_t windowsLen
);

void fjWindowSetVisible(
    struct FjWindow *window,
    uint32_t visible
);

/**
 * @brief Sets window title.
 * 
 * @param window Window
 * @param title UTF-8 string ending with a NULL-terminator
 * @return FJ_OK or error code
 */
uint32_t fjWindowSetTitle(
    struct FjWindow *window,
    const char *title
);


/**
 * @brief Runs the main GUI loop.
 * 
 * @param app Fejix app
 * @param eventHandler Handler for all global events
 */
void fjLoop(
    struct FjApp *app,
    FjEventHandler eventHandler
);




#ifdef __cplusplus
    } // extern "C"
#endif


#endif // _FEJIX_H_