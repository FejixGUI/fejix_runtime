#include <fejix_runtime/fejix.h>
#include <fejix_runtime/stdlayout.h>

#include <math.h>

// Caution: magic outside Hogwarts

#define CONST_MIN_W(WGT) (WGT)->constraints.minW
#define CONST_MIN_H(WGT) (WGT)->constraints.minH
#define CONST_MAX_W(WGT) (WGT)->constraints.maxW
#define CONST_MAX_H(WGT) (WGT)->constraints.maxH

#define MIN_W(WGT) (WGT)->_tmpConstraints.minW
#define MIN_H(WGT) (WGT)->_tmpConstraints.minH
#define MAX_W(WGT) (WGT)->_tmpConstraints.maxW
#define MAX_H(WGT) (WGT)->_tmpConstraints.maxH

#define EXACT_X(WGT) (WGT)->geometry.x
#define EXACT_Y(WGT) (WGT)->geometry.y
#define EXACT_W(WGT) (WGT)->geometry.w
#define EXACT_H(WGT) (WGT)->geometry.h

#define WEIGHT_X(WGT) (WGT)->weights.x
#define WEIGHT_Y(WGT) (WGT)->weights.y

#define LEN(WGT) ((WGT)->contentLength)
#define NO_CONTENT(WGT) (LEN(WGT) == 0)
#define CHILD(WGT, INDEX) ((WGT)->content[INDEX])

/// Uses `self` !!!
#define MY_CHILD(i) CHILD(self, i)

#define FOR_EACH_CHILD(WGT, COUNTER) \
    for(int COUNTER=0; COUNTER<LEN(WGT); COUNTER++)

// Never try to make the following a macro
static inline int32_t min(int32_t a, int32_t b) {
    return a < b ? a : b;
}

static inline int32_t max(int32_t a, int32_t b) {
    return a > b ? a : b;
}

/// Example usage:
/// EXACT_W(childWidget) = CONSTRAIN(
///     EXACT_W(self),
///     MIN_W(childWidget),
///     MAX_W(childWidget)
/// )
#define CONSTRAIN(_VALUE, _MIN, _MAX) max(min(_VALUE, _MAX), _MIN)



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
            MAX_W(MY_CHILD(0)) = CONSTRAIN(
                EXACT_W(self),
                CONST_MIN_W(MY_CHILD(0)),
                CONST_MAX_W(MY_CHILD(0))
            );
            MAX_H(MY_CHILD(0)) = CONSTRAIN(
                EXACT_H(self),
                CONST_MIN_H(MY_CHILD(0)),
                CONST_MAX_H(MY_CHILD(0))
            );
        break;

        case FJ_LAYOUT_MIN:
            MIN_W(self) = MIN_W(MY_CHILD(0));
            MIN_H(self) = MIN_H(MY_CHILD(0));
        break;

        case FJ_LAYOUT_EXACT:
        {
            EXACT_X(self) = 0;
            EXACT_Y(self) = 0;

            EXACT_X(MY_CHILD(0)) = 0;
            EXACT_Y(MY_CHILD(0)) = 0;

            EXACT_W(self) = max(MIN_W(self), EXACT_W(self));
            EXACT_H(self) = max(MIN_H(self), EXACT_H(self));

            EXACT_W(MY_CHILD(0)) = CONSTRAIN(
                EXACT_W(self),
                MIN_W(MY_CHILD(0)),
                MAX_W(MY_CHILD(0))
            );
            EXACT_H(MY_CHILD(0)) = CONSTRAIN(
                EXACT_H(self),
                MIN_H(MY_CHILD(0)),
                MAX_H(MY_CHILD(0))
            );
        }
        break;
    }
}


void fjStdLinearLayout(struct FjWidget *self, uint32_t mode)
{
    if (self->data == NULL)
        return; // Error

    struct FjStdLinearLayoutData *data = self->data;


    if (NO_CONTENT(self)) {
        MIN_W(self) = 2 * data->padding.x;
        MIN_H(self) = 2 * data->padding.y;
        return;
    }



    switch (mode) {
        case FJ_LAYOUT_MAX:
            FOR_EACH_CHILD(self, i)
            {
                MAX_W(MY_CHILD(i)) = CONSTRAIN(
                    MAX_W(self),
                    CONST_MIN_W(MY_CHILD(i)),
                    CONST_MAX_W(MY_CHILD(i))
                );

                MAX_H(MY_CHILD(i)) = CONSTRAIN(
                    MAX_H(self),
                    CONST_MIN_H(MY_CHILD(i)),
                    CONST_MAX_H(MY_CHILD(i))
                );
            }
        break;

        case FJ_LAYOUT_MIN:
        {
            uint32_t minW = 0;
            uint32_t minH = 0;

            data->_minSize = 0;
            data->_spaceSize = 0;

            if (data->orientation == FJ_VERTICAL) {
                FOR_EACH_CHILD(self, i) {
                    minW = max(MIN_W(MY_CHILD(i)), minW);
                    data->_minSize += MIN_H(MY_CHILD(i));
                }
                data->_spaceSize = data->spacing * (self->contentLength-1)
                    + 2 * data->padding.y;
                minH = data->_minSize + data->_spaceSize;
                minW += 2 * data->padding.x;
            } else {
                FOR_EACH_CHILD(self, i) {
                    data->_minSize += MIN_W(MY_CHILD(i));
                    minH = max(MIN_H(MY_CHILD(i)), minH);
                }
                data->_spaceSize = data->spacing * (self->contentLength-1)
                    + 2 * data->padding.x;
                minW = data->_minSize + data->_spaceSize;
                minH += 2 * data->padding.y;
            }

            MIN_W(self) = minW;
            MIN_H(self) = minH;
        }
        break;

        case FJ_LAYOUT_EXACT:
        {

            // If you change the following code, remember to also change
            // the code for the horizontal layout
            if (data->orientation == FJ_VERTICAL) {
                int32_t availableH =
                    EXACT_H(self) - data->_spaceSize
                    - data->_minSize;

                float allWeights = 0.f;
                FOR_EACH_CHILD(self, i) {
                    allWeights += WEIGHT_Y(MY_CHILD(i));
                }

                double spacePerWeight =
                    (double) availableH
                    / (double) allWeights;

                struct FjPoint position = {
                    EXACT_X(self) + data->padding.x,
                    EXACT_Y(self) + data->padding.y
                };

                int32_t maxW = EXACT_W(self) - 2 * data->padding.x;

                FOR_EACH_CHILD(self, i) {
                    EXACT_W(MY_CHILD(i)) = CONSTRAIN(
                        maxW,
                        MIN_W(MY_CHILD(i)),
                        MAX_W(MY_CHILD(i))
                    );

                    int32_t calculatedHeight = (int32_t) round(
                        spacePerWeight * (double) WEIGHT_Y(MY_CHILD(i))
                    );

                    EXACT_H(MY_CHILD(i)) = CONSTRAIN(
                        MIN_H(MY_CHILD(i)) + calculatedHeight,
                        MIN_H(MY_CHILD(i)),
                        MAX_H(MY_CHILD(i))
                    );

                    EXACT_X(MY_CHILD(i)) = position.x;
                    EXACT_Y(MY_CHILD(i)) = position.y;

                    position.y += EXACT_H(MY_CHILD(i)) + data->spacing;
                }

            } else /* FJ_HORIZONTAL */ {

                int32_t availableW =
                    EXACT_W(self) - data->_spaceSize
                    - data->_minSize;

                float allWeights = 0.f;
                FOR_EACH_CHILD(self, i) {
                    allWeights += WEIGHT_X(MY_CHILD(i));
                }

                double spacePerWeight =
                    (double) availableW
                    / (double) allWeights;

                struct FjPoint position = {
                    EXACT_X(self) + data->padding.x,
                    EXACT_Y(self) + data->padding.y
                };

                int32_t maxH = EXACT_H(self) - 2 * data->padding.y;

                FOR_EACH_CHILD(self, i) {
                    EXACT_H(MY_CHILD(i)) = CONSTRAIN(
                        maxH,
                        MIN_H(MY_CHILD(i)),
                        MAX_H(MY_CHILD(i))
                    );

                    int32_t calculatedWidth = (int32_t) round(
                        spacePerWeight * (double) WEIGHT_Y(MY_CHILD(i))
                    );

                    EXACT_W(MY_CHILD(i)) = CONSTRAIN(
                        MIN_W(MY_CHILD(i)) + calculatedWidth,
                        MIN_W(MY_CHILD(i)),
                        MAX_W(MY_CHILD(i))
                    );

                    EXACT_X(MY_CHILD(i)) = position.x;
                    EXACT_Y(MY_CHILD(i)) = position.y;

                    position.x += EXACT_W(MY_CHILD(i)) + data->spacing;
                }

            } // FJ_HORIZONTAL

        } // case FJ_LAYOUT_EXACT
        break;
    } // switch (mode)

} // fjStdLinearLayout