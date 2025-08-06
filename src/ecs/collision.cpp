#include "ecs.hpp"
#include "globals.hpp"
#include "utility.hpp"

// NOTE: I should probably rename the variables to not have '_' due to naming conventions, but it looks DAMN clean when you do math with
// them, though function names would look shit with them

namespace ecs {
// added this to change the reason why something is movable; a moving platform probably shouldn't be moved by the player
static bool isMovable(entt::registry &reg, entt::entity e) { return reg.any_of<Velocity>(e); }

static bool rayVsRect(const Point &ray_origin, const Point &ray_dir, const SDL_FRect &target, SDL_FPoint &contact_point,
                      SDL_FPoint &contact_normal, float &t_hit_near) {
    Point targetPos = {target.x, target.y};
    Point targetSize = {target.w, target.h};

    Point t_near = (targetPos - ray_origin) / ray_dir;
    Point t_far = (targetPos + targetSize - ray_origin) / ray_dir;
    if (std::isnan(t_near.x) || std::isnan(t_near.y) || std::isnan(t_far.x) || std::isnan(t_far.y))
        return false;

    if (t_near.x > t_far.x)
        std::swap(t_near.x, t_far.x);
    if (t_near.y > t_far.y)
        std::swap(t_near.y, t_far.y);

    if (t_near.x > t_far.y || t_near.y > t_far.x) {
        return false;
    }

    t_hit_near = std::max(t_near.x, t_near.y);
    float t_hit_far = std::min(t_far.x, t_far.y);

    if (t_hit_far < 0) {
        return false;
    }
    if (std::isinf(t_hit_near)) {
        return false;
    }

    contact_point = ray_origin + Point{ray_dir.x * t_hit_near, t_hit_near * ray_dir.y};

    SDL_FPoint far_contact = ray_origin + Point{ray_dir.x * t_hit_far, t_hit_far * ray_dir.y};

    if (t_near.x > t_near.y) {
        if (ray_dir.x < 0) {
            contact_normal = {1, 0};
        } else {
            contact_normal = {-1, 0};
        }
    } else if (t_near.x < t_near.y) {
        if (ray_dir.y < 0) {
            contact_normal = {0, 1};
        } else {
            contact_normal = {0, -1};
        }
    }

    return true;
}

static void handleBBCollisions(entt::registry &reg, entt::entity e1, entt::entity e2, const double dt) {
    Position &e1pos = reg.get<Position>(e1);
    BoxCollider &e1box = reg.get<BoxCollider>(e1);
    SDL_FRect e1rect = {e1pos.x + e1box.xOffset, e1pos.y + e1box.yOffset, e1box.width, e1box.height};

    Position &e2pos = reg.get<Position>(e2);
    BoxCollider &e2box = reg.get<BoxCollider>(e2);
    SDL_FRect e2rect = {e2pos.x + e2box.xOffset, e2pos.y + e2box.yOffset, e2box.width, e2box.height};

    if (isMovable(reg, e1) && !isMovable(reg, e2)) { // !movable vs movable is handled on the duplicate route
        Velocity &e1vel = reg.get<Velocity>(e1);
        SDL_FRect e2expanded = {e2rect.x - e1rect.w / 2, e2rect.y - e1rect.h / 2, e2rect.w + e1rect.w, e2rect.h + e1rect.h};

        if (e1vel.dx == 0 && e1vel.dy == 0) {
            return;
        }

        SDL_FPoint contact_pos;
        SDL_FPoint normal;
        float contact_time;
        if (rayVsRect({e1rect.x + e1rect.w / 2.f, e1rect.y + e1rect.h / 2.f}, {e1vel.dx * (float)dt, e1vel.dy * (float)dt}, e2expanded, contact_pos,
                      normal, contact_time) && contact_time < 1.0f && contact_time >= 0.f) {
            e1vel.dx += normal.x * std::abs(e1vel.dx) * (1 - contact_time);
            e1vel.dy += normal.y * std::abs(e1vel.dy) * (1 - contact_time);
        }
    } else if (isMovable(reg, e1) && isMovable(reg, e2)) { // which means yes, this runs twice, but thats fineeee
        Velocity &e1vel = reg.get<Velocity>(e1);
        Velocity &e2vel = reg.get<Velocity>(e2);
    }
}

static void handleBCCollisions(entt::registry &reg, entt::entity eb, entt::entity ec) {}

static void handleCCCollisions(entt::registry &reg, entt::entity e1, entt::entity e2) {}

void collision(entt::registry &registry, const double dt) {
    auto view = registry.view<Position>();
    for (auto [e1, pos1] : view.each()) {
        if (registry.any_of<BoxCollider, CircleCollider>(e1)) {
            for (auto [e2, pos2] : view.each()) {
                if (e1 == e2)
                    continue;
                if (registry.any_of<BoxCollider, CircleCollider>(e2)) {
                    if (registry.any_of<BoxCollider>(e1) && registry.any_of<BoxCollider>(e2)) {
                        handleBBCollisions(registry, e1, e2, dt);
                    } else if (registry.any_of<BoxCollider>(e1) && registry.any_of<CircleCollider>(e2)) {
                        handleBCCollisions(registry, e1, e2);
                    } else if (registry.any_of<CircleCollider>(e1) && registry.any_of<CircleCollider>(e2)) {
                        handleCCCollisions(registry, e1, e2);
                    }
                }
            }
        }
    }
}

} // namespace ecs
