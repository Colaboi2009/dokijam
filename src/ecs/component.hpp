#pragma once

#include "engine/animation.hpp"
#include "engine/sdl_wrapper.hpp"

namespace ecs {

// not really needed rn
struct Movable {};
struct Renderable {}; // anything that has a sprite or shape is renderable anyways

struct Sprite {
    std::shared_ptr<Animation> animation;

    Sprite(std::shared_ptr<Animation> &animation) : animation(animation) {}
};
struct Shape {
    SDL_Color fillColor;
    SDL_Color outlineColor;

    // ShapeType = Rectangle, Circle?

    Shape(const SDL_Color &color) : fillColor(color), outlineColor(color) {}
    Shape(const SDL_Color &fillColor, SDL_Color &outlineColor) : fillColor(fillColor), outlineColor(outlineColor) {}
};

struct Transform {
    SDL_FRect box = { 0.0f, 0.0f, 0.0f, 0.0f };

	// bottom right
    const SDL_FPoint botr() const { return {box.x + box.w, box.y + box.h}; }

    Transform() = default;
    Transform(const SDL_FRect& box) : box(box) {}
    Transform(const float x, const float y, const float w, const float h) : box({x, y, w, h}) {}
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

struct BoxCollider {
    // Bounds offset or scale
    // note(cola): scale is for the sprite not the box collider,
    // the offset being applied to the collider could make the collision code messier, but if its centralized to only that place might work

    BoxCollider() {}
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
    uint64_t createdAt;
    uint64_t msToLive;

    JustDieAfter(uint64_t now, uint64_t msToLive)
        : createdAt(now), msToLive(msToLive) {}
};

} // namespace ecs
