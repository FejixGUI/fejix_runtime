#ifndef _FEJIX_DEFINES_H_
#define _FEJIX_DEFINES_H_


#include <stdint.h>


#define FJ_VERSION_MAJOR 0
#define FJ_VERSION_MINOR 0
#define FJ_VERSION_PATCH 1


// Error codes
#define FJ_OK                       0
#define FJ_ERR_MALLOC_FAIL          1
#define FJ_ERR_WMAPI_FAIL           2
#define FJ_ERR_BACKEND_FAIL         3
#define FJ_ERR_BACKEND_UNKNOWN      4
#define FJ_ERR_INVALID_PARAM        5
#define FJ_ERR_INVALID_ENCODING     6

// Responses of event handlers
#define FJ_EXIT                 1

// Events
#define FJ_EVENT_CLOSE          1


#define FJ_BACKEND_NONE         0
#define FJ_BACKEND_OPENGL3      1


#endif // _FEJIX_DEFINES_H_