#pragma once

#include <SDL3/SDL_events.h>
class IElement {
  private:
  public:
	virtual ~IElement() = default;

	virtual void event(SDL_Event &e) = 0;
	virtual void prerender(float dt) = 0;
	virtual void render() = 0;
};
