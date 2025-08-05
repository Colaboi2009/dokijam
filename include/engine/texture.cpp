#include "sdl_wrapper.hpp"

#include "texture.hpp"
#include <cassert>

extern SDL sdl;

Texture::Texture() : m_texture{NULL}, m_w{-1}, m_h{-1} {}

Texture::Texture(std::string filepath) {
    SDL_Surface *surf = IMG_Load(filepath.c_str());
    create(surf);
    SDL_DestroySurface(surf);
}

Texture::Texture(SDL_Surface *surf) { create(surf); }

Texture::~Texture() {}

void Texture::create(SDL_Surface *surf, SDL_ScaleMode scaleMode) {
    m_texture = std::make_shared<TextureProxy>(sdl.createTextureFromSurface(surf));
    m_w = surf->w;
    m_h = surf->h;
	SDL_SetTextureScaleMode(*m_texture, scaleMode);
}

void Texture::render(SDL_FRect dst) {
    assert(m_texture != nullptr && "Cannot render a NULL texture!");

    // These cases will probably never happen,
    // so no need to check in performance-critical part of code
    // also the offsetting of .x and .y messes up collisions,
    // lets keep the origin of a texture in top-left

    //if (dst.x + dst.y + dst.h + dst.w == 0) {
    //    sdl.renderTexture(*m_texture, NULL, NULL);
	//	return;
    //}
    //
	//if (dst.w + dst.h == 0) {
    //    dst.w = float(m_w);
    //    dst.h = float(m_h);
    //} 
    //
	//dst.x = dst.x - float(dst.w / 2.f);
	//dst.y = dst.y - float(dst.h / 2.f);
	
    sdl.renderTexture(*m_texture, NULL, &dst);
}

void Texture::render(Point dst, float scale) {
	m_w *= scale;
	m_h *= scale;
	render(dst);
	m_w /= scale;
	m_h /= scale;
}
