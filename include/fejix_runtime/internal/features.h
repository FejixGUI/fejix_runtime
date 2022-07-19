#ifndef FEJIX_RUNTIME_FEATURES_H_
#define FEJIX_RUNTIME_FEATURES_H_


#ifdef FJ_USE_OPENGL
#   include <fejix_runtime/internal/features/opengl.h>
#endif

#ifdef FJ_USE_VARSIZE
#   include <fejix_runtime/internal/features/varsize.h>
#endif


#endif // FEJIX_RUNTIME_FEATURES_H_