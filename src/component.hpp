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
    SDL_FRect box;

    // not sure if you are supposed to put helper functions inside a struct, but its damn convenient, and prevents math mistakes. Maybe in
    // utility?
	// bottom right
    const SDL_FPoint botr() const { return {box.x + box.w, box.y + box.h}; }

    Transform() = default;
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

} // namespace ecs
