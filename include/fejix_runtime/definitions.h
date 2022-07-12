#ifndef _FEJIX_DEFINES_H_
#define _FEJIX_DEFINES_H_

#include <fejix_runtime/events.h>

#include <stdint.h>
#include <stddef.h>


#define FJ_VERSION_MAJOR 0
#define FJ_VERSION_MINOR 0
#define FJ_VERSION_PATCH 1


// Error codes
#define FJ_OK                       0
#define FJ_ERR_MALLOC_FAILED        1
#define FJ_ERR_WMAPI_FAILED         2
#define FJ_ERR_BACKEND_FAILED       3
#define FJ_ERR_BACKEND_UNKNOWN      4
#define FJ_ERR_INVALID_PARAM        5
#define FJ_ERR_INVALID_ENCODING     6
#define FJ_ERR_FEATURE_NOT_COMPILED 7


#define FJ_BACKEND_NONE         0
#define FJ_BACKEND_NANOVG       1


struct FjWidget;
struct FjWindow;
struct FjApp;
struct FjDrawContext;

typedef uint32_t (*FjEventHandler)(
    struct FjWindow *win,
    struct FjEvent *ev
);

typedef void (*FjDrawFn) (
    struct FjWidget *self,
    struct FjDrawContext *ctx
);

typedef void (*FjLayoutFn) (
    struct FjWidget *self,
    uint32_t mode
);

#define FJ_LAYOUT_MAX   0
#define FJ_LAYOUT_MIN   1
#define FJ_LAYOUT_EXACT 2

struct FjConstraints {
    int32_t minW;
    int32_t minH;
    int32_t maxW;
    int32_t maxH;
};

struct FjGeometry {
    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
};

struct FjXY {
    int32_t x;
    int32_t y;
};

/// The value that should be used for W/H of infinitely big widgets
#define FJ_EXPAND INT32_MAX


/**
 * @brief GUI application parameters
 * 
 */
struct FjAppParams {
    /// Primary backend
    uint32_t backend0;

    /// Fallback backend
    uint32_t backend1;
};


/**
 * @brief Window parameters
 * 
 */
struct FjWindowParams {
    /// Initial width of the window; mandatory to set
    uint32_t width;
    /// Initial height of the window; mandatory to set
    uint32_t height;

    /// Minimal width; set to 0 to ignore
    uint32_t minWidth;  
    /// Minimal height; set to 0 to ignore
    uint32_t minHeight; 

    /// Maximum width; set to 0 to ignore
    uint32_t maxWidth;  
    /// Maximum height; set to 0 to ignore
    uint32_t maxHeight; 

    /**
     * When 0, indicates that the window must not be resizable or maximizable 
     */
    unsigned isResizable: 1;
};


struct FjWidget {
    struct FjWidget *container; // Parent
    struct FjWidget **content;  // Children

    void *data;                 // Widget's local data

    struct FjConstraints constraints;     // Min/Max sizes
    struct FjConstraints _tmpConstraints; // For layout calculation
    struct FjGeometry geometry;          // X/Y/W/H
    struct FjXY weights;                  // Growing coefficients

    uint32_t contentLength;     // Number of children
    uint32_t _contentIndex;     // Used during layout traversal

    // Functions
    FjLayoutFn layout;
    FjDrawFn draw;
    // FjCursorHandlerFn handle;
};

#endif // _FEJIX_DEFINES_H_