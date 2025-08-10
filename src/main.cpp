#include "engine/sdl_wrapper.hpp"
#include "ecs/ecs.hpp"
#include "utility.hpp"

#include "tilemap.hpp"

#include <print>
#include <entt/entt.hpp>

SDL sdl;

int main() {
    entt::registry registry;

    SP<Animation> animation = std::make_shared<Animation>("green_junimo.aseprite");
	animation->repeat(0);

	Animation explosionAnimation{"dumb_boom.aseprite"};

    SP<TileMap> tileMap = std::make_shared<TileMap>("tilemap.aseprite");
    //tileMap->setLevel("testing");
    tileMap->setLevel("default");

    // We can use ecs::Movable, or we can decide based on ecs::Velocity,
    // if the entity can be moved

    const entt::entity player = registry.create();
    registry.emplace<ecs::Position>(player, 800, 600);
    registry.emplace<ecs::Velocity>(player);
    registry.emplace<ecs::BoxCollider>(player, -30, 0, 60, 60);
    registry.emplace<ecs::Sprite>(player, animation, 4);
    registry.emplace<ecs::Spawner>(player, ecs::Spawner::Type::Dragoon);
	registry.emplace<ecs::Camera>(player);
	registry.emplace<ecs::DragoonHolder>(player);

	const entt::entity final_bf = registry.create();
	registry.emplace<ecs::Position>(final_bf, 0, 0);
	registry.emplace<ecs::Sprite>(final_bf, std::make_shared<Animation>("final_bossfight_prototype.aseprite"), 4);

	const entt::entity tilemap = registry.create();
	registry.emplace<ecs::Position>(tilemap, 100, 100);
	registry.emplace<ecs::TileMapSprite>(tilemap, tileMap, 4.0f);
    registry.emplace<ecs::TileMapCollider>(tilemap);

    // Register TileMap Level-Collider-Entities
    auto view = registry.view<ecs::Position, ecs::TileMapSprite>();
    for (const auto& [e, position, tileMapSprite] : view.each()) {
        tileMapSprite.tilemap->registerEntities(
            registry, {position.x, position.y},
            tileMapSprite.scale
        );
    }

	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			const entt::entity dragoon = registry.create();
			const int w = 50;
			const int h = 50;
			const int spacing = 100;
			registry.emplace<ecs::Position>(dragoon, x * (w + spacing), y * (h + spacing));
			registry.emplace<ecs::Dragoon>(dragoon);
			registry.emplace<ecs::CircleCollider>(dragoon, 0, 0, w);
			registry.emplace<ecs::Rectangle>(dragoon, w, h, SDL_Color{50, 255, 50, 255});
			auto &exp = registry.emplace<ecs::Explosion>(dragoon);
			exp.radius = spacing + w / 2.f;
			exp.shouldTrigger = false;
		}
	}

	bool stopInput = false;
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
			if (!stopInput) {
				ecs::syncInput(registry, player, e);
			}
        }

		if (!stopInput) {
			ecs::asyncInput(registry, player);
		} else {
			// animation and dialogue
		}

		ecs::heldDragoonMover(registry, player);

        ecs::collision(registry, deltaTime);
        ecs::movement(registry, deltaTime);
		ecs::explosion(registry, explosionAnimation);
        ecs::spawn(registry);
        ecs::cleanup(registry, deltaTime);
        
        ecs::render(registry, sdl);

        sdl.present();
    }

    return 0;
}

