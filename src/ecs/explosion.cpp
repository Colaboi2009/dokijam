#include "ecs.hpp"
#include "utility.hpp"

namespace ecs {

static void triggerExplosion(entt::registry &reg, entt::entity e) {
    if (reg.any_of<Explosion>(e) && !reg.any_of<JustDieAfter>(e)) {
        // CHAIN REACTION :D
        // delay by 100 milliseconds to make it *DRAMATIC*
        reg.emplace<JustDieAfter>(e, 100);
    }
}

void explosion(entt::registry &reg) {
    std::vector<entt::entity> to_explode;

    auto view = reg.view<Position, Explosion>();
    for (auto [e, pos, exp] : view.each()) {
        if (exp.shouldTrigger) {
            auto boxColliders = reg.view<Position, BoxCollider>();
            for (auto [entity_box, pos_box, col_box] : boxColliders.each()) {
                if (e == entity_box) {
                    continue;
                }
                const SDL_FRect rect = {pos_box.x + col_box.xOffset, pos_box.y + col_box.yOffset, col_box.width, col_box.height};
                if (circleInFRect({pos.x, pos.y}, exp.radius, rect)) {
                    to_explode.push_back(entity_box);
                }
            }

            auto circleColliders = reg.view<Position, CircleCollider>();
            for (auto [entity_cir, pos_cir, col_cir] : circleColliders.each()) {
                if (e == entity_cir) {
                    continue;
                }
                const SDL_FPoint center = {pos.x + col_cir.xOffset, pos.y + col_cir.yOffset};
                if (circlesOverlap({pos.x, pos.y}, exp.radius, center, col_cir.radius)) {
                    to_explode.push_back(entity_cir);
                }
            }
        }
    }
    for (auto e : to_explode) {
        triggerExplosion(reg, e);
    }
    for (auto [e, pos, exp] : view.each()) {
        if (exp.shouldTrigger) {
            // TODO(cola): render an explosion animation
            reg.destroy(e);
        }
    }
}

} // namespace ecs
