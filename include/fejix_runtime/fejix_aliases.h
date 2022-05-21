#ifndef _FEJIX_ALIASES_H_
#define _FEJIX_ALIASES_H_


#include <fejix_runtime/fejix.h>


#define _ALIAS(NAME) typedef struct NAME NAME;

_ALIAS(FjBox)
_ALIAS(FjWidget)

#undef _ALIAS


#endif // _FEJIX_ALIASES_H_