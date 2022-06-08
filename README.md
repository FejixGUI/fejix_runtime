# 🛠️ Fejix Runtime 🛠️

> Work in progress ⌛...

This is a C library intended to aid creating GUI.
It is not supposed to be a complete GUI framework because of specificity of C
and difficulties of porting complete frameworks to other languages.

This library is supposed to handle:
- window management
- rendering widgets
- laying out widgets (more precisely, making it simple
  and providing a set of default layouts)
- global UI events (key presses, DPI changes)
- localising cursor movement and clicks to specific widgets

Allocation of windows and widgets is not handled by Fejix runtime. 

## Build

```sh
cmake -B build
cmake --build build
```

### Build requirements

**X11:**

* `libx11-xcb-dev`
* `libxcb-sync-dev`
* `libxcb-icccm4-dev`