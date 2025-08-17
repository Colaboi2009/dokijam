#include "engine/audio.hpp"
#include "engine/sdl_wrapper.hpp"
#include "ecs/ecs.hpp"
#include "utility.hpp"

#include "engine/raw_text.hpp"
#include "tilemap.hpp"

#include "ui.hpp"
#include <entt/entt.hpp>

#include <emscripten.h>

/*
	note: Edited by Skulaurun for Emscripten,
	This is spaghetti anyway, so quality is not a concern.

	The 'restart' works only sometimes, as resource loading is tied
	to game object initialization......
*/

SDL sdl;
Audio audio;

void init();

struct Game {
	entt::registry registry;
	entt::entity player;
	ecs::Player* playerComponent;

	SP<Animation> animation;
	SP<Animation> explosionAnimation;

	std::chrono::steady_clock::time_point BEGINTIME;

	bool restart = false;
	bool gameWon = false;
	bool showingQuestionScreen = false;
};

Game* game;
bool running = true;
uint64_t lastTime;

static void mainLoop() {
	static TTF_Font* font = TTF_OpenFont("art/sourcecodevf/sourcecodevf.ttf", 64.f);

	if (!running) {
		#ifdef __EMSCRIPTEN__
			emscripten_cancel_main_loop();
		#endif
	}

	if (game->restart) {
		init();
	}

	uint64_t now = SDL_GetPerformanceCounter();
	double deltaTime = static_cast<double>(now - lastTime) * 1000.0 / SDL_GetPerformanceFrequency();
	lastTime = now;

	sdl.clear({40, 40, 40, 255});

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			running = false;
		}
		if (game->playerComponent->alive) {
			ecs::syncInput(game->registry, game->player, e);
		}
		if (e.type == SDL_EVENT_KEY_DOWN) {
			if (e.key.key == SDLK_R) {
				game->restart = true;
			} else if (e.key.key == SDLK_W && !game->playerComponent->alive) {
				game->showingQuestionScreen = !game->showingQuestionScreen;
			}
		}
	}

	float timeTaken = 0;
	if (game->playerComponent->alive) {
		ecs::asyncInput(game->registry, game->player);
		ecs::heldDragoonMover(game->registry, game->player);
		ecs::spawnCrystals(game->registry, 2, deltaTime);
		ecs::moveCrystals(game->registry);
		ecs::murderCrystals(game->registry);
		ecs::handleCrystalSheilds(game->registry, deltaTime);
		ecs::handleRiverDivergence(game->registry);

		ecs::collision(game->registry, deltaTime);
		if (ecs::boss(game->registry, deltaTime, game->player)) {
			game->playerComponent->alive = false;
			game->gameWon = true;
		}
		ecs::movement(game->registry, deltaTime);
		ecs::explosion(game->registry, *game->explosionAnimation, audio, 4);
		ecs::spawn(game->registry);

		ecs::forcePlayerWithinBounds(game->registry, game->player);
		ecs::cleanup(game->registry, deltaTime);
		ecs::render(game->registry, sdl);
		timeTaken = endTimer(game->BEGINTIME);
		RawText::Render(std::format("{}", (int)timeTaken), {7, 10}, true, {255, 255, 255, 255}, font, .35f);
	} else {
		if (game->gameWon) {
			showWonScreen(font, timeTaken);
		} else {
			if (game->showingQuestionScreen) {
				showQuestionScreen(font);
			} else {
				showDeathScreen(font);
			}
		}
	}
	
	sdl.present();
}

void init() {
	delete game;
	game = new Game();

	for (int i = 0; i < 10; i++) { // im lazy, just queue it 10 times
		audio.queueMusic("art/doki_music_caveofancient.wav");
	}

    game->animation = std::make_shared<Animation>("doki.aseprite");
	game->animation->repeat(0);
	game->explosionAnimation = std::make_shared<Animation>("explosion_real.gif");

    //game->tileMap = std::make_shared<TileMap>("tilemap.aseprite");
    //game->tileMap->setLevel("default");

    game->player = game->registry.create();
    game->registry.emplace<ecs::Position>(game->player, 0, -400);
    game->registry.emplace<ecs::Velocity>(game->player);
    game->registry.emplace<ecs::BoxCollider>(game->player, -20, -35, 30, 80);
    game->registry.emplace<ecs::Sprite>(game->player, game->animation, .6f);
    game->registry.emplace<ecs::Spawner>(game->player, ecs::Spawner::Type::Dragoon);
	game->registry.emplace<ecs::Camera>(game->player);
	game->registry.emplace<ecs::DragoonHolder>(game->player);
	game->playerComponent = &game->registry.emplace<ecs::Player>(game->player);

	ecs::riversInit(game->registry);
	ecs::bossInit(game->registry, game->player);

	const entt::entity floor = game->registry.create();
	game->registry.emplace<ecs::Position>(floor, -1250, -700, 1.f);
	game->registry.emplace<ecs::Rectangle>(floor, 2500, 1400, SDL_Color{90, 30, 30, 255});

    // Register TileMap Level-Collider-Entities (never used lmao)
    auto view = game->registry.view<ecs::Position, ecs::TileMapSprite>();
    for (const auto& [e, position, tileMapSprite] : view.each()) {
        tileMapSprite.tilemap->registerEntities(
            game->registry, {position.x, position.y},
            tileMapSprite.scale
        );
    }

	lastTime = SDL_GetPerformanceCounter();
	game->BEGINTIME = beginTimer();
}

int main() {
	srand(time(0));

	init();
	#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop(mainLoop, 0, 1);
	#else
		while (running) {
			mainLoop();
		}
	#endif

	//if (restart) {
	//	main(); // LMAOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO im lazy
	//	/*
	//	 * listen, in my defense, its BASICALLY equivalent to making an init function and then exporting the game state, but you see
	//	 * im lazy
	//	 * i actually did do that
	//	 * then I thought back and said
	//	 * "hmm, i think it would be more fitting if this was a recursive main.
	//	 * This way, its a CHAIN REACTION." (famous last words)
	//	 * dont send a hitman after me
	//	 * this is funny as shit and you KNOW it
	//	*/
	//}

    return 0;
}

