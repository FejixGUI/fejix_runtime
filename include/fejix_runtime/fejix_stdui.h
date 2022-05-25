#ifndef _FEJIX_STDUI_H_
#define _FEJIX_STDUI_H_


#include <fejix_runtime/fejix.h>

// Row/column
struct stduiStackData {
    uint32_t spacing;
    struct FjBox padding;
};

void stduiRowLayout(struct FjWidget *self, uint32_t layoutMode);


#endif // _FEJIX_STDUI_H_