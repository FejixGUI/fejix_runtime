# Code style

> This notes are mostly for myself, a still incompetent C user. 

> Any suggestions are welcome.

## Naming
| Style | For |
| --- | --- |
| `fjPascalCase` | Functions |
| `FjPascalCase` | Types |
| `FJ_SCREAMING_SNAKE_CASE` | Macros and enum members |
| `_FJ_HEADER_NAME_H_` | Header guards |
| `_whatever` | Private/local things that nobody needs to see |

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
And put a typedef into [fejix_aliases.h](include/fejix_runtime/fejix_aliases.h):
```c
_ALIAS(SomeStruct)
```
If the user wants a typedef, they will include the header.
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
All Fejix headers should be accessed through
the [fejix_runtime directory](include/fejix_runtime):
```c
#include <fejix_runtime/fejix.h> // <- Yes
#include <fejix.h> // <- No
```
Perhaps, user code might not do the same way, however the library's code should.

## Numeric types
Always use the integral types from `<stdint.h>` in public interfaces (fields of
structures, argument types, etc.). Private things like `for` loop counters
may be given types at your discretion.

## Abbreviation
The are a few allowed words which you can shorten:

| Abbreviation | Full |
| --- | --- |
| V | Version |
| Bk | Backend |
| Fj | Fejix |
| Wm | Window manager/management |
| Init | Initialize |

Normally only the first letter of abbreviations are capitalized:
Html, Php, Http.

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