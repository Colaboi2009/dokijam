#include "sdl_wrapper.hpp"
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdlib>

SDL::SDL() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();

    SDL_CreateWindowAndRenderer("new_sdl",
								1920, 1080, SDL_WINDOW_RESIZABLE, &m_window, &m_renderer);
    SDL_SetRenderVSync(m_renderer, 1);

    present();
}

SDL::~SDL() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void SDL::drawLine(int x1, int y1, int x2, int y2, int thickness) {
    for (int i = 0; i < thickness / 2.f; i++) {
        SDL_RenderLine(m_renderer, x1 - i / 2.f, y1 - i / 2.f, x2 - i / 2.f, y2 - i / 2.f);
        SDL_RenderLine(m_renderer, x1, y1, x2, y2);
        SDL_RenderLine(m_renderer, x1 + i / 2.f, y1 + i / 2.f, x2 + i / 2.f, y2 + i / 2.f);
    }
}

void SDL::drawLines(std::vector<SDL_FPoint> points, int thickness) {
    for (int i = 1; i < points.size(); i++) {
        SDL_FPoint p0 = points[i - 1];
        SDL_FPoint p1 = points[i];
        drawLine(p0.x, p0.y, p1.x, p1.y, thickness);
    }
}

void SDL::drawCircle(int radius, int x, int y) {
    int offsetx, offsety, d;

    offsetx = 0;
    offsety = radius;
    d = radius - 1;

    while (offsety >= offsetx) {
        drawLine(x - offsety, y + offsetx, x + offsety, y + offsetx);
        drawLine(x - offsetx, y + offsety, x + offsetx, y + offsety);
        drawLine(x - offsetx, y - offsety, x + offsetx, y - offsety);
        drawLine(x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx += 1;
        } else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        } else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }
}

