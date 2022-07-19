#include "debug.h"
#include <stdlib.h>

#include <fejix_runtime/definitions.h>

void checkFj(const char *file, int line, int value)
{
#define out(MSG) printf(MSG " [%s:%d]\n", file, line)

    switch (value)
    {
    case FJ_OK:
        return;
    
    case FJ_ERR_INVALID_PARAM:
        out("Invalid function parameter");
        break;

    case FJ_ERR_MALLOC_FAILED:
        out("Out of memory (malloc failed)");
        break;

    case FJ_ERR_WMAPI_FAILED:
        out("Window manager API failed");
        break;

    case FJ_ERR_GAPI_FAILED:
        out("Backend error");
        break;

    default:
        printf("Unknown error code #%d [%s:%d]\n", value, file, line);       
        break;
    }

    exit(1);
}