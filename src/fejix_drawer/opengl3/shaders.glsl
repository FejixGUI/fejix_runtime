// Run with tools/convert_shaders.py

@begin vertexShader

    #version 330 core

    layout (location=0) in vec2 in_pos;

    void main() {
        gl_Position = vec4(in_pos, 0.0, 1.0);
    }

@end


@begin fragmentShader

    #version 330 core

    out vec3 out_color;

    void main() {
        out_color = vec4(1.0, 0.8, 0.0, 1.0);
    }

@end