#include <fejix_runtime/fejix.h>
#include <fejix_runtime/helper.h>
#include <fejix_runtime/stdlayout.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

#include "debug.h"

#define arrlen(ARRAY) (sizeof(ARRAY) / sizeof(*(ARRAY)))


uint32_t handleEvent(FjWindow *win, FjEvent *ev)
{
    switch (ev->eventType)
    {
        case FJ_EVENT_CLOSE:
            puts("Did you just close the window?");
            fjWindowSetVisible(win, false);
            return FJ_EXIT;
        break;

        case FJ_EVENT_RESIZE:
        {
            char title[32];
            sprintf(
                title,
                "%" PRIu32 "x" "%" PRIu32,
                ev->resizeEvent.width,
                ev->resizeEvent.height
            );
            fjWindowSetTitle(win, title);
        }
        break;
    }

    return FJ_OK;
}

void drawRectangle1(FjWidget *self, FjDrawContext *ctx) {
    ctx->beginPath(ctx);
    ctx->rrect(ctx,
        self->geometry.x,
        self->geometry.y,
        self->geometry.w,
        self->geometry.h,
        20.0f
    );
    ctx->setColor(ctx, 1.0f, 0.7f, 0.0f, 1.0f);
    ctx->fill(ctx);
}

void drawRectangle2(FjWidget *self, FjDrawContext *ctx) {
    ctx->beginPath(ctx);
    ctx->rrect(ctx,
        self->geometry.x,
        self->geometry.y,
        self->geometry.w,
        self->geometry.h,
        20.0f
    );
    ctx->setColor(ctx, 0.0f, 0.5f, 1.0f, 1.0f);
    ctx->fill(ctx);
}


FjApp app = {0};
FjWindow win = {0};

FjWidget
    root = {0},
    container = {0},
    wgt1 = {0},
    wgt2 = {0};




int main(void) {
    printf(
        "Devtest is running Fejix v%d.%d.%d\n",
        FJ_VERSION_MAJOR, FJ_VERSION_MINOR, FJ_VERSION_PATCH
    );

    FjAppParams appParams = {
        .backend0 = FJ_BACKEND_NANOVG,
        .backend1 = FJ_BACKEND_NONE
    };

    _(fjAppInit(&app, &appParams));

    FjWindowParams winParams = {
        .width = 800,
        .height = 600,
        .constraints = {.minW = 100, .minH = 70},
        .isResizable = 1
    };

    _(fjAppInitWindow(&app, &win, &winParams));

    _(fjWindowSetTitle(&win, "Це працює!"));

    win.root = &root;
    FjWidget *rootContent[] = {&container};
    root.content = rootContent;
    root.contentLength = arrlen(rootContent);
    root.layout = &fjStdRootLayout;

    FjWidget *widgets[] = {&wgt1};
    container.layout = &fjStdConstrainLayout;
    container.content = widgets;
    container.contentLength = arrlen(widgets);
    container.container = &root;
    container.draw = &drawRectangle2;
    FjConstraints contData = {.maxW = FJ_BIG, .maxH = 200};
    container.data = &contData;

    wgt1.layout = &fjStdNoLayout;
    wgt1.draw = &drawRectangle1;
    wgt1.weight = 1.f;
    wgt1.container = &container;

    fjWindowSetVisible(&win, true);

    FjWindow *windows[] = { &win };
    fjAppSetWindows(&app, windows, arrlen(windows));
    fjLoop(&app, &handleEvent);

    fjWindowDestroy(&win);

    fjAppDestroy(&app);

    return 0;
}