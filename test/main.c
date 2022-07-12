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

FjWidget linearContainer;

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
    // linearContainer = {0},
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
        .minWidth = 100,
        .minHeight = 70,
        .isResizable = 1
    };

    _(fjAppInitWindow(&app, &win, &winParams));

    _(fjWindowSetTitle(&win, "Це працює!"));

    win.root = &root;
    FjWidget *rootContent[] = {&linearContainer};
    root.content = rootContent;
    root.contentLength = arrlen(rootContent);
    root.layout = &fjStdRootLayout;

    FjWidget *widgets[] = {&wgt1, &wgt2};
    FjStdLinearLayoutData data = {
        .orientation = FJ_VERTICAL,
        .spacing = 20,
        .padding = {.x=70, .y=70}
    };
    linearContainer.layout = &fjStdLinearLayout;
    linearContainer.content = widgets;
    linearContainer.contentLength = arrlen(widgets);
    linearContainer.container = &root;
    linearContainer.data = &data;
    linearContainer.constraints = (FjConstraints){0,0,FJ_EXPAND,FJ_EXPAND};

    wgt1.constraints = (FjConstraints){50,50,500,500};
    wgt1.layout = &fjStdSelfLayout;
    wgt1.draw = &drawRectangle1;
    wgt1.weights = (FjXY){1,1};
    wgt1.container = &linearContainer;

    wgt2.constraints = (FjConstraints){50,50,500,500};
    wgt2.layout = &fjStdSelfLayout;
    wgt2.draw = &drawRectangle2;
    wgt2.weights = (FjXY){1,1};
    wgt2.container = &linearContainer;

    fjWindowSetVisible(&win, true);

    FjWindow *windows[] = { &win };
    fjAppSetWindows(&app, windows, arrlen(windows));
    fjLoop(&app, &handleEvent);

    fjWindowDestroy(&win);

    fjAppDestroy(&app);

    return 0;
}