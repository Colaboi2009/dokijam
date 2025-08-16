#include "ecs.hpp"
#include "engine/raw_text.hpp"
#include "engine/sdl_wrapper.hpp"

#include "component.hpp"

namespace ecs {

void render(entt::registry& registry, SDL& sdl) {
	// Camera
	entt::entity cameraEntity = registry.view<Camera>().front();
	Position cameraPosition = registry.get<Position>(cameraEntity);

	auto positions = registry.view<Position>();
	for (auto [e, pos] : positions.each()) {
		pos.x -= cameraPosition.x - sdl.getWindowSize().x / 2.f;
		pos.y -= cameraPosition.y - sdl.getWindowSize().y / 2.f;
	}

	auto tileMaps = registry.view<Position, TileMapSprite>();
	for (auto [e, pos, tilemap] : tileMaps.each()) {
        tilemap.tilemap->render(Point{pos.x, pos.y}, tilemap.scale);
	}

    auto shapeRenderables = registry.view<Position, Rectangle>();
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

    auto spriteRenderables = registry.view<Position, Sprite>();
	std::vector<std::tuple<entt::entity, Position, Sprite>> entities(spriteRenderables.each().begin(), spriteRenderables.each().end());
	std::sort(entities.begin(), entities.end(), [](const auto &l, const auto &r) {
		return std::get<Position>(l).z < std::get<Position>(r).z;
	});
    for (auto [e, position, sprite] : entities) {
        sprite.animation->render(position.x, position.y, sprite.scale);
    }


//	auto boxColliders = registry.view<Position, BoxCollider>();
//	sdl.setColor({255, 0, 255, 255});
//	for (auto [e, pos, box] : boxColliders.each()) {
//		sdl.drawRect({pos.x + box.xOffset, pos.y + box.yOffset, box.width, box.height});
//	}
//	auto circleColliders = registry.view<Position, CircleCollider>();
//	sdl.setColor({255, 0, 255, 255});
//	for (auto [e, pos, cir] : circleColliders.each()) {
//		sdl.drawCircle(cir.radius, pos.x + cir.xOffset, pos.y + cir.yOffset);
//	}

	auto rawText = registry.view<Position, RawText>();
	for (auto [e, pos, text] : rawText.each()) {
		text.render({pos.x, pos.y});
	}

	// Bring positions back to world space
	for (auto [e, pos] : positions.each()) {
		//sdl.drawCircle(10, pos.x, pos.y);
		pos.x += cameraPosition.x - sdl.getWindowSize().x / 2.f;
		pos.y += cameraPosition.y - sdl.getWindowSize().y / 2.f;
	}
}

}
