#include <fejix_runtime/internal/backend.h>

#include <fejix_private/nanovg/drawing.h>
#include <fejix_private/nanovg/nanovg.h>

#include <nanovg.h>

#define SELF struct FjDrawContext *self
#define WINDOW_DATA(VARIABLE_NAME) \
    struct FjBackendWindowData_nanovg *VARIABLE_NAME = self->windowData

static void setColor(SELF, float r, float g, float b, float a)
{
    WINDOW_DATA(data);
    data->color = (NVGcolor){r,g,b,a};
}

static void beginPath(SELF)
{
    WINDOW_DATA(data);
    nvgBeginPath(data->nvgctx);
}

static void rect(SELF, int32_t x, int32_t y, int32_t w, int32_t h)
{
    WINDOW_DATA(data);
    nvgRect(data->nvgctx, (float)x, (float)y, (float)w, (float)h);
}

static void rrect(
    SELF,
    int32_t x, int32_t y, int32_t w, int32_t h,
    float radius
)
{
    WINDOW_DATA(data);
    nvgRoundedRect(
        data->nvgctx,
        (float)x, (float)y, (float)w, (float)h,
        radius
    );
}

void fill(SELF)
{
    WINDOW_DATA(data);
    nvgFillColor(data->nvgctx, data->color);
    nvgFill(data->nvgctx);
}

void stroke(SELF, float width)
{
    WINDOW_DATA(data);
    nvgStrokeWidth(data->nvgctx, width);
    nvgStrokeColor(data->nvgctx, data->color);
    nvgStroke(data->nvgctx);
}

void fjDrawContextInit_nanovg(struct FjDrawContext *ctx)
{
    *ctx = (struct FjDrawContext) {
        ctx->windowData,
        &beginPath,
        &setColor,
        &rect,
        &rrect,
        &fill,
        &stroke
    };
}
