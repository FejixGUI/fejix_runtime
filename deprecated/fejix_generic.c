#include <fejix_runtime/fejix.h>

#define I(w) (w)->contentIndex
#define L(w) (w)->contentLength
#define BOX1(w) (w)->geometry.box1
#define BOX2(w) (w)->geometry.box2
// #define SIZEMIN(w) BOX1(w)
// #define SIZEMAX(w) BOX2(w)
// #define POS(w) BOX1(w)
// #define SIZE(w) BOX2(w)
#define SAFE_LAYOUT(WGT, ARG) \
    (WGT->layout) ? (WGT->layout(WGT, ARG)) : (_defaultLayout(WGT, ARG));

void fjRootSetColor(struct FjRoot *root, float R, float G, float B, float A)
{
    root->background.r = R;
    root->background.g = G;
    root->background.b = B;
    root->background.a = A;
}


static void _defaultLayout(struct FjWidget *self, uint32_t mode)
{
    if (L(self) == 0)
        return;

    BOX1(self->content[0]).x = BOX1(self).x;
    BOX1(self->content[0]).y = BOX1(self).y;
    BOX2(self->content[0]).x = BOX2(self).x;
    BOX2(self->content[0]).y = BOX2(self).y;
}


/** Assumptions:
 * 1. contentIndex of every widget is 0 (initialize widgets with zeros!)
 * 2. rootWidget has NULL container
 * 3. every layout() method handles the situation when a widget has no content
 * 4. all widgets reference their container
 * 5. all widgets' content elements are non-NULL
 * 6. if a widget has no layout() method, the _defaultLayout is used
 * 
 * To satisfy all the requirements, just do the following:
 * 1. Initialize widgets with 0
 * 2. Handle content absence in layout() methods
 * And everybody will be happy
 */
void fjLayout(struct FjRoot *root)
{
    struct FjWidget *wgt;
    
    wgt = root->rootWidget;

    // Perform 2 depth-first traversals

    // 1st traversal - min/max sizes
    while (1)
    {
        // This happens after the traversal is completed
        // (after the rootWidget and all its content is visited)
        if (wgt == NULL)
            break;
        
        // This widget is visited the first time
        if (I(wgt) == 0)
            SAFE_LAYOUT(wgt, FJ_LAYOUT_MODE_MAX_SIZE);

        // We are finished with this widget [and its content]
        if (I(wgt) == L(wgt)) {
            I(wgt) = 0; // Clean up. Be nice to yourself
            SAFE_LAYOUT(wgt, FJ_LAYOUT_MODE_MIN_SIZE);
            wgt = wgt->container; // Go up the tree
            continue;
        }

        // This widget has content
        if (L(wgt) > 0) {
            I(wgt)++;
            wgt = wgt->content[I(wgt) - 1]; // Go down the tree
            //I(wgt) = 0; // Unnecessary (remember how we cleaned everything up)
        }
    }

    wgt = root->rootWidget;

    // 2nd traversal - exact sizes and positions
    while (1)
    {
        if (wgt == NULL)
            break;

        if (I(wgt) == 0)
            SAFE_LAYOUT(wgt, FJ_LAYOUT_MODE_FINAL);

        if (I(wgt) == L(wgt)) {
            I(wgt) = 0;
            wgt = wgt->container;
            continue;
        }

        if (L(wgt) > 0) {
            I(wgt)++;
            wgt = wgt->content[I(wgt) - 1];
        }
    }
}
