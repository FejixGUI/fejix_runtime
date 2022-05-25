#include <fejix_runtime/fejix_stdui.h>

void stduiRowLayout(struct FjWidget *self, uint32_t layoutMode)
{
    switch (layoutMode) {
    case FJ_LAYOUT_MODE_MAX_SIZE:
        for (int i=0; i<self->contentLength; i++)
        {
            self->content[i]->geometry.box2.x = 0;//TODO
        }
    break;

    case FJ_LAYOUT_MODE_MIN_SIZE:
    break;

    case FJ_LAYOUT_MODE_FINAL:
    break;

    default: return;
    }
}