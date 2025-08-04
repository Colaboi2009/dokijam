#include "point.hpp"

Point::Point() : m_p{0, 0} {}
Point::Point(int x, int y) : m_p{float(x), float(y)} {}
Point::Point(float x, float y) : m_p{x, y} {}
Point::Point(SDL_Point p) : m_p{float(p.x), float(p.y)} {}
Point::Point(SDL_FPoint p) : m_p{p.x, p.y} {}
Point::Point(SDL_Rect r) : m_p{float(r.x), float(r.y)} {}
Point::Point(SDL_FRect r) : m_p{r.x, r.y} {}

Point::operator SDL_Point() const { return {int(x()), int(y())}; }
Point::operator SDL_FPoint() const { return {x(), y()}; }
Point::operator SDL_Rect() const { return {int(x()), int(y()), 0, 0}; }
Point::operator SDL_FRect() const { return {x(), y(), 0, 0}; }

float Point::x() const { return m_p[0]; }
float Point::y() const { return m_p[1]; }
void Point::x(int x) { m_p[0] = x; }
void Point::y(int y) { m_p[1] = y; }

Point Point::operator+(Point p) const { return { x() + p.x(), y() + p.y() }; }
Point Point::operator-(Point p) const { return { x() - p.x(), y() - p.y() }; }
bool Point::operator==(Point p) const {
	return x() == p.x() && y() == p.y();
}
bool Point::operator<=(Point p) const {
	return x() <= p.x() && y() <= p.y();
}
bool Point::operator>=(Point p) const {
	return x() >= p.x() && y() >= p.y();
}
