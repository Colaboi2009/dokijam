#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <vector>
#include "point.hpp"

class SDL {
  private:
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;

  public:
    SDL();
    ~SDL();

    void present() { SDL_RenderPresent(m_renderer); }
    void setColor(SDL_Color c) { SDL_SetRenderDrawColor(m_renderer, c.r, c.g, c.b, c.a); }
    void clear(SDL_Color c) { setColor(c); SDL_RenderClear(m_renderer); }

	void renderDebugText(SDL_FPoint p, std::string str) { SDL_RenderDebugText(m_renderer, p.x, p.y, str.c_str()); }

    void drawPoint(SDL_Point p) { SDL_RenderPoint(m_renderer, p.x, p.y); }
	void drawLine(int x1, int y1, int x2, int y2, int thickness = 1);
	void drawLines(std::vector<SDL_FPoint>, int thickness = 1);
	void drawCircle(int radius, int x, int y);
    void drawRect(SDL_FRect r) { SDL_RenderRect(m_renderer, &r); }
    void drawRectFilled(SDL_FRect r) { SDL_RenderFillRect(m_renderer, &r); }

	void setRenderScale(Point p) { SDL_SetRenderScale(m_renderer, p.x, p.y); }
	void resetRenderScale() { SDL_SetRenderScale(m_renderer, 1, 1); }

	bool startTextInput() { return SDL_StartTextInput(m_window); }
	bool stopTextInput() { return SDL_StopTextInput(m_window); }
	bool textInputActive() { return SDL_TextInputActive(m_window); }

    SDL_Texture *createTextureFromSurface(SDL_Surface *surf) { return SDL_CreateTextureFromSurface(m_renderer, surf); }
    void renderTexture(SDL_Texture *t, SDL_FRect *src, SDL_FRect *dst) { SDL_RenderTexture(m_renderer, t, src, dst); }

	void showMessageBox(SDL_MessageBoxFlags f, std::string title, std::string text) { SDL_ShowSimpleMessageBox(f, title.c_str(), text.c_str(), m_window); }
	void error(std::string title, std::string text) { showMessageBox(SDL_MESSAGEBOX_ERROR, title, text); }
};
