#include "ecs.hpp"
#include "utility.hpp"

#include <SDL3/SDL.h>

namespace ecs {

void syncInput(entt::registry &registry, const entt::entity &player, SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        auto& spawner = registry.get<ecs::Spawner>(player);
        spawner.shouldSpawn = true;
    }
}

void asyncInput(entt::registry &registry, const entt::entity &player) {
    auto &velocity = registry.get<ecs::Velocity>(player);

    velocity.dx = 0;
    velocity.dy = 0;

    float speed = 1;
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
        velocity.dy += speed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_E]) {
        velocity.dy -= speed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F]) {
        velocity.dx += speed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
        velocity.dx -= speed;
    }
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_SPACE]) {
		SP<Animation> anim = registry.get<ecs::Sprite>(player).animation;
		anim->once(1);
	}
}

// Let's leave this here for now
void movement(entt::registry &registry, const double dt) {
    auto view = registry.view<ecs::Transform, ecs::Velocity>();
    for (auto [e, position, velocity] : view.each()) {
        position.box.x += velocity.dx * dt;
        position.box.y += velocity.dy * dt;
    }
}

}
