#include "raw_text.hpp"
#include <cmath>

RawText::RawText(std::string str, SDL_Color c, TTF_Font *font, int allowedHeight) {
	SDL_Surface *surf;
	if (allowedHeight == 0) {
		surf = TTF_RenderText_Solid(font, str.c_str(), 0, c);
	} else {
		const int fontHeight = TTF_GetFontSize(font);
		const int lineCount = std::floor(allowedHeight / fontHeight);
		if (str.contains(" ")) {
			surf = TTF_RenderText_Solid_Wrapped(font, str.c_str(), 0, c, str.size() / lineCount * fontHeight);
		} else {
			surf = TTF_RenderText_Solid_Wrapped(font, str.c_str(), 0, c, 0);
		}
	}
	m_texture.create(surf, SDL_SCALEMODE_LINEAR);
	SDL_DestroySurface(surf);
}

RawText::~RawText() {
}

void RawText::render(SDL_FRect r) {
	if (m_alignLeft) {
		r.x += m_texture.w() / 2.f;
	}
	m_texture.render(r, m_scale);
}

Texture RawText::Render(std::string str, SDL_FRect r, bool alignLeft, SDL_Color c, TTF_Font *font, float scale, bool render) {
	if (str.size() < 1) {
		return {};
	}
	RawText t{str, c, font};
	t.alignLeft(alignLeft);
	t.scale(scale);
	if (render) {
		t.render(r);
	}
	return t.tex();
}
