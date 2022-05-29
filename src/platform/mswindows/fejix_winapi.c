#include <fejix_runtime/fejix.h>

uint32_t fjInstanceInit(struct FjInstance *inst, uint32_t *params)
{
    inst->params = params;
    inst->hInst = GetModuleHandle(NULL);
}



void fjInstanceDestroy(struct FjInstance *inst)
{

}



uint32_t fjWindowInit(struct FjInstance *inst, struct FjWindow *win, uint32_t *params)
{

}


void fjWindowDestroy(struct FjWindow *win)
{

}