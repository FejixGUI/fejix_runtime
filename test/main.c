#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_aliases.h>


const uint32_t params[] = {
    FJ_PARAM_INIT_BK, FJ_BK_CAIRO,
    FJ_PARAM_END,
};


int main() {
    assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

    SDL_Window *window = SDL_CreateWindow(
        "Hello world!",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600,
        0 //SDL_WINDOW_RESIZABLE
    );
    assert(window);

    FjRoot root = {0};
    assert(fjRootInit_Sdl(&root, params, window) == FJ_OK);

    fjRootSetColor(&root, 1., .7, .0, 1.);

    SDL_Event ev;
    int alive = 1;
    while (alive && SDL_WaitEvent(&ev)) {
        switch (ev.type) {

        case SDL_QUIT:
            alive = 0;
            break;

        }

        assert(fjDraw(&root) == FJ_OK);
    }

    fjRootDestroy(&root);

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}