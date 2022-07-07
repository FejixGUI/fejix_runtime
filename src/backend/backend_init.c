#include <fejix_private/backend_generic.h>

#ifdef FJ_USE_NANOVG
#   include <fejix_private/nanovg/nanovg.h>
#endif

uint32_t fjBackendInitApp(struct FjApp *app, struct FjBackend *backend, struct FjBackendParams *params)
{
    backend->backendId = FJ_BACKEND_NONE;

    uint32_t backendId = params->backend0;
    uint32_t status = FJ_OK;

    for (int i=0; i<2; i++) {
        if (i == 1) backendId = params->backend1;

        switch (backendId)
        {
            case FJ_BACKEND_NANOVG:
#              ifdef FJ_USE_NANOVG
                    status = fjBackendInitApp_nanovg(app, backend, params);
#              else
                    return FJ_ERR_FEATURE_NOT_COMPILED;
#              endif
            break;

            case FJ_BACKEND_NONE:
            break;

            default:
                return FJ_ERR_BACKEND_UNKNOWN;
        }

        if (status == FJ_OK)
            return FJ_OK;
    }

    return status;
}