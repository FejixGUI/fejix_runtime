#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <fejix_runtime/fejix.h>
#include <fejix_runtime/fejix_aliases.h>

int main() {
    assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

    SDL_Window *win = SDL_CreateWindow(
        "Hello world!",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        800, 600,
        0 //SDL_WINDOW_RESIZABLE
    );

    assert(win);

    SDL_Renderer *rend = SDL_CreateRenderer(win, -1, 0);

    assert(win);

    printf("SDL initialized successfully\n");

    uint32_t status;
    
    FjRoot root = {0};
    status = fjRootInit_Sdl(&root, rend);
    printf("Init root: %d\n", status);

    fjRootSetColor(&root, 1., .7, .0, 1.);

    SDL_Event ev;
    int alive = 1;
    while (alive && SDL_WaitEvent(&ev)) {
        switch (ev.type) {

        case SDL_QUIT:
            alive = 0;
            break;

        }

        fjDraw_Sdl(&root, rend);
        SDL_RenderPresent(rend);
    }

    fjRootDestroy_Sdl(&root);

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}