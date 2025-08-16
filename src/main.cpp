#include "engine/audio.hpp"
#include "engine/sdl_wrapper.hpp"
#include "ecs/ecs.hpp"
#include "utility.hpp"

#include "engine/raw_text.hpp"
#include "tilemap.hpp"

#include "ui.hpp"
#include <entt/entt.hpp>

SDL sdl;
Audio audio;

int main() {
	srand(time(0));
    entt::registry registry;

	TTF_Font *font = TTF_OpenFont("art/sourcecodevf/sourcecodevf.ttf", 64.f);

    SP<Animation> animation = std::make_shared<Animation>("doki.aseprite");
	animation->repeat(0);

	Animation explosionAnimation{"dumb_boom.aseprite"};

    SP<TileMap> tileMap = std::make_shared<TileMap>("tilemap.aseprite");
    tileMap->setLevel("default");

    const entt::entity player = registry.create();
    registry.emplace<ecs::Position>(player, 0, -400);
    registry.emplace<ecs::Velocity>(player);
    registry.emplace<ecs::BoxCollider>(player, -20, -35, 30, 80);
    registry.emplace<ecs::Sprite>(player, animation, .6f);
    registry.emplace<ecs::Spawner>(player, ecs::Spawner::Type::Dragoon);
	registry.emplace<ecs::Camera>(player);
	registry.emplace<ecs::DragoonHolder>(player);
	ecs::Player &playerComponent = registry.emplace<ecs::Player>(player);

	ecs::riversInit(registry);
	ecs::bossInit(registry, player);

	const entt::entity floor = registry.create();
	registry.emplace<ecs::Position>(floor, -1250, -800, 1.f);
	registry.emplace<ecs::Rectangle>(floor, 2500, 1600, SDL_Color{90, 30, 30, 255});

    // Register TileMap Level-Collider-Entities (never used lmao)
    auto view = registry.view<ecs::Position, ecs::TileMapSprite>();
    for (const auto& [e, position, tileMapSprite] : view.each()) {
        tileMapSprite.tilemap->registerEntities(
            registry, {position.x, position.y},
            tileMapSprite.scale
        );
    }

	bool restart = false;
	bool gameWon = false;
	bool showingQuestionScreen = false;

    bool running = true;
    uint64_t lastTime = SDL_GetPerformanceCounter();
	auto BEGINTIME = beginTimer();
	float timeTaken = 0;
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
			if (playerComponent.alive) {
				ecs::syncInput(registry, player, e);
			} else {
				if (e.type == SDL_EVENT_KEY_DOWN) {
					if (e.key.key == SDLK_R) {
						restart = true;
						running = false;
					} else if (e.key.key == SDLK_W) {
						showingQuestionScreen = !showingQuestionScreen;
					}
				}
			}
        }

		if (playerComponent.alive) {
			ecs::asyncInput(registry, player);
			ecs::heldDragoonMover(registry, player);
			ecs::spawnCrystals(registry, 2, deltaTime);
			ecs::moveCrystals(registry);
			ecs::murderCrystals(registry);
			ecs::handleCrystalSheilds(registry, deltaTime);
			ecs::handleRiverDivergence(registry);

			ecs::collision(registry, deltaTime);
			if (ecs::boss(registry, deltaTime, player)) {
				playerComponent.alive = false;
				gameWon = true;
			}
			ecs::movement(registry, deltaTime);
			ecs::explosion(registry, explosionAnimation, audio, 4);
			ecs::spawn(registry);

			ecs::forcePlayerWithinBounds(registry, player);
			ecs::cleanup(registry, deltaTime);
			ecs::render(registry, sdl);
			timeTaken = endTimer(BEGINTIME);
			RawText::Render(std::format("{}", (int)timeTaken), {7, 10}, true, {255, 255, 255, 255}, font, .35f);
		} else {
			if (gameWon) {
				showWonScreen(font, timeTaken);
			} else {
				if (showingQuestionScreen) {
					showQuestionScreen(font);
				} else {
					showDeathScreen(font);
				}
			}
		}
        
        sdl.present();
    }

	if (restart) {
		main(); // LMAOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO im lazy
		/*
		 * listen, in my defense, its BASICALLY equivalent to making an init function and then exporting the game state, but you see
		 * im lazy
		 * i actually did do that
		 * then I thought back and said
		 * "hmm, i think it would be more fitting if this was a recursive main.
		 * This way, its a CHAIN REACTION." (famous last words)
		 * dont send a hitman after me
		 * this is funny as shit and you KNOW it
		*/
	}

    return 0;
}

