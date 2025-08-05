#include "engine/sdl_wrapper.hpp"

#include "i_collider.hpp"
#include "i_element.hpp"
#include "platform.hpp"
#include "player.hpp"
#include "utility.hpp"

#include <entt/entt.hpp>

#include "component.hpp"

SDL sdl;

std::vector<SP<IElement>> ielements;
std::vector<SP<ICollider>> icolliders;

void handleInput(entt::registry &registry, const entt::entity &player) {
    auto &velocity = registry.get<ecs::Velocity>(player);

    velocity.dx = 0;
    velocity.dy = 0;

    float speed = 4;
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D]) {
        velocity.dy += speed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_E]) {
        velocity.dy -= speed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_F]) {
        velocity.dx += speed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S]) {
        velocity.dx -= speed;
    }
}

void handleMovementSystem(entt::registry &registry) {
    auto view = registry.view<ecs::Transform, ecs::Velocity>();
    for (auto [e, position, velocity] : view.each()) {
        position.box.x += velocity.dx;
        position.box.y += velocity.dy;
    }
}

void resolveCollision(entt::registry &registry, entt::entity &e1, entt::entity &e2) {
    ecs::Transform &transL = registry.get<ecs::Transform>(e1);
    ecs::Transform &transR = registry.get<ecs::Transform>(e2);

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
}

void handleCollisionSystem(entt::registry &registry) {
    auto view = registry.view<ecs::Transform, ecs::BoxCollider>();
    for (auto [e1, transformA] : view.each()) {
        for (auto [e2, transformB] : view.each()) {
            if (e1 == e2)
                continue;
            if (rectsOverlap(transformA.box, transformB.box)) {
                if (transformA.box.x + transformA.box.w / 2.f > transformB.box.x + transformB.box.w / 2.f) {
                    resolveCollision(registry, e2, e1);
                } else {
                    resolveCollision(registry, e1, e2);
                }
            }
        }
    }
}

int main() {

    /*
        THIS IS KINDA STUPID AS I DON'T HAVE THE BRAINS
        TO FIGURE IT OUT AT THIS UNHOLY HOUR
    */

    entt::registry registry;

    SP<Animation> animation = std::make_shared<Animation>("tester.aseprite");

    // We can use ecs::Movable, or we can decide based on ecs::Velocity,
    // if the entity can be moved

    const entt::entity player = registry.create();
    registry.emplace<ecs::Transform>(player, 800, 500, 0, 0);
    registry.emplace<ecs::Velocity>(player);
    registry.emplace<ecs::BoxCollider>(player);
    registry.emplace<ecs::Sprite>(player, animation);

    const entt::entity platform = registry.create();
    registry.emplace<ecs::Transform>(platform, 300, 800, 80, 30);
    registry.emplace<ecs::Velocity>(platform);
    registry.emplace<ecs::BoxCollider>(platform);
    registry.emplace<ecs::Shape>(platform, SDL_Color{255, 255, 255, 255});

    const entt::entity YOU_CANT_MOVE_ME = registry.create();
    registry.emplace<ecs::Transform>(YOU_CANT_MOVE_ME, 500, 600, 120, 30);
    registry.emplace<ecs::BoxCollider>(YOU_CANT_MOVE_ME);
    registry.emplace<ecs::Shape>(YOU_CANT_MOVE_ME, SDL_Color{0, 0, 0, 255});

    while (true) {
        sdl.clear({40, 40, 40, 255});

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                goto EXIT;
            }
        }

        handleInput(registry, player);
        handleCollisionSystem(registry);
        handleMovementSystem(registry);

        // TODO: Might need to handle render order
        auto spriteRenderables = registry.view<ecs::Transform, ecs::Sprite>();
        for (auto [e, transform, sprite] : spriteRenderables.each()) {
            sprite.animation->render(Point{transform.box.x, transform.box.y});
        }

        auto shapeRenderables = registry.view<ecs::Transform, ecs::Shape>();
        for (auto [e, transform, shape] : shapeRenderables.each()) {
            sdl.setColor(shape.outlineColor);
            sdl.drawRect(transform.box);
            sdl.setColor(shape.fillColor);
            sdl.drawRectFilled(transform.box);
        }

        sdl.present();
    }

EXIT:
    return 0;
}
