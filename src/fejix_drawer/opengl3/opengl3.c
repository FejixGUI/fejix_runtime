#include <fejix_runtime/fejix.h>

#include <fejix_private.h>
#include <fejix_drawer/opengl3/opengl3.h>

#include <fejix_drawer/opengl3/shaders.c>


#include <stdio.h>


static GLfloat
verteces[] = {
    0.f, 0.f,
    1.f, 0.f,
    0.f, 1.f,
    1.f, 1.f
};


static void _debug(int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        const char *msg = "unknown error";
        switch (errorCode) {
            case GL_INVALID_ENUM: msg = "invalid enum";
                break;
            case GL_INVALID_VALUE: msg = "invalid value";
                break;
            case GL_INVALID_OPERATION: msg = "invalid operation";
                break;
            case GL_OUT_OF_MEMORY: msg = "out of memory";
                break;
        }
        printf("OpenGL error on line %d: #%d - %s\n",
            line,
            (int) errorCode,
            msg
        );
    }
}
#define DBG _debug(__LINE__);


uint32_t _fjDrawContextInit_opengl3(struct FjBackendDrawContext_opengl3 *ctx)
{
    GLuint vertexShader, fragmentShader, program;
    GLint success;
    char log[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &_vertexShader, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, STATIC_LEN(log), NULL, log);
        puts(log);
        return FJ_ERR_BACKEND_FAIL;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &_fragmentShader, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, STATIC_LEN(log), NULL, log);
        puts(log);
        return FJ_ERR_BACKEND_FAIL;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, STATIC_LEN(log), NULL, log);
        puts(log);
        return FJ_ERR_BACKEND_FAIL;
    }

    ctx->program = program;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    glGenVertexArrays(1, &ctx->shapeVAO);
    glBindVertexArray(ctx->shapeVAO);

    glGenBuffers(1, &ctx->shapeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ctx->shapeVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        8 * sizeof(verteces),
        verteces,
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        0, 2,
        GL_FLOAT,
        GL_FALSE,
        0,
        NULL
    );

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return FJ_OK;
}


void _fjDrawContextDestroy_opengl3(struct FjBackendDrawContext_opengl3 *ctx)
{

}


void _fjDraw_opengl3(struct FjBackendDrawContext_opengl3 *ctx)
{
    glUseProgram(ctx->program);
    glBindVertexArray(ctx->shapeVAO);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(0);
    glBindVertexArray(0);
    glUseProgram(0);
}