#include "engine/sdl_wrapper.hpp"
#include "ecs/ecs.hpp"
#include "utility.hpp"

#include <entt/entt.hpp>

SDL sdl;

int main() {
    entt::registry registry;

    SP<Animation> animation = std::make_shared<Animation>("green_junimo.aseprite");
	animation->repeat(0);

    // We can use ecs::Movable, or we can decide based on ecs::Velocity,
    // if the entity can be moved

    const entt::entity player = registry.create();
    registry.emplace<ecs::Transform>(player, 800, 500, 50, 50);
    registry.emplace<ecs::Velocity>(player);
    registry.emplace<ecs::BoxCollider>(player);
    registry.emplace<ecs::Sprite>(player, animation);
    registry.emplace<ecs::Spawner>(player, ecs::Spawner::Type::Dragoon);

    const entt::entity platform = registry.create();
    registry.emplace<ecs::Transform>(platform, 300, 800, 80, 30);
    registry.emplace<ecs::Velocity>(platform);
    registry.emplace<ecs::BoxCollider>(platform);
    registry.emplace<ecs::Shape>(platform, SDL_Color{255, 255, 255, 255});

    const entt::entity YOU_CANT_MOVE_ME = registry.create();
    registry.emplace<ecs::Transform>(YOU_CANT_MOVE_ME, 500, 600, 120, 30);
    registry.emplace<ecs::BoxCollider>(YOU_CANT_MOVE_ME);
    registry.emplace<ecs::Shape>(YOU_CANT_MOVE_ME, SDL_Color{0, 0, 0, 255});

    bool running = true;
    uint64_t lastTime = SDL_GetPerformanceCounter();

    while (running) {
        uint64_t now = SDL_GetPerformanceCounter();
        double deltaTime = static_cast<double>(now - lastTime) * 1000.0 / SDL_GetPerformanceFrequency();
        lastTime = now;

        sdl.clear({40, 40, 40, 255});

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                running = false;
            }
            ecs::syncInput(registry, player, e);
        }

        ecs::asyncInput(registry, player);
        ecs::collision(registry);
        ecs::movement(registry, deltaTime);
        ecs::spawn(registry);
        ecs::cleanup(registry);
        
        ecs::render(registry, sdl);

        sdl.present();
    }

    return 0;
}
