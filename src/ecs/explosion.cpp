#include "ecs.hpp"
#include "engine/audio.hpp"
#include "utility.hpp"
#include "ecs_utility.hpp"

namespace ecs {

void explosion(entt::registry &reg, Animation &explosionAnimation, Audio &audio, int scale) {
    std::vector<std::pair<entt::entity, entt::entity>> culprit_exploded;

    auto view = reg.view<Position, Explosion>();
    auto boxColliders = reg.view<Position, BoxCollider>();
    auto circleColliders = reg.view<Position, CircleCollider>();
    for (auto [e, pos, exp] : view.each()) {
        if (exp.shouldTrigger) {
            SDL_FPoint explosion_center = getColliderCenter(e, pos, boxColliders, circleColliders);
            for (auto [entity_box, pos_box, col_box] : boxColliders.each()) {
                if (e == entity_box) {
                    continue;
                }
                const SDL_FRect rect = {pos_box.x + col_box.xOffset, pos_box.y + col_box.yOffset, col_box.width, col_box.height};
                if (circleInFRect(explosion_center, exp.radius, rect)) {
                    culprit_exploded.push_back({e, entity_box});
                }
            }

            for (auto [entity_cir, pos_cir, col_cir] : circleColliders.each()) {
                if (e == entity_cir) {
                    continue;
                }
                const SDL_FPoint center = {pos_cir.x + col_cir.xOffset, pos_cir.y + col_cir.yOffset};
                if (circlesOverlap(explosion_center, exp.radius, center, col_cir.radius)) {
                    culprit_exploded.push_back({e, entity_cir});
                }
            }
        }
    }
    for (auto e : culprit_exploded) {
        if (Explosion *exp = reg.try_get<Explosion>(e.second)) {
			if (exp->question(e.first)) {
				// CHAIN REACTION :D
				// delay by 100 milliseconds to make it *DRAMATIC*
				reg.emplace_or_replace<JustDieAfter>(e.second, 100);
			}
        }
    }
    for (auto [e, pos, exp] : view.each()) {
        if (exp.shouldTrigger) {
            SDL_FPoint animation_center = getColliderCenter(e, pos, boxColliders, circleColliders);

            entt::entity exp = reg.create();
            reg.emplace<Position>(exp, animation_center.x, animation_center.y);
            SP<Animation> anim = std::make_shared<Animation>(explosionAnimation);
            anim->play();
            reg.emplace<Sprite>(exp, anim, scale);
            reg.emplace<JustDieAfter>(exp, anim->getPlayLength());

            reg.destroy(e);
			audio.playSFX("art/sfx/explosion.wav");
        }
    }
}

} // namespace ecs
