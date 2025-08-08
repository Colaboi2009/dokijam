#include "ecs.hpp"
#include "globals.hpp"
#include "utility.hpp"
#include <print>

// NOTE: I should probably rename the variables to not have '_' due to naming conventions, but it looks DAMN clean when you do math with
// them, though function names would look shit with them

namespace ecs {
// added this to change the reason why something is movable; a moving platform probably shouldn't be moved by the player
static bool isMovable(entt::registry &reg, entt::entity e) { return reg.any_of<Velocity>(e); }

static void informCollision(entt::entity e1, detail::Collider c1, entt::entity e2, detail::Collider c2) {
    c1.hit = e2;
    c2.hit = e1;
}

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

    if (!rectsOverlap(e1rect, e2rect)) {
        return;
    }
    informCollision(e1, e1box, e2, e2box);

    if (isMovable(reg, e1) && !isMovable(reg, e2)) { // !movable vs movable is handled on the duplicate route
        Velocity &e1vel = reg.get<Velocity>(e1);
        SDL_FRect e2expanded = {e2rect.x - e1rect.w / 2, e2rect.y - e1rect.h / 2, e2rect.w + e1rect.w, e2rect.h + e1rect.h};

        if (e1vel.dx == 0 && e1vel.dy == 0) {
            return;
        }

        SDL_FPoint contact_pos;
        SDL_FPoint normal;
        float contact_time;
        if (rayVsRect({e1rect.x + e1rect.w / 2.f, e1rect.y + e1rect.h / 2.f}, {e1vel.dx * (float)dt, e1vel.dy * (float)dt}, e2expanded,
                      contact_pos, normal, contact_time) &&
            contact_time < 1.0f && contact_time >= 0.f) {
            e1vel.dx += normal.x * std::abs(e1vel.dx) * (1 - contact_time);
            e1vel.dy += normal.y * std::abs(e1vel.dy) * (1 - contact_time);
        }
    } else if (isMovable(reg, e1) && isMovable(reg, e2)) { // which means yes, this runs twice, but thats fineeee
        Velocity &e1vel = reg.get<Velocity>(e1);
        Velocity &e2vel = reg.get<Velocity>(e2);
    }
}

static void handleBCCollisions(entt::registry &reg, entt::entity eb, entt::entity ec, const double dt) {
    Position &ebpos = reg.get<Position>(eb);
    BoxCollider &ebbox = reg.get<BoxCollider>(eb);
    SDL_FRect ebrect = {ebpos.x + ebbox.xOffset, ebpos.y + ebbox.yOffset, ebbox.width, ebbox.height};

    Position &ecposcomp = reg.get<Position>(ec);
    CircleCollider &ecc = reg.get<CircleCollider>(ec);
    SDL_FPoint ecpos = {ecposcomp.x + ecc.xOffset, ecposcomp.y + ecc.yOffset};

    if (isMovable(reg, ec) && !isMovable(reg, eb)) {
        Velocity &ecvel = reg.get<Velocity>(ec);
        SDL_FRect ebexpanded = {ebrect.x - ecc.radius, ebrect.y - ecc.radius, ebrect.w + ecc.radius, ebrect.h + ecc.radius};

        if (ecvel.dx == 0 && ecvel.dy == 0) {
            return;
        }

        SDL_FPoint contact_pos;
        SDL_FPoint normal;
        float contact_time;
        if (rayVsRect(ecpos, {ecvel.dx * (float)dt, ecvel.dy * (float)dt}, ebexpanded, contact_pos, normal, contact_time) &&
            contact_time < 1.0f && contact_time >= 0.f) {
            informCollision(eb, ebbox, ec, ecc);

            ecvel.dx += normal.x * std::abs(ecvel.dx) * (1 - contact_time);
            ecvel.dy += normal.y * std::abs(ecvel.dy) * (1 - contact_time);
        }
    } else if (!isMovable(reg, ec) && isMovable(reg, eb)) {
        Velocity &ebvel = reg.get<Velocity>(eb);
    } else if (isMovable(reg, ec) && isMovable(reg, eb)) {
        Velocity &ecvel = reg.get<Velocity>(ec);
        Velocity &ebvel = reg.get<Velocity>(eb);
    }
}

static void handleCCCollisions(entt::registry &reg, entt::entity e1, entt::entity e2, const double dt) {
    Position &e1poscomp = reg.get<Position>(e1);
    CircleCollider &e1cir = reg.get<CircleCollider>(e1);
    Point e1pos = {e1poscomp.x + e1cir.xOffset, e1poscomp.y + e1cir.yOffset};

    Position &e2poscomp = reg.get<Position>(e2);
    CircleCollider &e2cir = reg.get<CircleCollider>(e2);
    Point e2pos = {e2poscomp.x + e2cir.xOffset, e2poscomp.y + e2cir.yOffset};

    float sqr_dist = sqrDist(e1pos, e2pos);
    float radius_sum = e1cir.radius + e2cir.radius;

    if (sqr_dist > radius_sum * radius_sum) {
        return;
    }

    informCollision(e1, e1cir, e2, e2cir);

    Point dir = e2pos - e1pos;
    float length = sqrt(dir.x * dir.x + dir.y * dir.y);
    dir.x /= length;
    dir.y /= length;

    float error = radius_sum - sqrt(sqr_dist);

    if (isMovable(reg, e1) && !isMovable(reg, e2)) {
        Velocity e1vel = reg.get<Velocity>(e1);

        e1poscomp.x -= dir.x * error;
        e1poscomp.y -= dir.y * error;
    }
}

// may change it later if it become unperformant, but for now, each tile is basically an immovable square
static void handleTCCollisions(entt::registry &reg, entt::entity et, entt::entity ec, const double dt) {
    Position position = reg.get<Position>(et);
    TileMap::LevelRef ref = reg.get<TileMapSprite>(et).tilemap->getLevel("def_col");
    auto c = ref.collisionLayer;
    // note(cola): im not sure how else to check the width and height of the tiles
    int width = 16;
    int height = 16;

    // HARD CODED SCALE, NEED TO ADD A MEMBER TO TILEMAP
    const int32_t w = width * 4.0;
    const int32_t h = height * 4.0;

    SDL_FRect dst = {position.x, position.y, (float)w, (float)h};
    std::size_t i = 0;
    for (std::size_t y = 0; y < c->height; y++) {
        dst.x = position.x;
        for (std::size_t x = 0; x < c->width; x++) {
            uint32_t tileIndex = c->tiles[i++];
            if (tileIndex != 0) {
                entt::entity etempTile = reg.create();
                reg.emplace<Position>(etempTile, dst.x, dst.y);
                reg.emplace<BoxCollider>(etempTile, 0, 0, w, h);
                handleBCCollisions(reg, etempTile, ec, dt);
				sdl.setColor({255, 255, 255, 255});
                sdl.drawRect({dst.x, dst.y, (float)w, (float)h});
                reg.destroy(etempTile);
            }
            dst.x += w;
        }
        dst.y += h;
    }
}

static void handleTBCollisions(entt::registry &reg, entt::entity et, entt::entity eb) {}

void collision(entt::registry &registry, const double dt) {
    auto boxColliders = registry.view<BoxCollider>();
    for (auto [e, col] : boxColliders.each()) {
        col.hit = entt::null;
    }
    auto circleColliders = registry.view<CircleCollider>();
    for (auto [e, col] : circleColliders.each()) {
        col.hit = entt::null;
    }

    auto view = registry.view<Position>();
    for (auto [e1, pos1] : view.each()) {
        if (registry.any_of<BoxCollider, CircleCollider, TileMapCollider>(e1)) {
            for (auto [e2, pos2] : view.each()) {
                if (e1 == e2)
                    continue;
                if (registry.any_of<BoxCollider, CircleCollider>(e2)) { // don't care about tilemap vs tilemap
                    if (registry.any_of<BoxCollider>(e1) && registry.any_of<BoxCollider>(e2)) {
                        handleBBCollisions(registry, e1, e2, dt);
                    } else if (registry.any_of<BoxCollider>(e1) && registry.any_of<CircleCollider>(e2)) {
                        handleBCCollisions(registry, e1, e2, dt);
                    } else if (registry.any_of<CircleCollider>(e1) && registry.any_of<CircleCollider>(e2)) {
                        handleCCCollisions(registry, e1, e2, dt);
                    } else if (registry.all_of<TileMapCollider, TileMapSprite>(e1) && registry.any_of<CircleCollider>(e2)) {
                        handleTCCollisions(registry, e1, e2, dt);
                    } else if (registry.all_of<TileMapCollider, TileMapSprite>(e1) && registry.any_of<BoxCollider>(e2)) {
                        handleTBCollisions(registry, e1, e2);
                    }
                }
            }
        }
    }
}

} // namespace ecs
