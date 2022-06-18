#ifndef _FEJIX_STDUI_H_
#define _FEJIX_STDUI_H_

#include <fejix_runtime/fejix_defines.h>

struct FjWidget;

void fjStdRootLayout(struct FjWidget *self, uint32_t mode);


/// Row | Column
struct FjStdRowData {
    /// FJ_HORIZONTAL or FJ_VERTICAL
    uint32_t orientation;

    /// Space between elements
    uint32_t spacing;

    /// Vertical and horizontal space between elements and row
    struct FjXY padding;
};

#define FJ_HORIZONTAL 0
#define FJ_VERTICAL   1

void fjStdRowLayout(struct FjWidget *self, uint32_t mode);

#endif // _FEJIX_STDUI_H_