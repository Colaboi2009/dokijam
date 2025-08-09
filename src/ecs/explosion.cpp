#include "ecs.hpp"
#include "utility.hpp"

namespace ecs {

static SDL_FPoint getColliderCenter(entt::entity e, Position originalPos, auto &boxView, auto &circleView) {
    SDL_FPoint center = {originalPos.x, originalPos.y}; // make it center of the collider if it exists
    if (boxView.contains(e)) {
        auto [_, box] = boxView.get(e);
        center.x += box.xOffset + box.width / 2.f;
        center.y += box.yOffset + box.height / 2.f;
    } else if (circleView.contains(e)) {
        auto [_, cir] = circleView.get(e);
        center.x += cir.xOffset;
        center.y += cir.yOffset;
    }
    return center;
}

void explosion(entt::registry &reg, Animation explosionAnimation) {
    std::vector<entt::entity> to_explode;

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
                    to_explode.push_back(entity_box);
                }
            }

            for (auto [entity_cir, pos_cir, col_cir] : circleColliders.each()) {
                if (e == entity_cir) {
                    continue;
                }
                const SDL_FPoint center = {pos.x + col_cir.xOffset, pos.y + col_cir.yOffset};
                if (circlesOverlap(explosion_center, exp.radius, center, col_cir.radius)) {
                    to_explode.push_back(entity_cir);
                }
            }
        }
    }
    for (auto e : to_explode) {
        if (reg.any_of<Explosion>(e) && !reg.any_of<JustDieAfter>(e)) {
            // CHAIN REACTION :D
            // delay by 100 milliseconds to make it *DRAMATIC*
            reg.emplace<JustDieAfter>(e, 100);
        }
    }
    for (auto [e, pos, exp] : view.each()) {
        if (exp.shouldTrigger) {
            SDL_FPoint animation_center = getColliderCenter(e, pos, boxColliders, circleColliders);

            entt::entity exp = reg.create();
            reg.emplace<Position>(exp, animation_center.x, animation_center.y);
            SP<Animation> anim = std::make_shared<Animation>(explosionAnimation);
            anim->play();
            reg.emplace<Sprite>(exp, anim, 5);
            reg.emplace<JustDieAfter>(exp, 1000);

            reg.destroy(e);
        }
    }
}

} // namespace ecs
