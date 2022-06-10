/**
 * @file fejix_backend_wrapper.h
 * @brief Contains functions that redirect to actual backend functions depending
 * on the backend that has been initialized.
 * 
 */

#ifndef _FEJIX_BACKEND_H_
#define _FEJIX_BACKEND_H_


#include <fejix_runtime/fejix.h>


struct _FjBackend {
    struct FjInstance *instance;
    void *data;

    uint32_t id;

    void (*destroy)(
        struct _FjBackend *self
    );

    uint32_t (*initWindow)(
        struct _FjBackend *self,
        struct FjWindow *win
    ); 

    void (*destroyWindow)(
        struct _FjBackend *self,
        struct FjWindow *win
    ); 

    uint32_t (*draw)(
        struct _FjBackend *self,
        struct FjWindow *win,
        uint32_t width,
        uint32_t height
    );

    uint32_t (*present)(
        struct _FjBackend *self,
        struct FjWindow *win
    );
};



#endif // _FEJIX_BACKEND_H_