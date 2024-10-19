#include "window.h"
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Time constants
const float MS_TO_SECONDS_F = 1000.0f;
const int MS_TO_SECONDS = 1000;
const int MAX_FPS = 120;
const int TICK_PER_FRAME = MS_TO_SECONDS / MAX_FPS;

typedef struct Time {
    float deltaTime;
    Uint32 time;
    Uint32 frameTime;
    Uint32 lastSecond;
    Uint32 lastFrame;
    int frames;
    int fps;

    // Constructor all set to 0
    Time() : time(0), deltaTime(0), lastFrame(1), frames(0) {};
}Time;


void updateTime(Time& time) {
    // Calculate delta time and keep physics consistent
    time.time = SDL_GetTicks();
    time.deltaTime = (time.time - time.lastFrame) / MS_TO_SECONDS_F;
    time.lastFrame = time.time;
    time.frames++;
    if (time.deltaTime < 0) {
        time.deltaTime = 0;
    }
    if (time.time - time.lastSecond >= MS_TO_SECONDS) {
        time.lastSecond = time.time;
        time.fps = time.frames;
        time.frames = 0;
        std::cout << "FPS: " << time.fps << std::endl;
    }
}

void limitFPS(Time& time) {
    time.frameTime = SDL_GetTicks() - time.time;
    if (time.frameTime < TICK_PER_FRAME) {
        SDL_Delay(TICK_PER_FRAME - time.frameTime);
    }
}

int main(int argc, char* args[]) {
    Window window = Window("Game", 1000, 700);
    Time time;

    // Window closes if the users quits or presses esc
    while (!window.shouldQuit()) {

        updateTime(time);
        window.update(time.deltaTime);
        window.render();
        limitFPS(time);
    }
    window.close();

    return 0;
}

