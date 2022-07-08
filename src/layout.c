#include <fejix_runtime/fejix.h>

#include <fejix_runtime/internal/layout.h>


#define L(w) (w)->contentLength
#define I(w) (w)->_contentIndex

#define SAFE_LAYOUT(WGT, ARG)       \
    (WGT->layout)                   \
    ? (WGT->layout(WGT, ARG))       \
    : (_defaultLayout(WGT, ARG));


static void _defaultLayout(struct FjWidget *self, uint32_t mode)
{

#define FIRST_CHILD(w) (w)->content[0]

    switch (mode) {
        case FJ_LAYOUT_MAX:
            self->_tmpConstraints.maxW = self->constraints.maxW;
            self->_tmpConstraints.maxH = self->constraints.maxH;
        break;

        case FJ_LAYOUT_MIN:
            self->_tmpConstraints.minW = self->constraints.minW;
            self->_tmpConstraints.minH = self->constraints.minH;
        break;

        case FJ_LAYOUT_EXACT:
            if (self->container != NULL) {
                self->_geometry.x = self->container->_geometry.x;
                self->_geometry.y = self->container->_geometry.y;
            } else {
                self->_geometry.x = 0;
                self->_geometry.y = 0;
            }

            self->_geometry.w = self->_tmpConstraints.maxW;
            self->_geometry.h = self->_tmpConstraints.maxH;
        break;
    }

#undef FIRST_CHILD
}

/// FIXME
/// TODO
void _fjLayout(struct FjWidget *root, uint32_t windowW, uint32_t windowH)
{
    struct FjWidget *wgt = root;

    if (wgt == NULL)
        return;

    I(wgt) = 0;
    wgt->_geometry.w = windowW;
    wgt->_geometry.h = windowH;

    // Perform 2 depth-first traversals

    // 1st traversal - min/max sizes
    while (1)
    {
        // This happens after the traversal is completed
        // (after the root widget and all its content is visited)
        if (wgt == NULL)
            break;
        
        // This widget is visited the first time
        if (I(wgt) == 0)
            SAFE_LAYOUT(wgt, FJ_LAYOUT_MAX);

        // We are finished with this widget [and its content]
        if (I(wgt) == L(wgt)) {
            I(wgt) = 0; // Clean up
            SAFE_LAYOUT(wgt, FJ_LAYOUT_MIN);
            wgt = wgt->container; // Go up the tree
            continue;
        }

        // This widget has content
        if (L(wgt) > 0) {
            I(wgt)++;
            wgt = wgt->content[I(wgt) - 1]; // Go down the tree
            I(wgt) = 0; // Prepare for next iteration
        }
    }

    wgt = root;

    // 2nd traversal - exact sizes and positions
    while (1)
    {
        if (wgt == NULL)
            break;

        if (I(wgt) == 0)
            SAFE_LAYOUT(wgt, FJ_LAYOUT_EXACT);

        if (I(wgt) == L(wgt)) {
            I(wgt) = 0;
            wgt = wgt->container;
            continue;
        }

        if (L(wgt) > 0) {
            I(wgt)++;
            wgt = wgt->content[I(wgt) - 1];
            // I(wgt) = 0; // Unnecessary (already 0 after the prev. traversal)
        }
    }
}
