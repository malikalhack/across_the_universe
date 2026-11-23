#define SDL_MAIN_HANDLED
#include "universe.h"

#include <iostream>

int main(int, char**) {
    SDL_SetMainReady();
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_DisplayMode displayMode{};
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) {
        std::cerr << "Could not get display mode: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Across the Universe",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        displayMode.w,
        displayMode.h,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_SHOWN
    );
    if (window == nullptr) {
        std::cerr << "Could not create window: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Accelerated renderer unavailable: " << SDL_GetError()
                  << ". Falling back to software rendering.\n";
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    }
    if (renderer == nullptr) {
        std::cerr << "Could not create renderer: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int width = 0;
    int height = 0;
    if (SDL_GetRendererOutputSize(renderer, &width, &height) != 0) {
        std::cerr << "Could not get renderer size: " << SDL_GetError() << '\n';
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Universe universe(width, height);
    bool running = true;
    int exitCode = 0;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }

        if (!running) {
            break;
        }

        if (!universe.Draw(renderer)) {
            std::cerr << "Could not draw frame: " << SDL_GetError() << '\n';
            exitCode = 1;
            running = false;
            continue;
        }
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return exitCode;
}
