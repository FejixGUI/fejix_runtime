#ifndef _FEJIX_HELPER_H_
#define _FEJIX_HELPER_H_

#ifdef __cplusplus
    extern "C" {
#endif

#define _ALIAS(X) typedef struct X X;

#ifdef _FEJIX_H_
    _ALIAS(FjApp);
    _ALIAS(FjAppParams);
    _ALIAS(FjWindow);
    _ALIAS(FjWindowParams);
    _ALIAS(FjEvent);
    _ALIAS(FjWidget);
    _ALIAS(FjDrawContext);

    _ALIAS(FjConstraints);
    _ALIAS(FjGeometry);
    _ALIAS(FjXY);
#endif

#undef _ALIAS

#ifdef __cplusplus
    } // extern "C"
#endif

#endif // _FEJIX_HELPER_H_