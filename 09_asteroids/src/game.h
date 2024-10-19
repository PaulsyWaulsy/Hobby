#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

#include "geometry.h"

/*-----------------------------------INPUTS----------------------------------*/
typedef struct {
    bool left;
    bool right;
    bool up;
    bool down;
    bool shoot;
} Inputs;

/*-----------------------------------PLAYER----------------------------------*/
// Player Constants
const float PLAYER_ROTATION_RATE = 0.05f;
const float PLAYER_SPEED = 30.0f;
const float PLAYER_DRAG = 0.02f;
const float MAX_FIRE_RATE = 200; // Milliseconds (1 bullet per 0.2s)

// Bullet Constants
const float BULLET_SPEED = 1000.0f;
const Uint32 BULLET_DESPAWN_TIME = 3000; // Milliseconds
const Uint32 FIRE_RATE = 150; // Milliseconds

// Ship Constants
const int SHIP_VERTICES_SIZE = 5;
const Vector2 SHIP_VERTICES[SHIP_VERTICES_SIZE] = {
    Vector2(0, -20),
    Vector2(15, 20),
    Vector2(5, 15),
    Vector2(-5, 15),
    Vector2(-15, 20),
};

typedef struct Player {
    Vector2 position;
    Vector2 velocity;
    Vector2 rotation;
    Uint32 lastShotTime;

    // The points of the ship (simplified to a triangle)
    Vector2 ship[SHIP_VERTICES_SIZE];
    Vector2 shipRotation[SHIP_VERTICES_SIZE];
    Player(Vector2 position) :
    position(position), velocity(Vector2(0, 0)), rotation(Vector2(0, 1)) {
        for (int i = 0; i < SHIP_VERTICES_SIZE; i++) {
            ship[i] = position + SHIP_VERTICES[i];
            shipRotation[i] = SHIP_VERTICES[i];
        }
    };
} Player;

typedef enum {
    SMALL,
    MEDIUM,
    LARGE,
} AsteroidSize;

typedef struct {
    Vector2 position;
    Vector2 velocity;
    AsteroidSize size;
    int seed;
} Asteroid;

typedef struct Bullet {
    Vector2 position;
    Vector2 velocity;
    Uint32 spawnTime;

    // When the bullet spawns it will also keep the time so we can delete it
    // After som amount of time has passed (BULLET_DESPAWN_TIME)
    Bullet(Vector2 position, Vector2 velocity) :
    position(position), velocity(velocity), spawnTime(SDL_GetTicks()) {};
} Bullet;


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
    
    // Keyboard Functions
    void processInputs(int keycode, bool keydown);

    // Other Functions
    void setDimensions(int width, int height);

private:
    // methods
    int width;
    int height;
    Uint32 lastFireTime;
    Inputs inputs;
    Player player;
    std::vector<Bullet> bullets;
    std::map<int, bool> keyboard;

    // Game Functions
    void handleGameEvents(SDL_Event& event);

    // Player Functions
    void updatePlayer(float deltaTime);
    void updateBullets(float deltaTime);
    void renderPlayer(SDL_Renderer* renderer);
    void renderBullets(SDL_Renderer* renderer);
    void shoot();
};


