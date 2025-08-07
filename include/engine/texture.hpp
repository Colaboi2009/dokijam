#pragma once

#include <string>

#include <SDL3/SDL.h>
#include <memory>

#include "point.hpp"

class TextureProxy {
  private:
    SDL_Texture *m_t = NULL;

  public:
    TextureProxy(SDL_Texture *t) : m_t{t} {}
    ~TextureProxy() { SDL_DestroyTexture(m_t); }
	operator SDL_Texture*() { return m_t; }
};

class Texture {
  private:
    std::shared_ptr<TextureProxy> m_texture;
    int m_w, m_h;

  public:
    Texture();
    Texture(std::string filepath);
    Texture(SDL_Surface *s);
    ~Texture();

	float w() const { return m_w; }
	float h() const { return m_h; }
	void tint(int r, int g, int b) { SDL_SetTextureColorMod(*m_texture, r, g, b); }

    void create(SDL_Surface *surf, SDL_ScaleMode = SDL_SCALEMODE_NEAREST);
    void render(SDL_FRect dst);
    void render(Point dst, float scale);

    void render(const SDL_FRect& src, const SDL_FRect& dst);
};
