#include "window.h"
#include "game.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <ctime>
#include <tuple>

Window::Window(const char* title, int width, int height) : game(width, height){
    windowData.title = title;
    windowData.width = width;
    windowData.height = height;
    init();
}

Window::~Window() {
    close();
}

void Window::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        exit(WINDOW_ERROR);
    }

    window = SDL_CreateWindow(windowData.title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowData.width, windowData.height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(WINDOW_ERROR);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        exit(WINDOW_ERROR);
    }
}

void Window::close() {
    renderer = NULL;
    window = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Window::render() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    // Render the game objects
    game.render(renderer);

    SDL_RenderPresent(renderer);
}

void Window::update(float deltaTime) {
    SDL_Event event;
    handleEvents(event);

    // Update game logic
    game.update(deltaTime);
}

void Window::handleEvents(SDL_Event& event) {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                // If Escape
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
                game.processInputs(event.key.keysym.sym, true);
                break;
            case SDL_KEYUP:
                game.processInputs(event.key.keysym.sym, false);
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    windowData.width = event.window.data1;
                    windowData.height = event.window.data2;
                    std::cout << "Resized to: (" << windowData.width << ", " << windowData.height << ")" << std::endl;
                    game.setDimensions(windowData.width, windowData.height);
                }
                break;
        }
    }
}

