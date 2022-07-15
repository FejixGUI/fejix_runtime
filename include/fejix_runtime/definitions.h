/**
 * @file
 * @brief The most important definitions.
 */

#ifndef _FEJIX_DEFINITIONS_H_
#define _FEJIX_DEFINITIONS_H_

#include <fejix_runtime/events.h>
#include <fejix_runtime/backend.h>

#include <stdint.h>
#include <stddef.h>

// Version of Fejix Runtime
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

// Backend identifiers
#define FJ_BACKEND_NONE         0
#define FJ_BACKEND_NANOVG       1

// Layout modes
#define FJ_LAYOUT_MAX   0
#define FJ_LAYOUT_MIN   1
#define FJ_LAYOUT_EXACT 2

// Convenience macro that stands for "infinitely big size (W/H)"
#define FJ_BIG INT32_MAX

// Orientation
#define FJ_HORIZONTAL 0
#define FJ_VERTICAL   1

// Vertical alignment
#define FJ_TOP      0b00
#define FJ_YCENTER  0b01
#define FJ_BOTTOM   0b10

// Horizantal alignment
#define FJ_LEFT     0b00
#define FJ_XCENTER  0b01
#define FJ_RIGHT    0b10


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


struct FjPoint {
    int32_t x;
    int32_t y;
};

struct FjPointF {
    float x;
    float y;
};

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

    /// Set members to 0 on order to ignore
    struct FjConstraints constraints;

    /**
     * When 0, indicates that the window must not be resizable or maximizable 
     */
    unsigned isResizable: 1;
};


struct FjWidget {
    /// Parent
    struct FjWidget *container;
    /// Children
    struct FjWidget **content;
    /// Number of children
    uint32_t contentLength;
    /// Used during widget tree traversals
    uint32_t _contentIndex;

    /// Widget's local data
    void *data;

    // Methods
    FjLayoutFn layout;
    FjDrawFn draw;

    /// Min/Max sizes (makes sense only during layout calculation)
    struct FjConstraints constraints;
    /// X, Y, width, height
    struct FjGeometry geometry;
    /// Flex grow weight
    float weight;
};


#endif // _FEJIX_DEFINITIONS_H_