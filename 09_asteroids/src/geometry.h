#pragma once

#include <SDL2/SDL_render.h>

typedef struct Vector4 {
    int r;
    int g;
    int b;
    int a;
    Vector4 (int r = 0xFF, int g = 0xFF, int b = 0xFF, int a = 0xFF) : r(r), g(g), b(b), a(a) {};
} Vector4;

class Vector2 {
public:
    float x;
    float y;

    // Constructor & Destructor
    Vector2(float x = 0, float y = 0) : x(x), y(y) {};
    ~Vector2() {};

    // Vecotor math operations
    Vector2 operator+(const Vector2& other) {
        return Vector2(this->x + other.x, this->y + other.y);
    }

    Vector2 operator-(const Vector2& other) {
        return Vector2(this->x - other.x, this->y - other.y);
    }

    // Vecotor math operations
    Vector2& operator+=(const Vector2& other) {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    Vector2& operator-=(const Vector2& other) {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    Vector2& operator*=(const float other) {
        this->x *= other;
        this->y *= other;
        return *this;
    }

    Vector2& operator/=(const float other) {
        this->x /= other;
        this->y /= other;
        return *this;
    }

    // Scalar math operations    
    Vector2 operator*(const float& other) {
        return Vector2(this->x * other, this->y * other);
    }

    Vector2 operator/(const float& other) {
        return Vector2(this->x / other, this->y / other);
    }

    float magnitude(void) {
        return (this->x * this->x) + (this->y * this->y);
    }

    // Angle in radians
    Vector2& rotate(const float& angle) {
        float cosTheta = std::cos(angle);
        float sinTheta = std::sin(angle);

        float newX = this->x * cosTheta - this->y * sinTheta;
        float newY = this->x * sinTheta + this->y * cosTheta;
        this->x = newX;
        this->y = newY;
        return *this;
    }

    Vector2& rotateAround(const Vector2& center, const float& angle) {
        float cosTheta = std::cos(angle);
        float sinTheta = std::sin(angle);

        // Translate the point to the origin (relative to the center)
        float translatedX = this->x - center.x;
        float translatedY = this->y - center.y;

        // Rotate the point
        float rotatedX = translatedX * cosTheta - translatedY * sinTheta;
        float rotatedY = translatedX * sinTheta + translatedY * cosTheta;

        // Translate back to the original position
        this->x = rotatedX + center.x;
        this->y = rotatedY + center.y;

        return *this;
    }
};

void drawLine(SDL_Renderer* renderer, Vector2 start, Vector2 end);

void drawShape(SDL_Renderer* renderer, const Vector2 points[], int size);








