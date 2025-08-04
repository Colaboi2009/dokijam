#include "utility.hpp"
#include <cstdlib>

float square(float f) { return f * f; }

float sqrDist(SDL_FPoint l, SDL_FPoint r) { return square(l.x - r.x) + square(l.y - r.y); }

SDL_FRect center(SDL_FRect r) { return {r.x - r.w / 2.f, r.y - r.h / 2.f, r.w, r.h}; }

// make it non-branched later
bool pointOnLine(SDL_FPoint p, SDL_FPoint lineStart, SDL_FPoint lineEnd, float lineThickness) {
    if (lineEnd.x == lineStart.x) {
        return std::abs(p.x - lineEnd.x) < lineThickness;
    }
    float lineSlope = (lineEnd.y - lineStart.y) / (lineEnd.x - lineStart.x);
    float b = lineEnd.y - lineSlope * lineEnd.x;
    return std::abs(p.y - (lineSlope * p.x + b)) < lineThickness;
}

bool pointInFRect(SDL_FPoint p, SDL_FRect r) { return SDL_PointInRectFloat(&p, &r); }

bool pointInCircle(SDL_FPoint p, float r, SDL_FPoint cp) { return square(cp.x - p.x) + square(cp.y - p.y) < square(r); }

bool pointWithin(SDL_FPoint b, SDL_FPoint a, float dist) { return square(b.x - a.x) + square(b.y - a.y) < dist * dist; }

bool floatWithin(float start, float end, float x) { return x > start && x < end; }

bool rectsOverlap(SDL_FRect r, SDL_FRect l) { return !(r.x > (l.x + l.w) || l.x > (r.x + r.w) || r.y > (l.y + l.h) || l.y > (r.y + r.h)); }

bool operator==(SDL_FPoint r, SDL_FPoint l) { return r.x == l.x && r.y == l.y; }

SDL_FPoint rectToPoint(SDL_FRect r) { return {r.x, r.y}; }
