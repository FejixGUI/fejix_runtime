/**
 * @file
 * @brief Contains functions that redirect to actual backend functions depending
 * on the backend that has been initialized.
 * 
 */

#ifndef _FEJIX_BACKEND_H_
#define _FEJIX_BACKEND_H_


typedef void FjBackendAppData;
typedef void FjBackendWindowData;


struct FjBackendParams;
struct FjApp;
struct FjWindow;


struct FjBackend {
    struct FjApp *app;
    FjBackendAppData *appData;

    uint32_t backendId;

    void (*destroy)(
        struct FjBackend *self
    );

    uint32_t (*initWindow)(
        struct FjBackend *self,
        struct FjWindow *win
    ); 

    void (*destroyWindow)(
        struct FjBackend *self,
        struct FjWindow *win
    ); 

    uint32_t (*drawWindow)(
        struct FjBackend *self,
        struct FjWindow *win,
        uint32_t width,
        uint32_t height
    );

    void (*presentWindow)(
        struct FjBackend *self,
        struct FjWindow *win
    );
};




#endif // _FEJIX_BACKEND_H_