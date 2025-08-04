#include "engine/sdl_wrapper.hpp"

#include "i_collider.hpp"
#include "i_element.hpp"
#include "platform.hpp"
#include "player.hpp"
#include "utility.hpp"

SDL sdl;

std::vector<SP<IElement>> ielements;
std::vector<SP<ICollider>> icolliders;

int main() {
    SP<Platform> plat = std::make_shared<Platform>(SDL_FRect{300, 800, 80, 30}, SDL_Color{255, 255, 255, 255});
    ielements.push_back(plat);
    icolliders.push_back(plat);

    SP<Player> player = std::make_shared<Player>();
    ielements.push_back(player);
    icolliders.push_back(player);

    while (true) {
        sdl.clear({40, 40, 40, 255});

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                goto EXIT;
            }
            for (const auto &el : ielements) {
                el->event(e);
            }
        }

        for (const auto &el : ielements) {
            el->prerender(0.1f);
        }

        for (const auto &el : ielements) {
            el->render();
        }

        for (auto &coll : icolliders) {
            for (auto &colr : icolliders) {
                if (coll == colr) {
                    continue;
                }
                sdl.setColor({0, 255, 0, 255});

                if (rectsOverlap(coll->bounds(), colr->bounds())) {
                    if (center(coll->bounds()).x < center(colr->bounds()).x) {
						std::swap(coll, colr);
                    }
                    SDL_FRect l = coll->bounds();
                    SDL_FRect r = colr->bounds();

                    SDL_FPoint ltopl = {l.x, l.y};
                    SDL_FPoint lbotr = {l.x + l.w, l.y + l.h};
                    SDL_FPoint lcen = {center(l).x, center(l).y};

                    SDL_FPoint rtopl = {r.x, r.y};
                    SDL_FPoint rbotr = {r.x + r.w, r.y + r.h};
                    SDL_FPoint rcen = {center(r).x, center(r).y};

					if (!coll->im_immovable && colr->im_immovable) {
						colr->sex(lbotr.x);
					} else if (coll->im_immovable && !colr->im_immovable) {
						coll->sex(rtopl.x - l.w);
					}
                }

                sdl.drawRect(coll->bounds());
                sdl.drawRect(colr->bounds());
            }
        }

        sdl.present();
    }

EXIT:
    return 0;
}
