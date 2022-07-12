#ifndef _FEJIX_PRIVATE_TRAVERSE_WIDGETS_H_
#define _FEJIX_PRIVATE_TRAVERSE_WIDGETS_H_

struct FjWidget;

/// @param down 0 if the wgt is discovered (currently going down), 1 otherwise
/// @returns 1 if content of the widget must not be traversed, 0 otherwise
typedef int (*FjWidgetFn)(struct FjWidget *wgt, int up, void *data);

/**
 * @brief Performs a depth-first widget tree traversal.
 * 
 * @param root The root widget
 * @param downFn The function which is called when a widget is discovered
 * @param upFn The function which is called when a widget is leaved
 * @param data Data that will be passed to downFn and upFn
 */
void fjTraverseWidgets(
    struct FjWidget *root,
    FjWidgetFn widgetFn,
    void *data
);

#endif // _FEJIX_PRIVATE_TRAVERSE_WIDGETS_H_