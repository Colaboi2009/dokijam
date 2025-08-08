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

static void handleBBCollisions(entt::registry &reg, entt::entity e1, Position &e1pos, BoxCollider &e1box, entt::entity e2, Position &e2pos, BoxCollider &e2box, const double dt) {
    SDL_FRect e1rect = {e1pos.x + e1box.xOffset, e1pos.y + e1box.yOffset, e1box.width, e1box.height};
    SDL_FRect e2rect = {e2pos.x + e2box.xOffset, e2pos.y + e2box.yOffset, e2box.width, e2box.height};
	
	SDL_FRect e1expanded = {e1rect.x - e2rect.w / 2, e1rect.y - e2rect.h / 2, e1rect.w + e2rect.w, e1rect.h + e2rect.h};
	SDL_FRect e2expanded = {e2rect.x - e1rect.w / 2, e2rect.y - e1rect.h / 2, e2rect.w + e1rect.w, e2rect.h + e1rect.h};

	if (!rectsOverlap(e1expanded, e2expanded)) {
		return;
	}

    if (isMovable(reg, e1) && !isMovable(reg, e2)) { // !movable vs movable is handled on the duplicate route
        Velocity &e1vel = reg.get<Velocity>(e1);

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
            informCollision(e1, e1box, e2, e2box);
        }
    } else if (isMovable(reg, e1) && isMovable(reg, e2)) { // which means yes, this runs twice, but thats fineeee
        Velocity &e1vel = reg.get<Velocity>(e1);
        Velocity &e2vel = reg.get<Velocity>(e2);
    }
}

static void handleBCCollisions(entt::registry &reg, entt::entity eb, Position &ebpos, BoxCollider &ebbox, entt::entity ec, Position &ecposcomp, CircleCollider &ecc, const double dt) {
    SDL_FRect ebrect = {ebpos.x + ebbox.xOffset, ebpos.y + ebbox.yOffset, ebbox.width, ebbox.height};
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

static void handleCCCollisions(entt::registry &reg, entt::entity e1, Position &e1poscomp, CircleCollider &e1cir, entt::entity e2, Position &e2poscomp, CircleCollider &e2cir, const double dt) {
    Point e1pos = {e1poscomp.x + e1cir.xOffset, e1poscomp.y + e1cir.yOffset};
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

void collision(entt::registry &registry, const double dt) {
    auto boxColliders = registry.view<BoxCollider>();
    for (auto [e, col] : boxColliders.each()) {
        col.hit = entt::null;
    }
    auto circleColliders = registry.view<CircleCollider>();
    for (auto [e, col] : circleColliders.each()) {
        col.hit = entt::null;
    }

    auto boxView = registry.view<Position, BoxCollider>();
	auto circleView = registry.view<Position, CircleCollider>();

	for (auto [e1, e1pos, e1box] : boxView.each()) {
		for (auto [e2, e2pos, e2box] : boxView.each()) {
			if (e1 == e2) {
				continue;
			}
			handleBBCollisions(registry, e1, e1pos, e1box, e2, e2pos, e2box, dt);
		}
	}
	for (auto [eb, ebpos, ebbox] : boxView.each()) {
		for (auto [ec, ecpos, eccir] : circleView.each()) {
			// just incase
			if (eb == ec) {
				continue;
			}
			handleBCCollisions(registry, eb, ebpos, ebbox, ec, ecpos, eccir, dt);
		}
	}
	for (auto [e1, e1pos, e1cir] : circleView.each()) {
		for (auto [e2, e2pos, e2cir] : circleView.each()) {
			if (e1 == e2) {
				continue;
			}
			handleCCCollisions(registry, e1, e1pos, e1cir, e2, e2pos, e2cir, dt);
		}
	}
}

} // namespace ecs
