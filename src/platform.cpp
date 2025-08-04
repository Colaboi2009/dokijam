#include "platform.hpp"

#include "globals.hpp"

Platform::Platform(SDL_FRect r, SDL_Color c) : m_rect{r}, m_color{c} {}

void Platform::render() {
    sdl.setColor(m_color);
    sdl.drawRectFilled(m_rect);
}

SDL_FRect Platform::bounds() { return m_rect; }

void Platform::sex(float x) { m_rect.x = x; }
void Platform::sey(float y) { m_rect.y = y; }
