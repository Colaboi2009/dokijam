#pragma once

#include <SDL3/SDL.h>

class Point {
  private:
    float m_p[2];

  public:
	Point();
    Point(int x, int y);
    Point(float x, float y);
    Point(SDL_Point);
    Point(SDL_FPoint);
    Point(SDL_Rect);
    Point(SDL_FRect);

    operator SDL_Point() const;
    operator SDL_FPoint() const;
    operator SDL_Rect() const;
    operator SDL_FRect() const;

    float x() const;
    float y() const;
    void x(int);
    void y(int);

	Point operator+(Point) const;
	Point operator-(Point) const;
	bool operator==(Point) const;
	bool operator<=(Point) const;
	bool operator>=(Point) const;
};
