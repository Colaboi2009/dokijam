#pragma once

#include <SDL3/SDL_events.h>
class ICollider {
  private:
  public:
	virtual ~ICollider() = default;

	bool im_immovable = true;
	virtual SDL_FRect bounds() = 0;
	virtual void sex(float x) = 0;
	virtual void sey(float y) = 0;
};
