#ifndef FEJIX_RUNTIME_PLATFORM_H_
#define FEJIX_RUNTIME_PLATFORM_H_

#include <fejix_runtime/definitions.h>

#ifdef FJ_USE_X11
#   include <fejix_runtime/internal/platform/x11.h>
#endif

/**
 * @brief Initializes the windowing API and graphical backend associated with
 * the app.
 * 
 * @param app The Fejix GUI application to initialize.
 * @param params Application parameters
 * @return uint32_t status, FJ_OK if succeeds, error code otherwise.
 */
uint32_t fjAppInit(
    struct FjApp *app
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
    FjEventHandlerFn eventHandler
);

#endif // FEJIX_RUNTIME_PLATFORM_H_