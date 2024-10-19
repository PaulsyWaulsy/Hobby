#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_quit.h>
#include <SDL2/SDL_video.h>
#include <map>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>

#include "game.h"
#include "geometry.h"

#define WINDOW_ERROR 2

typedef struct {
    const char* title;
  int width;
  int height;
} WindowData;

class Window {
public:
    Window(const char* title, int width, int height);
    ~Window(void);

    void init(void);
    void close(void);
    void update(float deltaTime);
    void render(void);
    bool shouldQuit(void) { return quit; };

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    WindowData windowData;
    Game game;
    bool quit;
    std::map<int, bool> keyboard;

    void handleEvents(SDL_Event& event);
};
