#include <fejix_runtime/fejix.h>

#include <fejix_private_defines.h>
#include <fejix_drawer/opengl3/opengl3.h>

#include <fejix_drawer/opengl3/shaders.c>


#include <stdio.h>


static GLfloat shape[] = {
    40,100,
    60,80,
    100,60,
    160,80,
    200,120,
    240,100,
    260,40,
    300,20,
    320,80,
    280,140,
    240,200,
    200,220,
    160,200,
    100,160,
    60,160,
    80,240,
    40,200,
    20,140
};



static GLfloat verteces[] = {
    0.f,   0.f,
    400.f, 0.f,
    0.f,   500.f,
    400.f, 500.f
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


static GLuint makeProgram(const char *vertexSrc, const char *fragmentSrc)
{
    GLuint vertexShader, fragmentShader, program;
    GLint success;
    char log[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, STATIC_LEN(log), NULL, log);
        puts(log);
        return FJ_ERR_BACKEND_FAILED;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, STATIC_LEN(log), NULL, log);
        puts(log);
        return FJ_ERR_BACKEND_FAILED;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, STATIC_LEN(log), NULL, log);
        puts(log);
        return FJ_ERR_BACKEND_FAILED;
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, STATIC_LEN(log), NULL, log);
        puts(log);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}



uint32_t _fjDrawContextInit_opengl3(struct FjBackendDrawContext_opengl3 *ctx)
{
    ctx->program = makeProgram(_vertexShader, _fragmentShader);

    glGenVertexArrays(1, &ctx->fillVAO);
    glBindVertexArray(ctx->fillVAO);

    glEnableVertexAttribArray(0);
    glGenBuffers(1, &ctx->fillVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ctx->fillVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        SIZEOF_BITS(verteces),
        verteces,
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(
        0, 2,
        GL_FLOAT,
        GL_FALSE,
        0,
        NULL
    );
    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &ctx->shapeVAO);
    glBindVertexArray(ctx->shapeVAO);

    glGenBuffers(1, &ctx->shapeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, ctx->shapeVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        SIZEOF_BITS(shape),
        shape,
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, ctx->shapeVBO);
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

    // glUseProgram(ctx->program);
    ctx->u_screenSize = glGetUniformLocation(ctx->program, "u_screenSize");
    // glUseProgram(0);

    return FJ_OK;
}


void _fjDrawContextDestroy_opengl3(struct FjBackendDrawContext_opengl3 *ctx)
{
    glDeleteProgram(ctx->program);
    glDeleteVertexArrays(1, &ctx->fillVAO);
    glDeleteBuffers(1, &ctx->shapeVBO);
    glDeleteBuffers(1, &ctx->fillVBO);
}


void _fjDraw_opengl3(struct FjBackendDrawContext_opengl3 *ctx)
{
    glViewport(0, 0, ctx->width, ctx->height);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(ctx->program);

    glUniform2f(ctx->u_screenSize, (float)ctx->width, (float)ctx->height);

    glBindVertexArray(ctx->shapeVAO);
    glEnableVertexAttribArray(0);
    glEnable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilFunc(GL_NEVER, 0, 0xFF);
    glStencilOp(GL_INVERT, GL_KEEP, GL_KEEP);
    glDrawArrays(GL_TRIANGLE_FAN, 0, STATIC_LEN(shape)/2);
    glDisableVertexAttribArray(0);

    glBindVertexArray(ctx->fillVAO);
    glEnableVertexAttribArray(0);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(0);

    glBindVertexArray(0);
    
    glUseProgram(0);
}