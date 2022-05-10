#ifndef _FEJIX_H_
#define _FEJIX_H_


#include <fejix_runtime/fejix_defines.h>

#ifdef FJ_USE_WINAPI
#   include <fejix_runtime/fejix_winapi.h>
#endif



uint32_t fjInstanceInit(struct FjInstance *inst, uint32_t *params);
void fjInstanceDestroy(struct FjInstance *inst);

uint32_t fjWindowInit(struct FjInstance *inst, struct FjWindow *win, uint32_t *params);
void fjWindowDestroy(struct FjWindow *win);


#endif // _FEJIX_H_