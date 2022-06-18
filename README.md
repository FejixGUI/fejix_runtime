## Temporarily abandoned in favor of the Rust programming language and its numerous ready libraries

I am switching to Rust in order to get a working prototype as soon as possible.

After I settle down my imagination, ideas and ambitions, I will continue to
develop this C library.

Please, note that the progress I made is not lost. It will just be translated
to Rust.

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


### Additional steps

In order to convert `.glsl` shaders to `.c` files (simply generate C strings)
you need to run:
```
./tools/proc-opengl3.sh
```