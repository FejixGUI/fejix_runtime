#ifndef _FEJIX_PRIVATE_BACKEND_INIT_H_
#define _FEJIX_PRIVATE_BACKEND_INIT_H_

#include <fejix_runtime/definitions.h>
#include <fejix_runtime/backend.h>

struct FjBackendParams {
    struct FjApp *app;
    uint32_t backend0;
    uint32_t backend1;
};

uint32_t fjBackendInitApp(
    struct FjApp *app,
    struct FjBackend *backend,
    struct FjBackendParams *params
);

#endif // _FEJIX_PRIVATE_BACKEND_INIT_H_