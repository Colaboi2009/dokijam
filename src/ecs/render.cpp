#include "ecs.hpp"
#include "engine/sdl_wrapper.hpp"

namespace ecs {

void render(entt::registry& registry, SDL& sdl) {
    // TODO: Might need to handle render order
    auto spriteRenderables = registry.view<ecs::Position, ecs::Sprite>();
    for (auto [e, position, sprite] : spriteRenderables.each()) {
        sprite.animation->render(position.x, position.y, sprite.scale);
    }

    // Debug stuff will always be on top
    auto shapeRenderables = registry.view<ecs::Position, ecs::Rectangle>();
    for (auto [e, position, shape] : shapeRenderables.each()) {
        SDL_FRect dstRect = {
            position.x, position.y,
            shape.width, shape.height
        };

        sdl.setColor(shape.outlineColor);
        sdl.drawRect(dstRect);
        sdl.setColor(shape.fillColor);
        sdl.drawRectFilled(dstRect);
    }

	auto tileMaps = registry.view<ecs::Position, ecs::TileMapSprite>();
	for (auto [e, pos, tilemap] : tileMaps.each()) {
        tilemap.tilemap->render(Point{pos.x, pos.y}, 4.0f);
	}
}

}
