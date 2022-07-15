#include <fejix_runtime/fejix.h>

#include <fejix_private/layout.h>
#include <fejix_private/traverse_widgets.h>

static int layoutMaxMin(struct FjWidget *wgt, int up, void *data) {
    if (wgt->layout == NULL)
        return 1;
    
    wgt->layout(wgt, !up ? FJ_LAYOUT_MAX : FJ_LAYOUT_MIN);

    return 0;
}

static int layoutExact(struct FjWidget *wgt, int up, void *data) {
    if (wgt->layout == NULL)
        return 1;
    
    if (!up)
        wgt->layout(wgt, FJ_LAYOUT_EXACT);

    return 0;
}

void fjLayout(struct FjWidget *root, uint32_t windowW, uint32_t windowH)
{
    if (root == NULL)
        return;

    root->geometry.x = 0;
    root->geometry.y = 0;
    root->geometry.w = windowW;
    root->geometry.h = windowH;

    fjTraverseWidgets(root, &layoutMaxMin, NULL);
    fjTraverseWidgets(root, &layoutExact, NULL);
}
