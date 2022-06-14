/**
 * @file fejix_backend_wrapper.h
 * @brief Contains functions that redirect to actual backend functions depending
 * on the backend that has been initialized.
 * 
 */

#ifndef _FEJIX_BACKEND_H_
#define _FEJIX_BACKEND_H_


typedef void FjBackendInstanceData;
typedef void FjBackendWindowContext;
typedef void FjBackendDrawContext;


struct FjBackendParams;
struct FjInstance;
struct FjWindow;


struct FjBackendInstanceContext {
    struct FjInstance *instance;
    FjBackendInstanceData *instanceData;

    uint32_t backendId;

    void (*destroy)(
        struct FjBackendInstanceContext *self
    );

    uint32_t (*initWindow)(
        struct FjBackendInstanceContext *self,
        struct FjWindow *win
    ); 

    void (*destroyWindow)(
        struct FjBackendInstanceContext *self,
        struct FjWindow *win
    ); 

    uint32_t (*draw)(
        struct FjBackendInstanceContext *self,
        struct FjWindow *win,
        uint32_t width,
        uint32_t height
    );

    uint32_t (*present)(
        struct FjBackendInstanceContext *self,
        struct FjWindow *win
    );
};


#ifdef FJ_USE_OPENGL3
    uint32_t _fjBackendInitInstance_opengl3(struct FjBackendParams *ctx); 
#endif



#endif // _FEJIX_BACKEND_H_