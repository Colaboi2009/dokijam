#pragma once

#include "engine/texture.hpp"
#include <SDL3/SDL_rect.h>
#include <string>

// a Sprite is something that checks for collisions, holds its position, and renders itself as a texture onto the screen.
class Sprite {
protected:
	SDL_FPoint m_pos;
	Texture m_tex;

public:
	Sprite(std::string filepath, SDL_FPoint pos);

	void render();
	bool checkCollision();
};
