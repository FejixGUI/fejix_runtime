| Option | Description | Requirements |
| --- | --- | --- |
| `FEJIX_USE_X11` | Use X11 protocol for window management (Usually, targeting GNU/Linux distros) | [Link](#x11-requirements) |
| `FEJIX_USE_WINAPI` | Use Windows API for window management (Usually, targeting MS Windows) | **-** |
| `FEJIX_USE_OPENGL` | Include functionality for setting up OpenGL | **-**; OpenGL does not by itself have dependencies. You need to also choose a specific OpenGL version. |
| `FEJIX_USE_OPENGL_3` | OpenGL 3.3 | On Windows, make sure you have `opengl32` library |
| `FEJIX_USE_VARSIZE` | Includes functionality that assumes that window sizes are variable and can be changed | **-** |

## X11 Requirements

Install the following packages:

* `libx11-dev`
* `libxcb-dev`
* `libx11-xcb-dev`
* `libxcb-sync-dev`
* `libxcb-icccm4-dev`