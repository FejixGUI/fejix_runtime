#ifndef _FEJIX_DEFINES_H_
#define _FEJIX_DEFINES_H_


#include <stdint.h>


#define FJ_VERSION_MAJOR 0
#define FJ_VERSION_MINOR 0
#define FJ_VERSION_PATCH 1


// Error codes
#define FJ_OK                   0
#define FJ_ERR_MALLOC_FAIL      1
#define FJ_ERR_WMAPI_FAIL       2
#define FJ_ERR_BACKEND_FAIL     3
#define FJ_ERR_UNKNOWN_PARAM    4
#define FJ_ERR_UNKNOWN_BACKEND  5
#define FJ_ERR_BACKEND_ABSENT   6


// Instance parameters
#define FJ_IPARAM_END            0
#define FJ_IPARAM_USE_OPENGL     1

// Window parameters 
#define FJ_WPARAM_END           0
#define FJ_WPARAM_RESIZABLE     1


#endif // _FEJIX_DEFINES_H_