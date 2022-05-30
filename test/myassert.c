#include "myassert.h"
#include <stdlib.h>

void _massert(const char *file, int line, int value)
{
    if (!value) {
        printf("[%s:%d] Assertion failed! Ooops...", file, line);
        exit(1);
    }
}