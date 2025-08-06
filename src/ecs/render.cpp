#include "ecs.hpp"
#include "engine/sdl_wrapper.hpp"

namespace ecs {

void render(entt::registry& registry, SDL& sdl) {
    // TODO: Might need to handle render order
    auto spriteRenderables = registry.view<ecs::Transform, ecs::Sprite>();
    for (auto [e, transform, sprite] : spriteRenderables.each()) {
        sprite.animation->render(transform.box);
    }

    auto shapeRenderables = registry.view<ecs::Transform, ecs::Shape>();
    for (auto [e, transform, shape] : shapeRenderables.each()) {
        sdl.setColor(shape.outlineColor);
        sdl.drawRect(transform.box);
        sdl.setColor(shape.fillColor);
        sdl.drawRectFilled(transform.box);
    }
}

}
