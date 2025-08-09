#pragma once

#include "engine/animation.hpp"
#include "engine/sdl_wrapper.hpp"
#include "entt/entity/fwd.hpp"
#include "tilemap.hpp"
#include "utility.hpp"

namespace ecs {

namespace detail {
    struct Collider {
        float xOffset;
        float yOffset;

		entt::entity hit;
    };
}

// not really needed rn
struct Movable {};
struct Renderable {}; // anything that has a sprite or shape is renderable anyways

struct Sprite {
    float scale;
    std::shared_ptr<Animation> animation;

    Sprite(std::shared_ptr<Animation> &animation, float scale = 1.0f)
        : animation(animation), scale(scale) {}
};

struct Rectangle {
    SDL_Color fillColor;
    SDL_Color outlineColor;

    float width = 0.0f;
    float height = 0.0f;

    Rectangle(float width, float height, const SDL_Color &color)
        : Rectangle(width, height, color, color) {}
    Rectangle(float width, float height, const SDL_Color &fillColor, const SDL_Color &outlineColor)
        : width(width), height(height), fillColor(fillColor), outlineColor(outlineColor) {}
};

struct Position {
    float x = 0.0f;
    float y = 0.0f;
    /*
        Layer,
        we can also use this to skip collision detection,
        when the entities are on different layers,
        or skip checking some layers (like ground) completely,
        etc.
    */
    float z = 0.0f;

    Position() = default;
    Position(const float x, const float y, const float z = 0.0f) : x(x), y(y), z(z) {}
};

struct Velocity {
    float dx = 0.0f;
    float dy = 0.0f;

    Velocity() = default;
};

struct Health {
    int current;
    int maximum;

    Health(const int maximum) : current(maximum), maximum(maximum) {}
};

struct BoxCollider : public detail::Collider {
    float width;
    float height;

    // Bounds offset or scale
    // note(cola): scale is for the sprite not the box collider,
    // the offset being applied to the collider could make the collision code messier, but if its centralized to only that place might work

    BoxCollider(const float x, const float y, const float w, const float h)
        : detail::Collider({x, y}), width(w), height(h) {}
};

struct CircleCollider : public detail::Collider {
    float radius;

    CircleCollider(const float x, const float y, const float r)
        : detail::Collider({x, y}), radius(r) {}
};

struct TileMapCollider : public detail::Collider {
};

struct TileMapSprite {
    float scale;
	SP<TileMap> tilemap;

	TileMapSprite(SP<TileMap> tm, const float scale)
        : tilemap(tm), scale(scale) {}
};

struct AttackTarget {};

struct Spawner {
    enum class Type {
        Dragoon
    };

    Type type;

    // Direction of the spawned entity
    // in case of a dragoon will probably be set to mouse cursor?
    // in that case this vector should be normalized,
    // otherwise we can use this as spawn offset?
    float x = 0.0f;
    float y = 0.0f;

    // Set this to true if you want to spawn entities in the current tick
    bool shouldSpawn = false;
    // We can also have like a spawn count per tick/update
    //int spawnGroupCount;
    // for spawning like a group of enemies

    // can also have a magazine property, that would contain the total entities
    // to be spawned (until recharged by a different system)
    //int magazine;

    Spawner(Type type)
        : type(type) {}
};

// Explosion, can be used by the cleanup system to trigger an explosion when an entity dies
struct Explosion {
    float radius;

    // maybe a type = Enemy vs Player,
    // so that enemy explosion don't hurt them?

    // might be fun if enemies can kill each other tho

    // wheter an explosion should be triggered in the current tick
    bool shouldTrigger;
};

// Remove the entity after msToLive reaches zero
struct JustDieAfter {
    int64_t msToLive;

    JustDieAfter(int64_t msToLive)
        : msToLive(msToLive) {}
};

} // namespace ecs
