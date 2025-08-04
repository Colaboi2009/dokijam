#pragma once

#include "engine/animation.hpp"
#include "i_collider.hpp"
#include "i_element.hpp"

class Player : public IElement, public ICollider {
  private:
    Animation m_sprite;
    SDL_FPoint m_pos;
	// sprites are centered, so use this to match it with the bounding box
	SDL_FPoint m_spriteOffset;
	SDL_FPoint m_bounds;

  public:
    Player();
    ~Player() {};

    void event(SDL_Event &e) override;
    void prerender(float dt) override;
    void render() override;

	SDL_FRect bounds() override;
	void sex(float x) override;
	void sey(float y) override;
};
