#include "geometry.h"

void drawLine(SDL_Renderer* renderer, Vector2 start, Vector2 end) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

void drawShape(SDL_Renderer* renderer, const Vector2 points[], int size) {
    for (int i = 0; i < size - 1; i++) {
        drawLine(renderer, points[i], points[i + 1]);
    }
    drawLine(renderer, points[0], points[size - 1]);
}

