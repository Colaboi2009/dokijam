#include "ecs.hpp"

#include "ecs/ecs_utility.hpp"
#include <print>

namespace ecs {

void cleanup(entt::registry &registry, const double dt) {
    auto view = registry.view<JustDieAfter>();
    for (auto [e, dieAfter] : view.each()) {
        //uint64_t now = SDL_GetTicks();
        dieAfter.msToLive -= dt;
        if (dieAfter.msToLive <= 0) {
            bool hasExplosion = registry.any_of<Explosion>(e);
            if (hasExplosion) {
                // trigger an explosion and then destroy
                // probably through a bool on Explosion
                // to a separate Explosion system,
                // depends on the performance cost of EnTT views,
                // but code-organization-wise, it is better to separate to systems
                // that handle their own logic and nothing more
				registry.get<Explosion>(e).shouldTrigger = true;
            } else {
                registry.destroy(e);
            }
        }
    }
}

void spawn(entt::registry &registry) {
    auto view = registry.view<Position, Spawner>();
    for (auto [e, position, spawner] : view.each()) {
        // If an entity has an attached spawner, and also has a transform
        // we can create an entity next to it based on the spawner type

        // If we want mobs to duplicate or spawn minions, we can use this too

        // Or if we want to shoot dragoons, we can spawn them
        // and apply velocity in the direction of mouse cursor
        if (spawner.shouldSpawn) {
            switch (spawner.type) {
                case Spawner::Type::Dragoon: {
                    auto dragoon = registry.create();

					SDL_FPoint mouse = getMousePosition(registry);

                    constexpr float speed = .5f;
					const int w = 20;
					const int h = 20;

					// note(cola): should calculate till end of dragoon
                    float dx = mouse.x - position.x - w / 2.f;
                    float dy = mouse.y - position.y - h / 2.f;

                    float length = std::sqrt(dx * dx + dy * dy);
                    if (length == 0) continue;

					dx /= length;
					dy /= length;

					// calculate how long it will take to travel to the mouse position
					// note(cola): this has a little variance due to dt being little bit inconsistent each frame
					float travelTime = length / speed;

					// cast the data to get the crystal type
					CrystalType *crystalType = (CrystalType *)spawner.data;

                    registry.emplace<Position>(dragoon, position.x, position.y);
                    auto& velocity = registry.emplace<Velocity>(dragoon);
                    velocity.dx = speed * dx;
                    velocity.dy = speed * dy;
					switch (*crystalType) {
						case CrystalType::fire: {
							registry.emplace<Sprite>(dragoon, std::make_shared<Animation>("shots/fire_shot.aseprite"));
						}; break;
						case CrystalType::ice: {
							registry.emplace<Sprite>(dragoon, std::make_shared<Animation>("shots/ice_shot.aseprite"));
						}; break;
						case CrystalType::water: {
							registry.emplace<Sprite>(dragoon, std::make_shared<Animation>("shots/water_shot.aseprite"));
						}; break;
						case CrystalType::lightning: {
							registry.emplace<Sprite>(dragoon, std::make_shared<Animation>("shots/lightning_shot.aseprite"));
						}; break;
						case CrystalType::nature: {
							registry.emplace<Sprite>(dragoon, std::make_shared<Animation>("shots/nature_shot.aseprite"));
						}; break;

					}
                    registry.emplace<JustDieAfter>(dragoon, travelTime);
					registry.emplace<DragoonShot>(dragoon, *crystalType);
                    registry.emplace<BoxCollider>(dragoon, 0, 0, w, h);
					auto &exp = registry.emplace<Explosion>(dragoon);
					exp.radius = 100;
					exp.shouldTrigger = false;
                } break;
				case Spawner::Type::Boss: {
					entt::entity &bossEntity = *(entt::entity *)spawner.data;
					if (!registry.valid(bossEntity)) {
						return;
					}
					Position &bossPos = registry.get<Position>(bossEntity);
					Boss &bossComponent = registry.get<Boss>(bossEntity);

					const int shot_count = 12;
					const float shot_speed = .3f;

					for (int i = 0; i < shot_count; i++) {
						auto bossShot = registry.create();

						float x = cos((360.f / shot_count) * (i + 1) * (1 / 57.f));
						float y = sin((360.f / shot_count) * (i + 1) * (1 / 57.f));
						registry.emplace<Position>(bossShot, x, y);
						float dist = sqrDist({x, y}, {bossPos.x, bossPos.y});
						Point normDir = {x / dist, y / dist};
						registry.emplace<Velocity>(bossShot, normDir.x * shot_speed, normDir.y * shot_speed);
						registry.emplace<Sprite>(bossShot, std::make_shared<Animation>(*bossComponent.shotAnimations[(int)bossComponent.thisShotType]));
						registry.emplace<JustDieAfter>(bossShot, 500 + rand() % 7000);
						registry.emplace<CircleCollider>(bossShot, 0, 0, 5);

						BossShot &shot = registry.emplace<BossShot>(bossShot);
						shot.type = bossComponent.thisShotType;
					}
				} break;
            }
            spawner.shouldSpawn = false;
        }
    }
}

}
