#include <fejix_runtime/definitions.h>

#include <fejix_private/traverse_widgets.h>


void fjTraverseWidgets(
    struct FjWidget *root,
    FjWidgetFn widgetFn,
    void *data
)
{
    struct FjWidget *wgt = root;

    if (wgt == NULL)
        return;

#define L(w) (w)->contentLength
#define I(w) (w)->_contentIndex

    I(wgt) = 0;

    while (1)
    {
        // This happens after the traversal is completed
        // (after the root widget and all its content is visited)
        if (wgt == NULL)
            break;
        
        // This widget is visited the first time
        if (I(wgt) == 0)
            if (widgetFn != NULL) {
                int doNotTraverseContent = widgetFn(wgt, 0, data);
                if (doNotTraverseContent)
                    I(wgt) = L(wgt); // Pretent that we finished this node
            }

        // We are finished with this widget [and its content]
        if (I(wgt) == L(wgt)) {
            I(wgt) = 0; // Clean up
            if (widgetFn != NULL) widgetFn(wgt, 1, data);
            wgt = wgt->container; // Go up the tree

            if (wgt)
                I(wgt)++; // Show that one more child widget was traversed

            continue;
        }

        // This widget has content
        if (L(wgt) > 0) {
            wgt = wgt->content[I(wgt)]; // Go down the tree
            I(wgt) = 0; // Prepare for next iteration
        }
    }

#undef I
#undef L
}