#include <entt/entt.hpp>

#include "component.hpp"

namespace ecs {

CrystalType currentShotCrystalType = (CrystalType)0;
entt::entity bossEntity;

void bossInit(entt::registry &r, entt::entity player) {
    bossEntity = r.create();
    r.emplace<Position>(bossEntity, 0, 0);
    Boss &bossComp = r.emplace<Boss>(bossEntity, 8000.f);
    Spawner &spawner = r.emplace<Spawner>(bossEntity, Spawner::Type::Boss);
    spawner.data = (void *)&bossEntity;

	const float w = 350.f;
	r.emplace<BoxCollider>(bossEntity, -w / 2.f, -w / 2.f - 15, w, w);
	r.emplace<Sprite>(bossEntity, std::make_shared<Animation>("dad.aseprite"), .6f);


    bossComp.shotAnimations = {
        std::make_shared<Animation>("shots/fire_shot.aseprite"),      std::make_shared<Animation>("shots/water_shot.aseprite"),
        std::make_shared<Animation>("shots/ice_shot.aseprite"),       std::make_shared<Animation>("shots/nature_shot.aseprite"),
        std::make_shared<Animation>("shots/lightning_shot.aseprite"),
    };

    // lightning comes later
    bossComp.remainingTypes = {
        CrystalType::fire,
        CrystalType::water,
        CrystalType::ice,
        CrystalType::nature,
    };
}

static void bossShots(entt::registry &r, Boss &bossComp, entt::entity player) {
    Position &bossPos = r.get<Position>(bossEntity);
    Position &playerPos = r.get<Position>(player);

    auto view = r.view<JustDieAfter, BossShot, Velocity, CircleCollider, Sprite>();
    for (auto [e, death, shot, vel, circCol, sprite] : view.each()) {
        bool isHalfLife = death.msToLive < death.totalAllowedTime / 2.f;
        switch (shot.type) {
            case CrystalType::fire: {
                if (isHalfLife) {
                    vel.dx = 0;
                    vel.dy = 0;
                    circCol.radius = 64;
                    if (!shot.hit) {
                        shot.hit = true;
                        sprite.animation->stepFrame();
                    }
                }
				if (circCol.hit == player) {
					r.get<Player>(player).alive = false;
				}
            }; break;
            case CrystalType::water: {
                if (circCol.hit == player) {
                    float dist = sqrt(sqrDist({bossPos.x, bossPos.y}, {playerPos.x, playerPos.y}));
                    Point dir = Point(playerPos.x, playerPos.y) - Point(bossPos.x, bossPos.y);
                    Point dirNorm = {dir.x / dist, dir.y / dist};
                    const float kb_amount = 300;
                    playerPos.x += dirNorm.x * kb_amount;
                    playerPos.y += dirNorm.y * kb_amount;
                }
            }; break;
            case CrystalType::nature: {
                vel.dx *= 1.02f;
                vel.dy *= 1.02f;
                if (circCol.hit == player) {
                    r.get<Player>(player).alive = false;
                }
            }; break;
            case CrystalType::ice: {
                sprite.animation->setFrame(0);
                if (circCol.hit == player || shot.hit) {
                    shot.hit = true;
                    Velocity &playerVel = r.get<Velocity>(player);
                    playerVel.dx = playerVel.dy = vel.dx = vel.dy = 0;
                    sprite.animation->setFrame(1);
                }
            }; break;
            case CrystalType::lightning: {
                vel.dx *= 0.94f;
                vel.dy *= 0.94f;
                float dist = sqrt(sqrDist({bossPos.x, bossPos.y}, {playerPos.x, playerPos.y}));
                Point dir = Point(playerPos.x, playerPos.y) - Point(bossPos.x, bossPos.y);
                Point dirNorm = {dir.x / dist, dir.y / dist};
                const float viscosity = 0.01f;
                vel.dx += dirNorm.x * viscosity;
                vel.dy += dirNorm.y * viscosity;
                if (circCol.hit == player) {
                    r.get<Player>(player).alive = false;
                }
            }; break;
        }
    }
}

bool boss(entt::registry &r, float dt, entt::entity player) {
    Boss &bossComponent = r.get<Boss>(bossEntity);

    auto originsView = r.view<CrystalOrigin>();
    auto riversView = r.view<River>();

    bossComponent.msTimeSinceLastShot += dt;
    if (bossComponent.msTimeSinceLastShot > bossComponent.shotInterval) {
        bossComponent.msTimeSinceLastShot = 0;

        Spawner &spawner = r.get<Spawner>(bossEntity);
        spawner.shouldSpawn = true;
        if (!bossComponent.isSecondPhase) {
			int index = rand() % (bossComponent.remainingTypes.size());
			bossComponent.thisShotType = bossComponent.remainingTypes[index];
            for (auto [e, origin] : originsView.each()) {
                origin.stop = true;
                if (bossComponent.thisShotType == origin.type) {
                    origin.stop = false;
                }
            }
            bossComponent.isSecondPhase = true;
            for (auto [e, river] : riversView.each()) {
				if (river.type == CrystalType::lightning) {
					continue;
				}
                river.normal->setFrame(1);
                if (bossComponent.thisShotType == river.type) {
                    river.normal->setFrame(0);
                }
                if (!river.isDiverged) {
                    bossComponent.isSecondPhase = false;
                } else {
					// one at a time, since its not possible to remove all at once (dont mention it)
					int removeIndex = -1;
					for (int i = 0; i < bossComponent.remainingTypes.size(); i++) {
						if (bossComponent.remainingTypes[i] == river.type) {
							removeIndex = i;
							break;
						}
					}
					if (removeIndex != -1) {
						bossComponent.remainingTypes.erase(bossComponent.remainingTypes.begin() + removeIndex);
					}
				}
            }
        } else {
			bossComponent.thisShotType = CrystalType::lightning;
            for (auto [e, origin] : originsView.each()) {
                if (origin.type == CrystalType::lightning) {
                    origin.stop = false;
                }
            }
            for (auto [e, river] : riversView.each()) {
                river.normal->setFrame(0);
				if (river.type == CrystalType::lightning && river.isDiverged) {
					return true;
				}
            }
        }
    }

    bossShots(r, bossComponent, player);
	return false;
}
} // namespace ecs
