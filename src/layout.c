#include <fejix_runtime/fejix.h>

#include <fejix_private/layout.h>
#include <fejix_private/traverse_widgets.h>

static int layoutMax(struct FjWidget *wgt, void *data) {
    if (wgt->layout == NULL)
        return 1;
    
    wgt->layout(wgt, FJ_LAYOUT_MAX);

    return 0;
}

static int layoutMin(struct FjWidget *wgt, void *data) {
    if (wgt->layout == NULL)
        return 1;
    
    wgt->layout(wgt, FJ_LAYOUT_MIN);

    return 0;
}

static int layoutExact(struct FjWidget *wgt, void *data) {
    if (wgt->layout == NULL)
        return 1;
    
    wgt->layout(wgt, FJ_LAYOUT_EXACT);

    return 0;
}

void fjLayout(struct FjWidget *root, uint32_t windowW, uint32_t windowH)
{
    root->_geometry.w = windowW;
    root->_geometry.h = windowH;

    fjTraverseWidgets(root, &layoutMax, &layoutMin, NULL);
    fjTraverseWidgets(root, &layoutExact, NULL, NULL);
}
