#include "point.hpp"

Point::Point() : m_p{0, 0} {}
Point::Point(int x, int y) : m_p{float(x), float(y)} {}
Point::Point(float x, float y) : m_p{x, y} {}
Point::Point(SDL_Point p) : m_p{float(p.x), float(p.y)} {}
Point::Point(SDL_FPoint p) : m_p{p.x, p.y} {}
Point::Point(SDL_Rect r) : m_p{float(r.x), float(r.y)} {}
Point::Point(SDL_FRect r) : m_p{r.x, r.y} {}

Point::operator SDL_Point() const { return {int(m_p[0]), int(m_p[1])}; }
Point::operator SDL_FPoint() const { return {m_p[0], m_p[1]}; }
Point::operator SDL_Rect() const { return {int(m_p[0]), int(m_p[1]), 0, 0}; }
Point::operator SDL_FRect() const { return {m_p[0], m_p[1], 0, 0}; }

Point Point::operator+(Point p) const { return { x + p.x, y + p.y }; }
Point Point::operator-(Point p) const { return { x - p.x, y - p.y }; }
Point Point::operator/(Point p) const { return { x / p.x, y / p.y }; }
bool Point::operator==(Point p) const {
	return m_p[0] == p.m_p[0] && m_p[1] == p.m_p[1];
}
bool Point::operator<=(Point p) const {
	return m_p[0] <= p.m_p[0] && m_p[1] <= p.m_p[1];
}
bool Point::operator>=(Point p) const {
	return m_p[0] >= p.m_p[0] && m_p[1] >= p.m_p[1];
}
