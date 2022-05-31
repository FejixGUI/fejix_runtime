#ifndef _FEJIX_HELPER_H_
#define _FEJIX_HELPER_H_

#define _ALIAS(X) typedef struct X X;

#ifdef _FEJIX_H_
    _ALIAS(FjInstance);
    _ALIAS(FjInstanceInitContext);
    _ALIAS(FjWindow);
    _ALIAS(FjWindowParams);
    _ALIAS(FjEvent);
#endif

#undef _ALIAS

#endif // _FEJIX_HELPER_H_