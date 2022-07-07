#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_stdui.h>


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

#define NO_CONTENT(WGT) ((WGT)->contentLength == 0)
#define FIRST_CHILD (self->content[0])
#define CHILD(INDEX) (self->content[INDEX])
#define LEN (self->contentLength)
#define FOR_EACH_CHILD(COUNTER) for(int COUNTER=0; COUNTER<LEN; COUNTER++)

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

/// Example usage: CONSTRAIN(EXACT_W(self), MIN_W(CHILD(i)), MAX_W(CHILD(i)))
#define CONSTRAIN(_VALUE, _MIN, _MAX) MAX(MIN(_VALUE, _MAX), _MIN)


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
            MAX_W(FIRST_CHILD) = CONSTRAIN(
                EXACT_W(self),
                CONST_MIN_W(FIRST_CHILD),
                CONST_MAX_W(FIRST_CHILD)
            );
            MAX_H(FIRST_CHILD) = CONSTRAIN(
                EXACT_H(self),
                CONST_MIN_H(FIRST_CHILD),
                CONST_MAX_H(FIRST_CHILD)
            );
        break;

        case FJ_LAYOUT_MIN:
            MIN_W(self) = MIN_W(FIRST_CHILD);
            MIN_H(self) = MIN_H(FIRST_CHILD);
        break;

        case FJ_LAYOUT_EXACT:
        {
            int32_t minW = MIN_W(self);
            int32_t minH = MIN_H(self);
            int32_t childMinW = MIN_W(FIRST_CHILD);
            int32_t childMinH = MIN_H(FIRST_CHILD);

            EXACT_X(self) = 0;
            EXACT_Y(self) = 0;

            EXACT_X(FIRST_CHILD) = 0;
            EXACT_Y(FIRST_CHILD) = 0;

            EXACT_W(self) = MAX(minW, EXACT_W(self));
            EXACT_H(self) = MAX(minH, EXACT_H(self));

            EXACT_W(FIRST_CHILD) = CONSTRAIN(
                EXACT_W(self), MIN_W(FIRST_CHILD), MAX_W(FIRST_CHILD)
            );
            EXACT_H(FIRST_CHILD) = CONSTRAIN(
                EXACT_H(self), MIN_H(FIRST_CHILD), MAX_H(FIRST_CHILD)
            );
        }
        break;
    }
}



void fjStdRowLayout(struct FjWidget *self, uint32_t mode)
{
    if (NO_CONTENT(self))
        return;

    if (self->data == NULL)
        return;

    struct FjStdRowData *rowData = self->data;

    switch (mode) {
        case FJ_LAYOUT_MAX:
            FOR_EACH_CHILD(i)
            {
                MAX_W(CHILD(i)) = CONSTRAIN(
                    MAX_W(self),
                    CONST_MIN_W(CHILD(i)),
                    CONST_MAX_W(CHILD(i))
                );

                MAX_H(CHILD(i)) = CONSTRAIN(
                    MAX_H(self),
                    CONST_MIN_H(CHILD(i)),
                    CONST_MAX_H(CHILD(i))
                );
            }
        break;

        case FJ_LAYOUT_MIN:
        {
            int32_t minW = CONST_MIN_W(self) + 2 * rowData->padding.x;
            int32_t minH = CONST_MIN_H(self) + 2 * rowData->padding.y;

            if (rowData->orientation == FJ_VERTICAL) {
                FOR_EACH_CHILD(i) {
                    minW = MAX(MIN_W(CHILD(i)), minW);
                    minH += MIN_H(CHILD(i));
                    
                    if (i != 0)
                        minH += rowData->spacing;
                }
            } else {
                FOR_EACH_CHILD(i) {
                    minW += MIN_W(CHILD(i));
                    minH = MAX(MIN_H(CHILD(i)), minH);
                    
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
                FOR_EACH_CHILD(i) {
                    allWeights += CHILD(i)->weights.y;
                }

                double oneWeight = (double) availableH / allWeights;

                FOR_EACH_CHILD(i) {
                    EXACT_W(CHILD(i)) = CONSTRAIN(
                        EXACT_W(self),
                        MIN_W(CHILD(i)),
                        MAX_W(CHILD(i))
                    );

                    EXACT_H(CHILD(i)) = 
                }
            }
        }
        break;
    }
}
