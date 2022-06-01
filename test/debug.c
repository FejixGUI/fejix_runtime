#include "debug.h"
#include <stdlib.h>

#include <fejix_runtime/fejix_defines.h>

void checkFj(const char *file, int line, int value)
{
#define out(MSG) printf(MSG " [%s:%d]\n", file, line)

    switch (value)
    {
    case FJ_OK:
        return;
    
    case FJ_ERR_BACKEND_FAIL:
        out("Backend error");
        break;

    case FJ_ERR_BACKEND_UNKNOWN:
        out("Unknown backend");
        break;
    
    case FJ_ERR_INVALID_ENCODING:
        out("Invalid encoding");
        break;

    case FJ_ERR_INVALID_PARAM:
        out("Invalid function parameter");
        break;

    case FJ_ERR_MALLOC_FAIL:
        out("Out of memory (malloc failed)");
        break;

    case FJ_ERR_WMAPI_FAIL:
        out("Window manager API failed");
        break;

    case FJ_ERR_FEATURE_NOT_COMPILED:
        out("Feature is not compiled (perhaps, define FJ_USE_X)");
        break;

    default:
        printf("Unknown error code #%d [%s:%d]\n", value, file, line);       
        break;
    }

    exit(1);
}