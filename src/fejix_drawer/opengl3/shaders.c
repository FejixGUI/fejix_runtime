// AUTOGENERATED! DO NOT EDIT!
// Generated by "tools/convert_shaders.py"
// Generated from: "/home/mark/Documents/Projects/fejix_runtime/src/fejix_drawer/opengl3/shaders.glsl"

static const char *_vertexShader =
   "#version 330 core\n"
   "layout (location=0) in vec2 in_pos;\n"
   "void main() {\n"
   "gl_Position = vec4(in_pos.xy, 0.0, 1.0);\n"
   "}\n"
;

static const char *_fragmentShader =
   "#version 330 core\n"
   "out vec4 out_color;\n"
   "void main() {\n"
   "out_color = vec4(1.0, 0.8, 0.0, 1.0);\n"
   "}\n"
;

