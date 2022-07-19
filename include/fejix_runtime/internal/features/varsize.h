/**
 * @file
 * @brief This feature allows windows to have variable size and size constraints
 */

#ifndef FEJIX_RUNTIME_VARSIZE_H_
#define FEJIX_RUNTIME_VARSIZE_H_


#include <fejix_runtime/definitions.h>


FjStatus fj_Varsize_SetWindowSize(
    struct FjWindow *win,
    uint32_t width,
    uint32_t height
);

FjStatus fj_Varsize_SetWindowSizeMin(
    struct FjWindow *win,
    uint32_t minWidth,
    uint32_t minHeight
);

FjStatus fj_Varsize_SetWindowSizeMax(
    struct FjWindow *win,
    uint32_t maxWidth,
    uint32_t maxHeight
);


#endif // FEJIX_RUNTIME_VARSIZE_H_