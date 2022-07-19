# Project structure

* [include](./include/) - all headers
  * [fejix_runtime](./include/fejix_runtime/) - public header files
    * [internal](./include/fejix_runtime/internal/) - private header files that are to be included in public header files
  * [fejix_runtime_dev](./src/fejix_runtime_dev/) - development-only headers
* [src](./src/) - source code
  * [platform](./src/platform/) - windowing functionality implementations
* [deps](./deps/) - dependencies
* [test](./test/) - development test(s)
* [tools](./tools/) - tools, including build tools

You should always include `fejix_runtime` directory when including files:
```c
// Good
#include <fejix_runtime/fejix_runtime.h>

// Bad!
#include <fejix_runtime.h>
```