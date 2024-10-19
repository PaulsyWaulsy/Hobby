#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>


/*------------------------------------GAME-----------------------------------*/
// The game goes indefintetly so there is no 'win' state
// Just attempting to get the highgest score
typedef enum {
    GAME_MENU,
    GAME_RUN,
    GAME_LOSE,
} GameState;

class Game {
public:
    // Constructor & Destructors
    Game(int width, int height);
    ~Game();

    // Methods
    GameState state;

    // Game Functions
    void init();
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    
    // Other Functions
    void setDimensions(int width, int height);

private:
    // methods
    int width;
    int height;
    int** cells;

    // Game Functions
    void handleGameEvents(SDL_Event& event);
};


