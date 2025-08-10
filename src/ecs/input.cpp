#include "ecs.hpp"
#include "ecs_utility.hpp"

#include <SDL3/SDL.h>

namespace ecs {

static void tryPickupDragoon(entt::registry &registry, const entt::entity &player, SDL_Event &event) {
    if (!(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_RIGHT)) {
        return;
    }
    auto [playerPos, playerDragoonHolder] = registry.get<Position, DragoonHolder>(player);
    const SDL_FPoint playerCenter = getColliderCenter(registry, player, playerPos);

    const float pickupDistance = 100;
    auto view = registry.view<Position, Dragoon>();
    for (auto [e, dragoonPos] : view.each()) {
        SDL_FPoint dragoonCenter = getColliderCenter(registry, e, dragoonPos);
        if (sqrDist(playerCenter, dragoonCenter) < square(pickupDistance) && pointInCircle(getMousePosition(registry), 30, dragoonCenter)) {
            playerDragoonHolder.heldDragoonEntity = e;
            break;
        }
    }
}

static void tryThrowDragoon(entt::registry &registry, const entt::entity &player, SDL_Event &event) {
    if (!(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT)) {
        return;
    }
    auto [playerSpawner, playerDragoonHolder] = registry.get<Spawner, DragoonHolder>(player);
    if (!registry.valid(playerDragoonHolder.heldDragoonEntity)) {
        return;
    }
    playerDragoonHolder.heldDragoonEntity = entt::null;
    playerSpawner.shouldSpawn = true;
}

void syncInput(entt::registry &registry, const entt::entity &player, SDL_Event &event) {
    tryPickupDragoon(registry, player, event);
    tryThrowDragoon(registry, player, event);
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
    auto view = registry.view<ecs::Position, ecs::Velocity>();
    for (auto [e, position, velocity] : view.each()) {
        position.x += velocity.dx * dt;
        position.y += velocity.dy * dt;
    }
}

// this too :D
void heldDragoonMover(entt::registry &registry, const entt::entity &player) {
    auto [playerPos, playerCol, holder] = registry.get<Position, BoxCollider, DragoonHolder>(player);

    SDL_FPoint playerCenter = getColliderCenter(player, playerPos, &playerCol, nullptr);
    if (registry.valid(holder.heldDragoonEntity)) {
        auto [pos, col] = registry.get<Position, CircleCollider>(holder.heldDragoonEntity);
        pos.x = playerCenter.x;
        pos.y = playerPos.y - col.radius;
    }
}

} // namespace ecs
