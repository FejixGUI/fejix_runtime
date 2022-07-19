#ifndef FEJIX_RUNTIME_TYPEDEFS_H_
#define FEJIX_RUNTIME_TYPEDEFS_H_

#define FJ__TYPEDEF(X) typedef struct X X;

FJ__TYPEDEF(FjApp)
FJ__TYPEDEF(FjWindow)
FJ__TYPEDEF(FjWindowParams)
FJ__TYPEDEF(FjEvent)

#undef FJ__TYPEDEF

#endif // FEJIX_RUNTIME_TYPEDEFS_H_