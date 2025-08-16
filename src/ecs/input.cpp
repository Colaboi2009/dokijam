#include "ecs.hpp"
#include "ecs_utility.hpp"

#include <SDL3/SDL.h>
#include <print>

namespace ecs {

static void tryPickupDragoon(entt::registry &registry, const entt::entity &player, SDL_Event &event) {
    if (!(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_RIGHT)) {
        return;
    }
    auto [playerPos, playerDragoonHolder] = registry.get<Position, DragoonHolder>(player);
    if (registry.valid(playerDragoonHolder.heldDragoonEntity)) {
        return;
    }
    const SDL_FPoint playerCenter = getColliderCenter(registry, player, playerPos);

    const float pickupDistance = 100;
    auto view = registry.view<Position, Dragoon>();
    for (auto [e, dragoonPos, _] : view.each()) {
        SDL_FPoint dragoonCenter = getColliderCenter(registry, e, dragoonPos);
        if (sqrDist(playerCenter, dragoonCenter) < square(pickupDistance) && pointInCircle(getMousePosition(registry), 30, dragoonCenter)) {
            playerDragoonHolder.heldDragoonEntity = e;
            break;
        }
    }
}

static void tryPlaceDragoon(entt::registry &registry, const entt::entity &player, SDL_Event &event) {
    if (!(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_RIGHT)) {
        return;
    }
    auto [playerPos, playerSpawner, playerDragoonHolder] = registry.get<Position, Spawner, DragoonHolder>(player);
    if (!registry.valid(playerDragoonHolder.heldDragoonEntity)) {
        return;
    }

    const float placeDistance = 100;

    bool wasSwapped = false;
    ;
    auto dragoonHolders = registry.view<Position, DragoonHolder>();
    const SDL_FPoint playerCenter = getColliderCenter(registry, player, playerPos);
    for (auto [e, pos, holder] : dragoonHolders.each()) {
        if (e == player) {
            continue;
        }
        if (sqrDist(playerCenter, {pos.x, pos.y}) < placeDistance) {
            std::swap(playerDragoonHolder.heldDragoonEntity, holder.heldDragoonEntity);
            wasSwapped = true;
            break;
        }
    }
    if (wasSwapped) {
        return;
    }

    playerDragoonHolder.heldDragoonEntity = entt::null;
}

static void tryShootWithDragoon(entt::registry &registry, const entt::entity &player, SDL_Event &event) {
    if (!(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN && event.button.button == SDL_BUTTON_LEFT)) {
        return;
    }
    auto [playerSpawner, playerDragoonHolder] = registry.get<Spawner, DragoonHolder>(player);
    if (!registry.valid(playerDragoonHolder.heldDragoonEntity)) {
        return;
    }

    playerSpawner.data = &registry.get<Dragoon>(playerDragoonHolder.heldDragoonEntity).type;
    playerSpawner.shouldSpawn = true;
}

void syncInput(entt::registry &registry, const entt::entity &player, SDL_Event &event) {
    // stupid thing: place must come before pickup or else if you pickup it immediately puts down lol
    tryPlaceDragoon(registry, player, event);
    tryPickupDragoon(registry, player, event);
    tryShootWithDragoon(registry, player, event);
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        SDL_FPoint mouse = getMousePosition(registry);
    }
}

void asyncInput(entt::registry &registry, const entt::entity &player) {
    auto &velocity = registry.get<Velocity>(player);
    auto &sprite = registry.get<Sprite>(player);

    velocity.dx = 0;
    velocity.dy = 0;

    sprite.animation->stop();

    bool animationChosen = false;
    float speed = .3f;
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
        velocity.dx += speed;
        if (!animationChosen) {
            sprite.animation->repeat(3);
            animationChosen = true;
        }
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A]) {
        velocity.dx -= speed;
        if (!animationChosen) {
            sprite.animation->repeat(2);
            animationChosen = true;
        }
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
        velocity.dy += speed;
        if (!animationChosen) {
            sprite.animation->repeat(0);
            animationChosen = true;
        }
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W]) {
        velocity.dy -= speed;
        if (!animationChosen) {
            sprite.animation->repeat(1);
            animationChosen = true;
        }
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

} // namespace ecs
