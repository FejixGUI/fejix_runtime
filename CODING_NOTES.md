- [Project structure](#project-structure)
- [Fejix Runtime details](#fejix-runtime-details)
- [Code style](#code-style)
  - [Naming](#naming)
  - [Line width](#line-width)
  - [Typedefs](#typedefs)
  - [Forward declarations](#forward-declarations)
  - [Header files path](#header-files-path)
  - [Numeric types](#numeric-types)
  - [Comments](#comments)

# Project structure

* [include](./include/) - all headers
  * [fejix_runtime](./include/fejix_runtime/) - public header files
    * [internal](./include/fejix_runtime/internal/) - private header files that are to be included in public header files
  * [fejix_private](./src/fejix_private/) - development-only headers
* [src](./src/) - source code
  * [platform](./src/platform/) - windowing functionality implementations
  * [backend](./src/backend/) - graphics renderers
* [test](./test/) - development test(s)
* [tools](./tools/) - tools that aid building the project
* [deprecated](./deprecated/) - all deprecated code that may be useful in the future

# Fejix Runtime details

This might be too cryptic, but it is worth to mention.

All platform-specific functionalities like windowing API and OpenGL context creation are organised in the following way:
1. Platform-specific headers define platform-specific data types (`struct`s).
2. Generic headers select the platform-specific header for the current platform and declare generic functions using the types from (1).
3. Platform-specific source files implement the functions declared in (2).

All rendering backends are organised like this:
1. [fejix_runtime/internal/backend.h](include/fejix_runtime/internal/backend.h) defines the `FjBackend` structure
    which contains virtual functions (pointers to functions).
2. The `fjBackendInitApp` function defined in 
   [fejix_private/backend_generic.h](include/fejix_private/backend_generic.h) tries to run all possible
   `fjBackendInitApp_<BACKEND_NAME>` functions when a new App is being created. I said "all possible" because if an
   initialiser fails, `fjBackendInitApp` falls back to another one.

# Code style

> This notes are mostly for myself, a still incompetent C user. 

> Any suggestions are welcome.

## Naming
| Style | For |
| --- | --- |
| `fjCamelCase` | Functions |
| `FjPascalCase` | Types |
| `FJ_SCREAMING_SNAKE_CASE` | Macros and enum members |
| `_FJ_HEADER_NAME_H_` | Header guards |
| `FJ_USE_XXX` | If defined, means that the feature XXX is available |

Normally, only the first letter of any compound noun is capitalized: Nanovg, Opengl, Winapi.

## Line width
Please, do not exceed 80 columns.

## Typedefs
Never define a structure within a typedef:
```c
typedef struct SomeStruct {

} SomeStruct;
```
Instead, define a structure normally:
```c
struct SomeStruct {

};
```
And put a typedef into [fejix_helper.h](include/fejix_runtime/helper.h):
```c
_ALIAS(SomeStruct)
```
If users want a typedef, they will include the header.
Also, do not use typedefs in Fejix code.

## Forward declarations
If you want to create recursive structures like this:
```c
// Defined in A.h
struct A {
    struct B b;
};

// Defined in B.h
struct B {
    struct A a;
};
```
Use forward declarations instead of including "A.h" in "B.h" and vice versa:
```c
//#include <A.h>  <- Please, no
struct A;

struct B {
    struct A a;
};
```

## Header files path
All Fejix *public* headers should be accessed through
the [fejix_runtime directory](include/fejix_runtime):
```c
#include <fejix_runtime/fejix.h> // <- Yes
#include <fejix.h> // <- No
```

All Fejix *private* headers should be accessed through
the [fejix_private directory](src/fejix_private):
```c
#include <fejix_private/whatever.h> // <- Yes
#include "whatever.h" // <- No
```

Organise tests however you like.

## Numeric types
Always use the integral types from `<stdint.h>` in public interfaces (fields of
structures, argument types, etc.). Private things like `for` loop counters
may be given types at your discretion.




## Comments
Never use "banners" in comments for any declarations:
```c
/************************************
 * No, no, no, please, no
 ************************************/
in32_t x;
```
However, it is OK to use "banners" in the beginning of a file, because those
banners might include a _lot_ of documentation and examples. Use exactly
79 stars and no right border:
```c
/*******************************************************************************
 * @file
 * 
 * @brief Some file
 * 
 * ...
*******************************************************************************/
```

