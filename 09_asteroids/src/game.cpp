#include "game.h"
#include "geometry.h"
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <algorithm>

Game::Game(int width, int height) : player(Vector2(width, height)) {
    this->width = width;
    this->height = height;
}

Game::~Game() {
}

/*-----------------------------------INPUTS----------------------------------*/
void Game::processInputs(int keycode, bool keydown) {
    keyboard[keycode] = keydown;
}


/*-----------------------------------PLAYER----------------------------------*/
void Game::updatePlayer(float deltaTime) {

    // Rotate the ship if the inputs are RIGHT or LEFT
    if (keyboard[SDLK_LEFT]) {
        player.rotation.rotate(-PLAYER_ROTATION_RATE);
        for (int i = 0; i < SHIP_VERTICES_SIZE; i++) {
            player.shipRotation[i].rotate(-PLAYER_ROTATION_RATE);
        }
    }
    if (keyboard[SDLK_RIGHT]) {
        player.rotation.rotate(PLAYER_ROTATION_RATE);
        for (int i = 0; i < SHIP_VERTICES_SIZE; i++) {
            player.shipRotation[i].rotate(PLAYER_ROTATION_RATE);
        }
    }

    if (keyboard[SDLK_SPACE] && (SDL_GetTicks() - lastFireTime >= FIRE_RATE)) {
        lastFireTime = SDL_GetTicks();
        shoot();
    }

    // Move the ship forward
    if (keyboard[SDLK_UP]) {
        player.velocity -= player.rotation * PLAYER_SPEED;
    }

    player.velocity = player.velocity * (1 - PLAYER_DRAG);

    player.position += player.velocity * deltaTime;

    for (int i = 0; i < SHIP_VERTICES_SIZE; i++) {
        player.ship[i] = player.position + player.shipRotation[i];
    }

    player.position.x = std::fmod(player.position.x + width, width);
    player.position.y = std::fmod(player.position.y + height, height);
}

void Game::renderPlayer(SDL_Renderer* renderer) {
    drawShape(renderer, player.ship, SHIP_VERTICES_SIZE);
}

void Game::updateBullets(float deltaTime) {
    // iterate over the bullets and entities.erase(

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) {
            return (SDL_GetTicks() - bullet.spawnTime >= BULLET_DESPAWN_TIME);
        }),
        bullets.end()
    );
    
    for (Bullet& bullet : bullets) {
        bullet.position += bullet.velocity * deltaTime;
    }
}

void Game::renderBullets(SDL_Renderer* renderer) {
    for (Bullet& bullet : bullets) {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawPoint(renderer, bullet.position.x, bullet.position.y);
    }
}

void Game::shoot() {
    Bullet bullet = Bullet(player.position, player.rotation * -BULLET_SPEED);
    bullets.push_back(bullet);
}

/*------------------------------------GAME-----------------------------------*/
void Game::update(float deltaTime) {
    updatePlayer(deltaTime);
    updateBullets(deltaTime);
}

void Game::render(SDL_Renderer* renderer) {
    renderPlayer(renderer);
    renderBullets(renderer);
}

void Game::setDimensions(int width, int height) {
    this->width = width;
    this->height = height;
}



