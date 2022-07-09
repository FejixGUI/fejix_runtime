#ifndef _FEJIX_DEFINES_H_
#define _FEJIX_DEFINES_H_

#include <fejix_runtime/events.h>

#include <stdint.h>


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

#endif // _FEJIX_DEFINES_H_