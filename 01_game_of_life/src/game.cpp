#include "game.h"

Game::Game(int width, int height) {
    this->width = width;
    this->height = height;
}

Game::~Game() {
}


/*------------------------------------GAME-----------------------------------*/
void Game::update(float deltaTime) {
}

void Game::render(SDL_Renderer* renderer) {
}

void Game::setDimensions(int width, int height) {
    this->width = width;
    this->height = height;
}



