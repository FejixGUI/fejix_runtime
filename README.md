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

Currently, the only way to build Fejix is to use CMake:
```sh
cmake -B build
cmake --build build
```

I would appreciate adding support for other build systems.
If you decide to add one, do not pollute the `src` or `include` directories with build scripts. Use `tools` instead.