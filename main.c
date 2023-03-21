#include <stdio.h>
#include <SDL2/SDL.h>

const int WIDTH = 1200;
const int HEIGHT = 800;

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;

        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
