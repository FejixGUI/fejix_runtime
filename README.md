# 🛠️ Fejix Runtime 🛠️

> Work in progress ⌛...

This is a C library intended to aid creating GUI.
It is not supposed to be a complete GUI framework because of specificity of C
and difficulties of porting complete frameworks to other languages.

This library is supposed to handle:
- window management
- simple graphics rendering (via wrapping another library)
- laying out widgets (more precisely, making it simple and providing a set of default layouts)
- global UI events (key presses, DPI changes)
- localising cursor movement and clicks to specific widgets
- and other small things

## Build

```sh
cmake -B build -D <OPTION1>=<VALUE1> -D <OPTION2>=<VALUE2> ...
cmake --build build
```

Choosing options is crucial for building Fejix because it does not have default options.

See [BUILD.md](./BUILD.md) for full list of options and build requirements.