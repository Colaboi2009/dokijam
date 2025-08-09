#pragma once

#include <SDL3/SDL_rect.h>
#include <chrono>
#include <memory>

template<typename T>
using UP = std::unique_ptr<T>;
template<typename T>
using WP = std::weak_ptr<T>;
template<typename T>
using SP = std::shared_ptr<T>;

float square(float);
float sqrDist(SDL_FPoint, SDL_FPoint);
float dot(SDL_FPoint, SDL_FPoint);
SDL_FRect center(SDL_FRect);
bool pointOnLine(SDL_FPoint p, SDL_FPoint lineStart, SDL_FPoint lineEnd, float lineThickness);
bool pointInFRect(SDL_FPoint, SDL_FRect);
bool pointInCircle(SDL_FPoint p, float r, SDL_FPoint cp);
bool pointWithin(SDL_FPoint b, SDL_FPoint a, float dist);
bool circleInFRect(SDL_FPoint cp, float r, SDL_FRect b);
bool floatWithin(float start, float end, float x);
bool circlesOverlap(SDL_FPoint c1, float r1, SDL_FPoint c2, float r2);
bool rectsOverlap(SDL_FRect, SDL_FRect);
bool operator==(SDL_FPoint r, SDL_FPoint l);
SDL_FPoint rectToPoint(SDL_FRect);

std::chrono::time_point<std::chrono::high_resolution_clock> beginTimer();
float endTimer(std::chrono::time_point<std::chrono::high_resolution_clock> begin);
