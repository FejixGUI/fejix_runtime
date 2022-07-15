#include <fejix_runtime/fejix.h>
#include <fejix_runtime/stdlayout.h>

#include <math.h>

// Caution: magic outside Hogwarts

#define MIN_W(WGT) ((WGT)->constraints.minW)
#define MIN_H(WGT) ((WGT)->constraints.minH)
#define MAX_W(WGT) ((WGT)->constraints.maxW)
#define MAX_H(WGT) ((WGT)->constraints.maxH)

#define EXACT_X(WGT) ((WGT)->geometry.x)
#define EXACT_Y(WGT) ((WGT)->geometry.y)
#define EXACT_W(WGT) ((WGT)->geometry.w)
#define EXACT_H(WGT) ((WGT)->geometry.h)

#define WEIGHT(WGT) ((WGT)->weight)

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

/// All people call it `clamp`. I call it `constrain`.
///
/// Example usage:
/// EXACT_W(child) = constrain(EXACT_W(self), MIN_W(child), MAX_W(child))
static inline
int32_t constrain(int32_t value, int32_t minBound, int32_t maxBound) {
    return max(min(value, maxBound), minBound);
}

static inline int32_t divround(int32_t a, int32_t b) {
    return (int32_t) round((double) a / (double) b);
}



void fjStdNoLayout(struct FjWidget *self, uint32_t mode)
{
    if (mode == FJ_LAYOUT_MIN) {
        MIN_W(self) = 0;
        MIN_H(self) = 0;
    }
}



void fjStdRootLayout(struct FjWidget *self, uint32_t mode)
{

    if (NO_CONTENT(self)) {
        return;
    }

    switch (mode) {
        case FJ_LAYOUT_MAX:
            MAX_W(MY_CHILD(0)) = EXACT_W(self);
            MAX_H(MY_CHILD(0)) = EXACT_H(self);
        break;

        case FJ_LAYOUT_MIN:
            MIN_W(self) = MIN_W(MY_CHILD(0));
            MIN_H(self) = MIN_H(MY_CHILD(0));
        break;

        case FJ_LAYOUT_EXACT:
        {
            EXACT_X(MY_CHILD(0)) = EXACT_X(self);
            EXACT_Y(MY_CHILD(0)) = EXACT_Y(self);

            // TODO Optionally set window's min size to root's min size
            EXACT_W(self) = max(MIN_W(self), EXACT_W(self));
            EXACT_H(self) = max(MIN_H(self), EXACT_H(self));

            EXACT_W(MY_CHILD(0)) = constrain(
                EXACT_W(self),
                MIN_W(MY_CHILD(0)),
                MAX_W(MY_CHILD(0))
            );
            EXACT_H(MY_CHILD(0)) = constrain(
                EXACT_H(self),
                MIN_H(MY_CHILD(0)),
                MAX_H(MY_CHILD(0))
            );
        }
        break;
    }
}


void fjStdConstrainLayout(struct FjWidget *self, uint32_t mode)
{
    if (self->data == NULL)
        return; // ERROR

    struct FjConstraints cons = *((struct FjConstraints *) self->data);

        switch (mode) {
            case FJ_LAYOUT_MAX:
                MAX_W(self) = cons.maxW;
                MAX_H(self) = cons.maxH;
            break;

            case FJ_LAYOUT_MIN:
                if (!NO_CONTENT(self)) {
                    MIN_W(self) = cons.minW;
                    MIN_H(self) = cons.minH;
                } else {
                    MIN_W(self) = constrain(
                        MIN_W(MY_CHILD(0)),
                        cons.minW,
                        cons.maxW
                    );

                    MIN_H(self) = constrain(
                        MIN_H(MY_CHILD(0)), cons.minH, cons.maxH
                    );
                }
            break;

            case FJ_LAYOUT_EXACT:
                if (!NO_CONTENT(self)) {
                    EXACT_X(MY_CHILD(0)) = EXACT_X(self);
                    EXACT_Y(MY_CHILD(0)) = EXACT_Y(self);
                    EXACT_W(MY_CHILD(0)) = constrain(
                        EXACT_W(self), MIN_W(MY_CHILD(0)), MAX_W(MY_CHILD(0))
                    );
                    EXACT_H(MY_CHILD(0)) = constrain(
                        EXACT_H(self), MIN_H(MY_CHILD(0)), MAX_H(MY_CHILD(0))
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
                MAX_W(MY_CHILD(i)) = MAX_W(self);
                MAX_H(MY_CHILD(i)) = MAX_H(self);
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
                    allWeights += WEIGHT(MY_CHILD(i));
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
                    EXACT_W(MY_CHILD(i)) = constrain(
                        maxW,
                        MIN_W(MY_CHILD(i)),
                        MAX_W(MY_CHILD(i))
                    );

                    int32_t calculatedHeight = (int32_t) round(
                        spacePerWeight * (double) WEIGHT(MY_CHILD(i))
                    );

                    EXACT_H(MY_CHILD(i)) = constrain(
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
                    allWeights += WEIGHT(MY_CHILD(i));
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
                    EXACT_H(MY_CHILD(i)) = constrain(
                        maxH,
                        MIN_H(MY_CHILD(i)),
                        MAX_H(MY_CHILD(i))
                    );

                    int32_t calculatedWidth = (int32_t) round(
                        spacePerWeight * (double) WEIGHT(MY_CHILD(i))
                    );

                    EXACT_W(MY_CHILD(i)) = constrain(
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



void fjStdCenterLayout(struct FjWidget *self, uint32_t mode)
{
    if (NO_CONTENT(self))
        return; // Error

    switch (mode) {
        case FJ_LAYOUT_MAX:
            MAX_W(MY_CHILD(0)) = MAX_W(self);
            MAX_H(MY_CHILD(0)) = MAX_H(self);
        break;

        case FJ_LAYOUT_MIN:
            MIN_W(self) = MIN_W(MY_CHILD(0));
            MIN_H(self) = MIN_H(MY_CHILD(0));
        break;

        case FJ_LAYOUT_EXACT:
        {
            EXACT_W(MY_CHILD(0)) = constrain(
                EXACT_W(self),
                MIN_W(MY_CHILD(0)),
                MAX_W(MY_CHILD(0))
            );

            EXACT_H(MY_CHILD(0)) = constrain(
                EXACT_H(self),
                MIN_H(MY_CHILD(0)),
                MAX_H(MY_CHILD(0))
            );

            int32_t myCenterX = EXACT_X(self) + divround(EXACT_W(self), 2);
            int32_t myCenterY = EXACT_Y(self) + divround(EXACT_H(self), 2);
            int32_t childHalfW = divround(EXACT_W(MY_CHILD(0)), 2);
            int32_t childHalfH = divround(EXACT_H(MY_CHILD(0)), 2);

            EXACT_X(MY_CHILD(0)) = myCenterX - childHalfW;
            EXACT_Y(MY_CHILD(0)) = myCenterY - childHalfH;
        }
        break;
    }
}


void fjStdAlignLayout(struct FjWidget *self, uint32_t mode)
{
    if (NO_CONTENT(self))
        return;

    if (self->data == NULL)
        return;

    struct FjStdAlignLayoutData *data = self->data;

    switch (mode) {
        case FJ_LAYOUT_MAX:
            MAX_W(MY_CHILD(0)) = MAX_W(self);
            MAX_H(MY_CHILD(0)) = MAX_H(self);
        break;

        case FJ_LAYOUT_MIN:
            MIN_W(self) = MIN_W(MY_CHILD(0));
            MIN_H(self) = MIN_H(MY_CHILD(0));
        break;

        case FJ_LAYOUT_EXACT:
        {
            EXACT_W(MY_CHILD(0)) = constrain(
                EXACT_W(self),
                MIN_W(MY_CHILD(0)),
                MAX_W(MY_CHILD(0))
            );

            EXACT_H(MY_CHILD(0)) = constrain(
                EXACT_H(self),
                MIN_H(MY_CHILD(0)),
                MAX_H(MY_CHILD(0))
            );

            switch (data->xalign) {
            case FJ_LEFT:
                EXACT_X(MY_CHILD(0)) = EXACT_X(self);
            break;

            case FJ_XCENTER:
            {
                int32_t myCenterX = EXACT_X(self) + divround(EXACT_W(self), 2);
                int32_t childHalfW = divround(EXACT_W(MY_CHILD(0)), 2);
                EXACT_X(MY_CHILD(0)) = myCenterX - childHalfW;
            }
            break;

            case FJ_RIGHT:
                EXACT_X(MY_CHILD(0)) =
                    EXACT_X(self) + EXACT_W(self) - EXACT_W(MY_CHILD(0));
            break;
            }

            switch (data->yalign) {
            case FJ_TOP:
                EXACT_Y(MY_CHILD(0)) = EXACT_Y(self);
            break;

            case FJ_YCENTER:
            {
                int32_t myCenterY = EXACT_Y(self) + divround(EXACT_H(self), 2);
                int32_t childHalfH = divround(EXACT_H(MY_CHILD(0)), 2);
                EXACT_Y(MY_CHILD(0)) = myCenterY - childHalfH;
            }
            break;

            case FJ_BOTTOM:
                EXACT_Y(MY_CHILD(0)) =
                    EXACT_Y(self) + EXACT_H(self) - EXACT_H(MY_CHILD(0));
            break;
            }
        }
        break;
    }
}