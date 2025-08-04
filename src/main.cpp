#include "engine/sdl_wrapper.hpp"

SDL sdl;

int main() {
    while (true) {
        sdl.clear();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
				goto EXIT;
            }
        }

        sdl.present();
    }

EXIT:
	return 0;
}
