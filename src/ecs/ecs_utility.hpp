#pragma once

#include "SDL3/SDL_rect.h"
#include "ecs/component.hpp"
#include "entt/entity/fwd.hpp"

namespace ecs {
// note: this is the slowest one, if you know the collider type ahead of time, or have veiws, use those
SDL_FPoint getColliderCenter(entt::registry &r, entt::entity e, Position originalPos);

SDL_FPoint getColliderCenter(entt::entity e, Position originalPos, BoxCollider *boxView, CircleCollider *circleView);

// must be defined here because of "auto"
SDL_FPoint getColliderCenter(entt::entity e, Position originalPos, auto &boxView, auto &circleView) {
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

SDL_FPoint getMousePosition(entt::registry &r);
}
