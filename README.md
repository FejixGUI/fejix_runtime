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

## Build

1. Install Xmake [like this](https://xmake.io/#/getting_started?id=installation).
2. Configure:
   ```sh
   xmake config --menu
   ```
3. Build a static library:
   ```sh
   xmake build fejix_runtime
   ```