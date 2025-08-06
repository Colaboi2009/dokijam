#pragma once

#include "component.hpp"

#include <entt/entt.hpp>

union SDL_Event;
class SDL;

namespace ecs {

// Systems
void cleanup(entt::registry &registry, const double dt);
void spawn(entt::registry &registry);
void asyncInput(entt::registry &registry, const entt::entity &player);
void syncInput(entt::registry &registry, const entt::entity &player, SDL_Event& event);
void movement(entt::registry &registry, const double dt);
void collision(entt::registry &registry);

void render(entt::registry &registry, SDL& sdl);

}
