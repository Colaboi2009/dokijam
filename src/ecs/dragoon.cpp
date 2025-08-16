#include "SDL3/SDL_rect.h"
#include "ecs/component.hpp"
#include "ecs/ecs_utility.hpp"
#include "entt/entity/fwd.hpp"
#include <print>

namespace ecs {

static const float crystal_speed = .2f;
static const float max_dist = 15.f;

void heldDragoonMover(entt::registry &registry, const entt::entity &player) {
    auto [playerPos, playerCol, holder] = registry.get<Position, BoxCollider, DragoonHolder>(player);

    SDL_FPoint playerCenter = getColliderCenter(player, playerPos, &playerCol, nullptr);
    if (registry.valid(holder.heldDragoonEntity)) {
        auto [pos, col] = registry.get<Position, CircleCollider>(holder.heldDragoonEntity);
        pos.x = playerCenter.x;
        pos.y = playerPos.y - col.radius;
    }
}

void spawnCrystals(entt::registry &registry, float crystalScale, float dt) {
    auto crystalOriginsView = registry.view<Position, CrystalOrigin>();

    for (auto [e, pos, origin] : crystalOriginsView.each()) {
        origin.msTimeSinceLastSpawn += dt;
        if (origin.msTimeSinceLastSpawn > origin.msSpawnInterval && !origin.stop) {
            origin.msTimeSinceLastSpawn = 0;

            entt::entity crystal = registry.create();
            registry.emplace<Position>(crystal, pos);
			registry.emplace<Velocity>(crystal, origin.dx * crystal_speed, origin.dy * crystal_speed);
            registry.emplace<Crystal>(crystal, origin.type);
			registry.emplace<CrystalSheild>(crystal, 1000.f);
			registry.emplace<BoxCollider>(crystal, -5, -20, 12, 50);

			SP<Animation> anim = std::make_shared<Animation>(*origin.crystalAnimation);
			anim->repeat(0);
            registry.emplace<Sprite>(crystal, anim, crystalScale);

			registry.emplace<Explosion>(crystal, 150.f, [&registry, crystal, origin] (entt::entity culprit) {
				if (auto shot = registry.try_get<DragoonShot>(culprit)) {
					CrystalSheild &s = registry.get<CrystalSheild>(crystal);
					return !s.isOn && shot->targetType == origin.type;
				}
				if (auto crystal = registry.try_get<Crystal>(culprit)) {
					return crystal->type == origin.type;
				}
				return false;
			});
        }
    }
}

void moveCrystals(entt::registry &registry) {
    auto crystalView = registry.view<Position, Velocity, Crystal>();
	auto directionChangersView = registry.view<Position, CrystalDirectionChanger>();
	for (auto [crystalEntity, crystalPos, crystalVel, _] : crystalView.each()) {
		for (auto [dcE, dcPos, dcComp] : directionChangersView.each()) {
			if (sqrDist({dcPos.x, dcPos.y}, {crystalPos.x, crystalPos.y}) < max_dist) {
				crystalVel.dx = dcComp.dx * crystal_speed;
				crystalVel.dy = dcComp.dy * crystal_speed;
			}
		}
	}
}

void murderCrystals(entt::registry &reg) {
	auto crystalView = reg.view<Position, Crystal>();
	auto murdererView = reg.view<Position, CrystalMurderer>();
	for (auto [crystalEntity, crystalPos, _] : crystalView.each()) {
		for (auto [murdererEntity, murdererPos] : murdererView.each()) {
			if (crystalEntity == murdererEntity) {
				continue; // 1 DAY LEFT, THIS CODE IS EXTREMELY INTERDEPENDENT BUT WHOOOO CARES (murderers have crystals on them so they die and i get to know which rivers to diverge)
			}
			if (sqrDist({murdererPos.x, murdererPos.y}, {crystalPos.x, crystalPos.y}) < 100) {
				reg.destroy(crystalEntity);
			}
		}
	}
}

void handleCrystalSheilds(entt::registry &reg, float dt) {
	auto crystalView = reg.view<Sprite, CrystalSheild>();
	for (auto [e, sprite, sheild] : crystalView.each()) {
		sheild.msLiveTime += dt;
		if (sheild.msLiveTime > sheild.timeToEnable) {
			sheild.isOn = true;
		}
	}
	for (auto [e, sprite, sheild] : crystalView.each()) {
		if (sheild.isOn) {
			sprite.animation->repeat(1);
		} else {
			sprite.animation->repeat(0);
		}
	}
}

void handleRiverDivergence(entt::registry &registry) {
	auto riverView = registry.view<River, Sprite>();
	auto murdererView = registry.view<Crystal, CrystalMurderer>();
	auto crystalsView = registry.view<Crystal>();
	for (auto [e, river, sprite] : riverView.each()) {
		river.isDiverged = true;
		sprite.animation = river.diverged;
		for (auto [murderEntity, crystal] : murdererView.each()) {
			if (crystal.type == river.type) {
				river.isDiverged = false;
				sprite.animation = river.normal;
			}	
		}
	}
	for (auto [e, river, sprite] : riverView.each()) {
		if (river.isDiverged) {
			for (auto [ecrys, crystal] : crystalsView.each()) {
				if (river.type == crystal.type) {
					registry.destroy(ecrys);
				}
			}
		}
	}
}

} // namespace ecs
