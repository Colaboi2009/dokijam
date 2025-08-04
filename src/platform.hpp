#pragma once

#include "i_collider.hpp"
#include "i_element.hpp"
class Platform : public IElement, public ICollider {
private:
	SDL_FRect m_rect;
	SDL_Color m_color;

public:
	Platform(SDL_FRect, SDL_Color);

    void event(SDL_Event &e) override {}
    void prerender(float dt) override {};

    void render() override;
	SDL_FRect bounds() override;
	void sex(float x) override;
	void sey(float y) override;
};
