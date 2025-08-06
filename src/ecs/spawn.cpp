#include "ecs.hpp"

namespace ecs {

void cleanup(entt::registry &registry) {
    auto view = registry.view<ecs::JustDieAfter>();
    for (auto [e, dieAfter] : view.each()) {
        uint64_t now = SDL_GetTicks();
        if (now - dieAfter.createdAt >= dieAfter.msToLive) {
            bool hasExplosion = registry.any_of<ecs::Explosion>(e);
            if (hasExplosion) {
                // trigger an explosion and then destroy
                // probably through a bool on ecs::Explosion
                // to a separate Explosion system,
                // depends on the performance cost of EnTT views,
                // but code-organization-wise, it is better to separate to systems
                // that handle their own logic and nothing more
            } else {
                registry.destroy(e);
            }
        }
    }
}

void spawn(entt::registry &registry) {
    auto view = registry.view<ecs::Transform, ecs::Spawner>();
    for (auto [e, transform, spawner] : view.each()) {
        // If an entity has an attached spawner, and also has a transform
        // we can create an entity next to it based on the spawner type

        // If we want mobs to duplicate or spawn minions, we can use this too

        // Or if we want to shoot dragoons, we can spawn them
        // and apply velocity in the direction of mouse cursor
        if (spawner.shouldSpawn) {
            switch (spawner.type) {
                case ecs::Spawner::Type::Dragoon: {
                    auto dragoon = registry.create();

                    float mouseX = 0.0f;
                    float mouseY = 0.0f;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    float dx = mouseX - transform.box.x;
                    float dy = mouseY - transform.box.y;

                    float length = std::sqrt(dx * dx + dy * dy);
                    if (length == 0) continue;

                    dx /= length;
                    dy /= length;

                    constexpr float speed = 2.0f;

                    registry.emplace<ecs::Transform>(dragoon, transform.box);
                    auto& velocity = registry.emplace<ecs::Velocity>(dragoon);
                    velocity.dx = speed * dx;
                    velocity.dy = speed * dy;
                    registry.emplace<ecs::Shape>(dragoon, SDL_Color{255, 0, 0, 255});
                    registry.emplace<ecs::JustDieAfter>(dragoon, SDL_GetTicks(), (rand() % 600) + 500);
                    // For fun
                    registry.emplace<ecs::BoxCollider>(dragoon);
                    break;
                }
            }
            spawner.shouldSpawn = false;
        }
    }
}

}
