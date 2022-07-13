#ifndef _FEJIX_STDLAYOUT_H_
#define _FEJIX_STDLAYOUT_H_

#include <fejix_runtime/definitions.h>


/// Row | Column
struct FjStdLinearLayoutData {
    /// FJ_HORIZONTAL or FJ_VERTICAL
    uint32_t orientation;

    /// Space between elements
    uint32_t spacing;

    /// Vertical and horizontal space around the elements
    struct FjPoint padding;

    // Minimum size of content along the main axis
    uint32_t _minSize;

    // Exact size of padding+spacing along the main axis
    uint32_t _spaceSize;
};

void fjStdRootLayout(struct FjWidget *self, uint32_t mode);
void fjStdSelfLayout(struct FjWidget *self, uint32_t mode);
void fjStdLinearLayout(struct FjWidget *self, uint32_t mode);
void fjStdCenterLayout(struct FjWidget *self, uint32_t mode);
void fjStdAlignLayout(struct FjWidget *self, uint32_t mode);


#endif // _FEJIX_STDLAYOUT_H_