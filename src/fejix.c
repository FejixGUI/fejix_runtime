#include <fejix_runtime/fejix.h>


void fjRootSetColor(struct FjRoot *root, float R, float G, float B, float A)
{
    root->background.r = R;
    root->background.g = G;
    root->background.b = B;
    root->background.a = A;
}
