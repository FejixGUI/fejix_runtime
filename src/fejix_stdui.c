#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_stdui.h>


#define CONST_MIN_W(w) (w)->constraints.x1
#define CONST_MIN_H(w) (w)->constraints.y1
#define CONST_MAX_W(w) (w)->constraints.x2
#define CONST_MAX_H(w) (w)->constraints.y2

#define MIN_W(w) (w)->_geometry.x1
#define MIN_H(w) (w)->_geometry.y1
#define MAX_W(w) (w)->_geometry.x2
#define MAX_H(w) (w)->_geometry.y2

#define EXACT_X(w) (w)->_geometry.x1
#define EXACT_Y(w) (w)->_geometry.y1
#define EXACT_W(w) (w)->_geometry.x2
#define EXACT_H(w) (w)->_geometry.y2

#define NO_CONTENT(w) ((w)->contentLength == 0)
#define FIRST_CHILD (self->content[0])
#define CHILD(INDEX) (self->content[INDEX])

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

            EXACT_W(self) = CONSTRAIN(EXACT_W(self), minW, minW);

            EXACT_W(FIRST_CHILD) = MIN(EXACT_W(self), MAX_W(FIRST_CHILD));
            EXACT_H(FIRST_CHILD) = MIN(EXACT_H(self), MAX_H(FIRST_CHILD));
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
            for (int i=0; i<self->contentLength; i++)
            {
                MAX_W(CHILD(i)) = MIN(MAX_W(self), MAX_W(CHILD(i)));
                MAX_H(CHILD(i)) = MIN(MAX_H(self), MAX_H(CHILD(i)));
            }
        break;

        case FJ_LAYOUT_MIN:
            for (int i=0; i<self->contentLength; i++)
            {
                MAX_W(CHILD(i)) = MIN(MAX_W(self), MAX_W(CHILD(i)));
                MAX_H(CHILD(i)) = MIN(MAX_H(self), MAX_H(CHILD(i)));
            }
        break;
    }
}
