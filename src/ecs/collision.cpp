#include "ecs.hpp"
#include "utility.hpp"
/*
static void resolveCollision(entt::registry &registry, entt::entity &e1, entt::entity &e2) {
    ecs::Position &transL = registry.get<ecs::Position>(e1);
    ecs::Position &transR = registry.get<ecs::Position>(e2);

	// there could be immovables with velocity, e.g. a moving platform (you're not really supposed to be able to move that yourself)
    bool isLMovable = registry.any_of<ecs::Velocity>(e1);
    bool isRMovable = registry.any_of<ecs::Velocity>(e2);

    // TODO if needed in gameplay: Implement movable & movable (e.g. enemy collisions, pushing mechs), immovable & immovable (probably do
    // nothing in this scenario, its likely map edge scenary overlaping) collisions

    // it looks really ugly and messy to use transA and transB everywhere, so aliasing (temp comment)
    SDL_FRect l = transL.box;
    SDL_FRect r = transR.box;
    SDL_FPoint lbotr = transL.botr();
    SDL_FPoint rbotr = transR.botr();

    if (std::abs(r.x - lbotr.x) < std::abs(rbotr.y - l.y) && std::abs(r.x - lbotr.x) < std::abs(r.y - lbotr.y)) {
        if (isLMovable && !isRMovable) {
            // keeping sets verbose (temp comment)
            transL.box.x = r.x - l.w;
        } else if (!isLMovable && isRMovable) {
            transR.box.x = l.x + l.w;
        }
    } else {
        if (isLMovable && !isRMovable) {
            if (std::abs(rbotr.y - l.y) < std::abs(r.y - lbotr.y)) {
                transL.box.y = rbotr.y;
            } else {
                transL.box.y = r.y - l.h;
            }
        } else if (!isLMovable && isRMovable) {
            if (std::abs(rbotr.y - l.y) < std::abs(r.y - lbotr.y)) {
                transR.box.y = l.y - r.h;
            } else {
                transR.box.y = lbotr.y;
            }
        }
    }
}*/

namespace ecs {

void collision(entt::registry &registry) {
    auto view = registry.view<ecs::Position, ecs::BoxCollider>();
    /*for (auto [e1, position, collider] : view.each()) {
        for (auto [e2, position, collider] : view.each()) {
            if (e1 == e2)
                continue;
            if (rectsOverlap(position.box, position.box)) {
                if (transformA.box.x + transformA.box.w / 2.f > transformB.box.x + transformB.box.w / 2.f) {
                    resolveCollision(registry, e2, e1);
                } else {
                    resolveCollision(registry, e1, e2);
                }
            }
        }
    }*/
}

}
