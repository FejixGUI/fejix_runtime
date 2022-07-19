| Option | Description | Requirements |
| --- | --- | --- |
| `FEJIX_USE_X11` | Use X11 protocol for window management (Usually, targeting GNU/Linux distros) | [Link](#x11-requirements) |
| `FEJIX_USE_WINAPI` | Use Windows API for window management (Usually, targeting MS Windows) | None |
| `FEJIX_USE_OPENGL3` | Include functionality for setting up OpenGL 3 | On Windows, make sure you have `opengl32` library |

## X11 Requirements

Install the following packages:

* `libx11-dev`
* `libxcb-dev`
* `libx11-xcb-dev`
* `libxcb-sync-dev`
* `libxcb-icccm4-dev`