// Run with tools/convert_shaders.py

@begin _vertexShader

    #version 330 core

    layout (location=0) in vec2 in_pos;

    uniform vec2 u_screenSize;

    void main() {
        gl_Position = vec4(
            (in_pos.xy                  // (0..W/H)
            / u_screenSize * 2.0 - 1.0) // (-1..1)
            * vec2(1.0, -1.0),          // Y is counted from the bottom
            0.0,
            1.0
        );
    }

@end


@begin _fragmentShader

    #version 330 core

    out vec4 out_color;

    void main() {
        out_color = vec4(1.0, 0.5, 0.0, 1.0);
    }

@end