#include <fejix_runtime/fejix.h>

#include <fejix_layout.h>


#define L(w) (w)->contentLength
#define I(w) (w)->_contentIndex

#define SAFE_LAYOUT(WGT, ARG)       \
    (WGT->layout)                   \
    ? (WGT->layout(WGT, ARG))       \
    : (_defaultLayout(WGT, ARG));


static void _defaultLayout(struct FjWidget *self, uint32_t mode)
{

#define FIRST_CHILD(w) (w)->content[0]
#define GEOM(w) (w)->_geometry
#define CONST(w) (w)->constraints

    // No content
    if (L(self) == 0) {
        GEOM(self).x1 = CONST(self).x1;
        GEOM(self).y1 = CONST(self).y1;
        GEOM(self).x2 = CONST(self).x2;
        GEOM(self).y2 = CONST(self).y2;
        return;
    }

    // HACK
    // This works because:
    // Traversal #1 - we copy MIN and MAX sizes
    // Traversal #2 - we copy EXACT size and position
    GEOM(FIRST_CHILD(self)).x1 = GEOM(self).x1;
    GEOM(FIRST_CHILD(self)).y1 = GEOM(self).y1;
    GEOM(FIRST_CHILD(self)).x2 = GEOM(self).x2;
    GEOM(FIRST_CHILD(self)).y2 = GEOM(self).y2;

    // This is the root widget
    if (self->container == NULL && mode == FJ_LAYOUT_EXACT) {
        GEOM(self).x1 = 0;
        GEOM(self).y1 = 0;
    }

#undef FIRST_CHILD
#undef GEOM
#undef CONST
}


/**
 * Handles the situation when root is NULL
 * 
 * Assumptions:
 * 1. Root widget has NULL container
 * 2. Every layout() method handles the situation when a widget has no content
 * 3. layout() method of the root widget:
 *    - sets its own X/Y to 0
 *    - receives W/H from _geometry.x/y2
 *    - sets its own exact W/H
 * 4. All widgets reference their container
 * 5. All widgets' content elements are non-NULL
 * 6. If a widget has no layout() method, the _defaultLayout() is used
 *    P.S. Never leave your layout() method empty!
 */
void _fjLayout(struct FjWidget *root, uint32_t windowW, uint32_t windowH)
{
    struct FjWidget *wgt = root;

    if (wgt == NULL)
        return;

    I(wgt) = 0;
    wgt->_geometry.x2 = windowW;
    wgt->_geometry.y2 = windowH;

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
