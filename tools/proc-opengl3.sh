#!/usr/bin/sh

# Run this every time you want to convert your GLSL to .c files

TOOLS_DIR=$(realpath $(dirname "$0"))
ROOT_DIR=$(dirname "$TOOLS_DIR")
DRAWER="$ROOT_DIR/src/fejix_drawer"

PY=python3
CVT="$TOOLS_DIR/convert_shaders.py"

$PY $CVT "$DRAWER/opengl3/shaders.glsl" "$DRAWER/opengl3/shaders.c"