#pragma once

#include <entt/entt.hpp>

#include "component.hpp"
#include "engine/animation.hpp"
#include "engine/audio.hpp"

union SDL_Event;
class SDL;

namespace ecs {

// inits
void riversInit(entt::registry &r);
void bossInit(entt::registry &r);

// Systems
void cleanup(entt::registry &registry, const double dt);
void spawn(entt::registry &registry);
void explosion(entt::registry &registry, Animation &explosionAnimation, Audio &audio, int scale);
void asyncInput(entt::registry &registry, const entt::entity &player);
void syncInput(entt::registry &registry, const entt::entity &player, SDL_Event& event);
void movement(entt::registry &registry, const double dt);
void collision(entt::registry &registry, const double dt);

void heldDragoonMover(entt::registry &registry, const entt::entity &player);
// idk how to pass in crystal scale... it shouldve just been in Animation class...
void spawnCrystals(entt::registry &registry, float crystalScale, float dt);
void moveCrystals(entt::registry &registry);
void murderCrystals(entt::registry &reg);
void handleCrystalSheilds(entt::registry &reg, float dt);
void handleRiverDivergence(entt::registry &registry);

void forcePlayerWithinBounds(entt::registry &r, entt::entity player);

// boss
void bossInit(entt::registry &r, entt::entity player);
bool boss(entt::registry &r, float dt, entt::entity player);

void render(entt::registry &registry, SDL& sdl);

}
