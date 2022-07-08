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

#define SELF struct FjBackend *self

    void (*destroy)(SELF);

    uint32_t (*initWindow)(SELF, struct FjWindow *win); 
    void (*destroyWindow)(SELF, struct FjWindow *win); 
    void (*presentWindow)(SELF, struct FjWindow *win);

    uint32_t (*prepareWindow)(
        SELF,
        struct FjWindow *win,
        uint32_t width,
        uint32_t height
    );

    struct FjDrawContext* (*getWindowDrawContext)(SELF, struct FjWindow *win);

#undef SELF
};



struct FjDrawContext {
    FjBackendWindowData *windowData;
    
#define SELF struct FjDrawContext *self
    void (*setColor)(SELF, float r, float g, float b, float a);

    void (*beginPath)(SELF);
    void (*rect)(SELF, int32_t x, int32_t y, int32_t w, int32_t h);
    void (*rrect)(
        SELF,
        int32_t x, int32_t y, int32_t w, int32_t h,
        float radius
    );

    void (*fill)(SELF);
    void (*stroke)(SELF, float width);
#undef SELF
};


#endif // _FEJIX_BACKEND_H_