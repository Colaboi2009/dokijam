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

        for (auto &colA : icolliders) {
            for (auto &colB : icolliders) {
                if (colA == colB) {
                    continue;
                }
                sdl.setColor({0, 255, 0, 255});
                sdl.drawRect(colA->bounds());
                sdl.drawRect(colB->bounds());

                if (rectsOverlap(colA->bounds(), colB->bounds())) {
					SP<ICollider> coll;
					SP<ICollider> colr;
                    if (center(colA->bounds()).x > center(colB->bounds()).x) {
						coll = colB;
						colr = colA;
                    } else {
						colr = colB;
						coll = colA;
					}

                    SDL_FRect r = colr->bounds();
                    SDL_FPoint rbotr = {r.x + r.w, r.y + r.h};
                    SDL_FPoint rcen = {center(r).x, center(r).y};

                    SDL_FRect l = coll->bounds();
                    SDL_FPoint lbotr = {l.x + l.w, l.y + l.h};
                    SDL_FPoint lcen = {center(l).x, center(l).y};

					if (std::abs(r.x - lbotr.x) < std::abs(rbotr.y - l.y) && std::abs(r.x - lbotr.x) < std::abs(r.y - lbotr.y)) {
						if (!coll->im_immovable && colr->im_immovable) {
							coll->sex(r.x - l.w);
						} else if (coll->im_immovable && !colr->im_immovable) {
							colr->sex(l.x + l.w);
						}
					} else {
						if (!coll->im_immovable && colr->im_immovable) {
							if (std::abs(rbotr.y - l.y) < std::abs(r.y - lbotr.y)) {
								coll->sey(rbotr.y);
								sdl.setColor({255, 255, 0, 255});
								sdl.drawCircle(5, 50, 50);
							} else {
								coll->sey(r.y - l.h);
								sdl.setColor({0, 255, 255, 255});
								sdl.drawCircle(5, 50, 50);
							}
						} else if (coll->im_immovable && !colr->im_immovable) {
							if (std::abs(rbotr.y - l.y) < std::abs(r.y - lbotr.y)) {
								colr->sey(l.y - r.h);
							} else {
								colr->sey(lbotr.y);
							}
						}
					}

                    sdl.setColor({0, 0, 255, 255});
                    sdl.drawRect(colr->bounds());
                    sdl.setColor({255, 0, 0, 255});
                    sdl.drawRect(coll->bounds());
                }
            }
        }

        sdl.present();
    }

EXIT:
    return 0;
}
