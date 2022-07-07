#ifndef _FEJIX_DRAWER_NANOVG_H_
#define _FEJIX_DRAWER_NANOVG_H_

#include <fejix_runtime/definitions.h>
#include <fejix_runtime/internal/backend.h>

#include <fejix_private/opengl3_generic.h>

uint32_t fjBackendInitApp_nanovg(struct FjApp *app, struct FjBackend *backend, struct FjBackendParams *params);

#endif // _FEJIX_DRAWER_NANOVG_H_