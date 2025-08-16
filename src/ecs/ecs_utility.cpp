#include "ecs_utility.hpp"
#include "globals.hpp"

namespace ecs {

SDL_FPoint getColliderCenter(entt::registry &r, entt::entity e, Position originalPos) {
    SDL_FPoint center = {originalPos.x, originalPos.y};
    if (auto box = r.try_get<BoxCollider>(e)) {
        center.x += box->xOffset + box->width / 2.f;
        center.y += box->yOffset + box->height / 2.f;
    } else if (auto cir = r.try_get<BoxCollider>(e)) {
        center.x += cir->xOffset;
        center.y += cir->yOffset;
    }
    return center;
}

SDL_FPoint getColliderCenter(entt::entity e, ecs::Position originalPos, BoxCollider *box, CircleCollider *cir) {
    SDL_FPoint center = {originalPos.x, originalPos.y};
    if (box) {
        center.x += box->xOffset + box->width / 2.f;
        center.y += box->yOffset + box->height / 2.f;
    } else if (cir) {
        center.x += cir->xOffset;
        center.y += cir->yOffset;
    }
    return center;
}

entt::entity copy(entt::registry &r, const entt::entity toCopy) {
	entt::entity copy = r.create();
	for (auto [e, storage] : r.storage()) {
		if (storage.contains(toCopy)) {
			storage.push(copy, storage.value(toCopy));
		}
	}
	return copy;
}

SDL_FPoint getMousePosition(entt::registry &r) {
	float mouseX = 0.0f;
	float mouseY = 0.0f;
	SDL_GetMouseState(&mouseX, &mouseY);

	// convert from window to world space
	entt::entity cameraEntity = r.view<Camera>().front();
	Position cameraPosition = r.get<Position>(cameraEntity);
	mouseX += cameraPosition.x - sdl.getWindowSize().x / 2.f;
	mouseY += cameraPosition.y - sdl.getWindowSize().y / 2.f;

	return {mouseX, mouseY};
}

} // namespace ecs
