#include <fejix_runtime/fejix.h>
#include <fejix_runtime/stdlayout.h>


#define CONST_MIN_W(WGT) (WGT)->constraints.minW
#define CONST_MIN_H(WGT) (WGT)->constraints.minH
#define CONST_MAX_W(WGT) (WGT)->constraints.maxW
#define CONST_MAX_H(WGT) (WGT)->constraints.maxH

#define MIN_W(WGT) (WGT)->_tmpConstraints.minW
#define MIN_H(WGT) (WGT)->_tmpConstraints.minH
#define MAX_W(WGT) (WGT)->_tmpConstraints.maxW
#define MAX_H(WGT) (WGT)->_tmpConstraints.maxH

#define EXACT_X(WGT) (WGT)->_geometry.x
#define EXACT_Y(WGT) (WGT)->_geometry.y
#define EXACT_W(WGT) (WGT)->_geometry.w
#define EXACT_H(WGT) (WGT)->_geometry.h

#define LEN(WGT) ((WGT)->contentLength)
#define NO_CONTENT(WGT) (LEN(WGT) == 0)
#define FIRST_CHILD(WGT) ((WGT)->content[0])
#define CHILD(WGT, INDEX) ((WGT)->content[INDEX])
#define FOR_EACH_CHILD(WGT, COUNTER) \
    for(int COUNTER=0; COUNTER<LEN(WGT); COUNTER++)

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

/// Example usage: CONSTRAIN(EXACT_W(self), MIN_W(CHILD(i)), MAX_W(CHILD(i)))
#define CONSTRAIN(_VALUE, _MIN, _MAX) MAX(MIN(_VALUE, _MAX), _MIN)

void fjStdSelfLayout(struct FjWidget *self, uint32_t mode)
{
    switch (mode) {
        case FJ_LAYOUT_MIN:
            MIN_W(self) = CONST_MIN_W(self);
            MIN_H(self) = CONST_MIN_H(self);
        break;
    }
}

/// Does not use size constraints
void fjStdRootLayout(struct FjWidget *self, uint32_t mode)
{
    if (NO_CONTENT(self)) {
        EXACT_X(self) = 0;
        EXACT_Y(self) = 0;
        return;
    }

    switch (mode) {
        case FJ_LAYOUT_MAX:
            MAX_W(FIRST_CHILD(self)) = CONSTRAIN(
                EXACT_W(self),
                CONST_MIN_W(FIRST_CHILD(self)),
                CONST_MAX_W(FIRST_CHILD(self))
            );
            MAX_H(FIRST_CHILD(self)) = CONSTRAIN(
                EXACT_H(self),
                CONST_MIN_H(FIRST_CHILD(self)),
                CONST_MAX_H(FIRST_CHILD(self))
            );
        break;

        case FJ_LAYOUT_MIN:
            MIN_W(self) = MIN_W(FIRST_CHILD(self));
            MIN_H(self) = MIN_H(FIRST_CHILD(self));
        break;

        case FJ_LAYOUT_EXACT:
        {
            int32_t minW = MIN_W(self);
            int32_t minH = MIN_H(self);
            int32_t childMinW = MIN_W(FIRST_CHILD(self));
            int32_t childMinH = MIN_H(FIRST_CHILD(self));

            EXACT_X(self) = 0;
            EXACT_Y(self) = 0;

            EXACT_X(FIRST_CHILD(self)) = 0;
            EXACT_Y(FIRST_CHILD(self)) = 0;

            EXACT_W(self) = MAX(minW, EXACT_W(self));
            EXACT_H(self) = MAX(minH, EXACT_H(self));

            EXACT_W(FIRST_CHILD(self)) = CONSTRAIN(
                EXACT_W(self),
                MIN_W(FIRST_CHILD(self)),
                MAX_W(FIRST_CHILD(self))
            );
            EXACT_H(FIRST_CHILD(self)) = CONSTRAIN(
                EXACT_H(self),
                MIN_H(FIRST_CHILD(self)),
                MAX_H(FIRST_CHILD(self))
            );
        }
        break;
    }
}


#if 0
void fjStdRowLayout(struct FjWidget *self, uint32_t mode)
{
    if (NO_CONTENT(self))
        return;

    if (self->data == NULL)
        return;

    struct FjStdRowData *rowData = self->data;

    switch (mode) {
        case FJ_LAYOUT_MAX:
            FOR_EACH_CHILD(self, i)
            {
                MAX_W(CHILD(self, i)) = CONSTRAIN(
                    MAX_W(self),
                    CONST_MIN_W(CHILD(self, i)),
                    CONST_MAX_W(CHILD(self, i))
                );

                MAX_H(CHILD(self, i)) = CONSTRAIN(
                    MAX_H(self),
                    CONST_MIN_H(CHILD(self, i)),
                    CONST_MAX_H(CHILD(self, i))
                );
            }
        break;

        case FJ_LAYOUT_MIN:
        {
            int32_t minW = CONST_MIN_W(self) + 2 * rowData->padding.x;
            int32_t minH = CONST_MIN_H(self) + 2 * rowData->padding.y;

            if (rowData->orientation == FJ_VERTICAL) {
                FOR_EACH_CHILD(self, i) {
                    minW = MAX(MIN_W(CHILD(self, i)), minW);
                    minH += MIN_H(CHILD(self, i));
                    
                    if (i != 0)
                        minH += rowData->spacing;
                }
            } else {
                FOR_EACH_CHILD(self, i) {
                    minW += MIN_W(CHILD(self, i));
                    minH = MAX(MIN_H(CHILD(self, i)), minH);
                    
                    if (i != 0)
                        minW += rowData->spacing;
                }
            }

            MIN_W(self) = minW;
            MIN_H(self) = minH;
        }
        break;

        case FJ_LAYOUT_EXACT:
        {
            if (rowData->orientation == FJ_VERTICAL) {
                int32_t availableH = MAX_H(self) - MIN_H(self);

                int32_t allWeights = 0;
                FOR_EACH_CHILD(self, i) {
                    allWeights += CHILD(self, i)->weights.y;
                }

                double oneWeight = (double) availableH / allWeights;

                FOR_EACH_CHILD(self, i) {
                    EXACT_W(CHILD(self, i)) = CONSTRAIN(
                        EXACT_W(self),
                        MIN_W(CHILD(self, i)),
                        MAX_W(CHILD(self, i))
                    );

                    // EXACT_H(CHILD(i)) = 
                }
            }
        }
        break;
    }
}
#endif 