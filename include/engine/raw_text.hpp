#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "texture.hpp"

class RawText {
  private:
    Texture m_texture;
	bool m_alignLeft{false};
	float m_scale = 1;

  public:
    RawText(std::string, SDL_Color, TTF_Font *, int allowedHeight = 0);
    ~RawText();

	void alignLeft(bool b) { m_alignLeft = b; }
	void scale(float scale) { m_scale = scale; }

	Texture tex() { return m_texture; }
    void render(SDL_FRect);

  public:
	// returns the texture of the text
    static Texture Render(std::string, SDL_FRect, bool alignLeft, SDL_Color, TTF_Font *, float scale = 1, bool render = true);
};
