/**
 * @file
 * @brief The most generic and the most important constants and types
 */

#ifndef FEJIX_RUNTIME_DEFINITIONS_H_
#define FEJIX_RUNTIME_DEFINITIONS_H_


#include <fejix_runtime/events.h>

#include <stdint.h>
#include <stddef.h>


#define FEJIX_RUNTIME_VERSION_MAJOR 0
#define FEJIX_RUNTIME_VERSION_MINOR 0
#define FEJIX_RUNTIME_VERSION_PATCH 1

/// "MAJOR.MINOR.PATCH"
#define FEJIX_RUNTIME_VERSION_STR    \
    #FEJIX_RUNTIME_VERSION_MAJOR "." \
    #FEJIX_RUNTIME_VERSION_MINOR "." \
    #FEJIX_RUNTIME_VERSION_PATCH 


// Error codes
enum {
    FJ_OK = 0,

    /// Memory allocation failed
    FJ_ERR_MALLOC_FAILED,

    /// Window management API failed
    FJ_ERR_WMAPI_FAILED,

    /// Graphics API failed
    FJ_ERR_GAPI_FAILED,

    /// Invalid parameter
    FJ_ERR_INVALID_PARAM,
};

struct FjApp;
struct FjWindow;
struct FjWindowParams;


#endif // FEJIX_RUNTIME_DEFINITIONS_H_