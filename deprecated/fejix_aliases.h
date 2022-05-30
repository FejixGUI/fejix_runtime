#ifndef _FEJIX_ALIASES_H_
#define _FEJIX_ALIASES_H_


#define _ALIAS(NAME) typedef struct NAME NAME;

#ifdef _FEJIX_H_
    _ALIAS(FjBox)
    _ALIAS(FjGeometry)
    _ALIAS(FjWidget)
    _ALIAS(FjRoot)
    _ALIAS(FjColor)
#endif

#undef _ALIAS


#endif // _FEJIX_ALIASES_H_