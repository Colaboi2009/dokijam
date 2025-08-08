#include "utility.hpp"

#include <cstdlib>
#include <chrono>

float square(float f) { return f * f; }

float sqrDist(SDL_FPoint l, SDL_FPoint r) { return square(l.x - r.x) + square(l.y - r.y); }

float dot(SDL_FPoint l, SDL_FPoint r) {
	//std::vector<float> v1{l.x, l.y};
	//std::vector<float> v2{r.x, r.y};
	//return std::inner_product(v1.begin(), v1.end(), v2.begin(), 0);

    // Do not use vector, cuz unnecessary heap-allocation,
    // TODO: Add glm for more complex stuff
    return (l.x * r.x + l.y * r.y);
}

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

bool circleInFRect(SDL_FPoint cp, float r, SDL_FRect b) {
	SDL_FPoint botr = {b.x + b.w, b.y + b.h};
	float closest_x = (cp.x < b.x ? b.x : (cp.x > botr.x ? botr.x : cp.x));
	float closest_y = (cp.y < b.y ? b.y : (cp.y > botr.y ? botr.y : cp.y));
	float dx = closest_x - cp.x;
	float dy = closest_y - cp.y;
	return (square(dx) + square(dy)) <= square(r);
}

bool floatWithin(float start, float end, float x) { return x > start && x < end; }

bool circlesOverlap(SDL_FPoint c1, float r1, SDL_FPoint c2, float r2) {
	return sqrDist(c1, c2) < square(r1 + r2);
}

bool rectsOverlap(SDL_FRect r, SDL_FRect l) { return !(r.x > (l.x + l.w) || l.x > (r.x + r.w) || r.y > (l.y + l.h) || l.y > (r.y + r.h)); }

bool operator==(SDL_FPoint r, SDL_FPoint l) { return r.x == l.x && r.y == l.y; }

SDL_FPoint rectToPoint(SDL_FRect r) { return {r.x, r.y}; }

std::chrono::time_point<std::chrono::high_resolution_clock> beginTimer() {
	return std::chrono::high_resolution_clock::now();
}

float endTimer(std::chrono::time_point<std::chrono::high_resolution_clock> begin) {
	return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - begin).count();
}
