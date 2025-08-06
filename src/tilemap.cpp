#include "tilemap.hpp"

#include "engine/loader/ase/aseprite.hpp"

#include <print>

TileMap::TileMap(const std::string& filepath) {
    loader::ase::Asefile f;
    loader::ase::aseprite("art/" + filepath, f);

    // Load TileSets
    for (const auto [key, value] : f.tilesets) {
        tileSets.emplace(key, TileSet{
            .width = value->width,
            .height = value->height,
            .count = value->count,
            .texture = std::make_shared<Texture>(value->surface)
        });
    }

    std::println("s={}", f.tilemaps.size());

    // TODO: Load TileMaps (Layers)
}

void TileMap::render(const uint16_t layerID, const Point position, const float scale) {
    uint16_t example[16] = {
        2, 0, 0, 2,
        0, 1, 1, 0,
        2, 0, 0, 2,
        1, 1, 1, 1,
    };

    // Loaded from TileMap?
    const uint16_t tileSetID = 0;

    auto it = tileSets.find(tileSetID);
    if (it == tileSets.end()) {
        return;
    }
    TileSet& ts = it->second;

    const int32_t w = ts.width * scale;
    const int32_t h = ts.height * scale;
    
    SDL_FRect dst = { position.x(), position.y(), w, h};

    std::size_t i = 0;
    for (std::size_t y = 0; y < 4; y++) {
        dst.x = position.x();
        for (std::size_t x = 0; x < 4; x++) {
            uint16_t tileIndex = example[i++];
            SDL_FRect src = {
                0.0f, tileIndex * ts.height,
                ts.width, ts.height
            };
            ts.texture->render(src, dst);
            dst.x += w;
        }
        dst.y += h;
    }
}
