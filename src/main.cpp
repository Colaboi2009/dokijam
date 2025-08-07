#include "engine/sdl_wrapper.hpp"
#include "ecs/ecs.hpp"
#include "utility.hpp"

#include "tilemap.hpp"

#include <entt/entt.hpp>

SDL sdl;

int main() {
    entt::registry registry;

    SP<Animation> animation = std::make_shared<Animation>("green_junimo.aseprite");
	animation->repeat(0);

    SP<TileMap> tileMap = std::make_shared<TileMap>("tilemap.aseprite");
    //tileMap->setLevel("testing");
    tileMap->setLevel("default");

    // We can use ecs::Movable, or we can decide based on ecs::Velocity,
    // if the entity can be moved

    const entt::entity player = registry.create();
    registry.emplace<ecs::Position>(player, 800, 600);
    registry.emplace<ecs::Velocity>(player);
    registry.emplace<ecs::CircleCollider>(player, -30, 0, 60);
    registry.emplace<ecs::Sprite>(player, animation, 4);
    registry.emplace<ecs::Spawner>(player, ecs::Spawner::Type::Dragoon);

    const entt::entity platform = registry.create();
    registry.emplace<ecs::Position>(platform, 300, 800);
    registry.emplace<ecs::Velocity>(platform);
    registry.emplace<ecs::BoxCollider>(platform, 0, 0, 80, 30);
    registry.emplace<ecs::Rectangle>(platform, 80, 30, SDL_Color{255, 255, 255, 255});

    const entt::entity YOU_CANT_MOVE_ME = registry.create();
    registry.emplace<ecs::Position>(YOU_CANT_MOVE_ME, 500, 600);
    registry.emplace<ecs::BoxCollider>(YOU_CANT_MOVE_ME, 0, 0, 120, 30);
    registry.emplace<ecs::Rectangle>(YOU_CANT_MOVE_ME, 120, 30, SDL_Color{0, 0, 0, 255});

	const entt::entity CIRCLE = registry.create();
	registry.emplace<ecs::Position>(CIRCLE, 1000, 600);
	registry.emplace<ecs::CircleCollider>(CIRCLE, 0, 0, 100);

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
        ecs::collision(registry, deltaTime);
        ecs::movement(registry, deltaTime);
        ecs::spawn(registry);
        ecs::cleanup(registry, deltaTime);
        
        ecs::render(registry, sdl);

        tileMap->render(Point{100.0f, 100.0f}, 4.0f);

        sdl.present();
    }

    return 0;
}
