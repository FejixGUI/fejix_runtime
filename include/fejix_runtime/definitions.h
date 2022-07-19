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

#define FJ__STR(X) #X
#define FJ__MAKE_VERSION(MAJ, MIN, PAT) \
    FJ__STR(MAJ) "." FJ__STR(MIN) "." FJ__STR(PAT)

/// "MAJOR.MINOR.PATCH"
#define FEJIX_RUNTIME_VERSION_STRING \
    FJ__MAKE_VERSION( \
        FEJIX_RUNTIME_VERSION_MAJOR, \
        FEJIX_RUNTIME_VERSION_MINOR, \
        FEJIX_RUNTIME_VERSION_PATCH)


typedef uint32_t FjStatus;


// FjStatus possible values
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

/** Every FjWindow must have:
 * 1. struct FjApp *app;
 * 2. void *data;
 */
struct FjWindow;

struct FjWindowParams;


#endif // FEJIX_RUNTIME_DEFINITIONS_H_