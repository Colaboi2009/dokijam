#include "player.hpp"

Player::Player() : m_sprite("tester.aseprite", 4) {
	m_pos = {800, 500};
	m_spriteOffset = {25, 25};
	m_bounds = {50, 50};

	im_immovable = false;
}

void Player::event(SDL_Event &e) {}

void Player::prerender(float dt) {
    float speed = 4;
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
        m_pos.y += speed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_E]) {
        m_pos.y -= speed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F]) {
        m_pos.x += speed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
        m_pos.x -= speed;
    }
}

void Player::render() { m_sprite.render((Point)m_pos + (Point)m_spriteOffset); }

SDL_FRect Player::bounds() {
	return {m_pos.x, m_pos.y, m_bounds.x, m_bounds.y};
}

void Player::sex(float x) {
	m_pos.x = x;
}
void Player::sey(float y) {
	m_pos.y = y;
}
