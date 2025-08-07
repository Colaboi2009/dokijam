#include "tilemap.hpp"

#include "engine/loader/ase/aseprite.hpp"

#include <print>

TileMap::TileMap(const std::string& filepath) {
    loader::ase::Asefile f;
    loader::ase::aseprite("art/" + filepath, f);

    constexpr std::size_t LAYER_MAX = 132;

    this->ref = {};
    // Reserve space for 132 layers,
    // e.g.:
    // 64 levels (64 collision layers, 64 image layers, and 4 tileset layer),
    // after that, the vector will reallocate and invalidate LevelRefs
    this->tileMaps.reserve(LAYER_MAX);

    // Load TileSets
    for (const auto& [key, value] : f.tilesets) {
        tileSets.emplace(key, TileSet{
            .width = value->width,
            .height = value->height,
            .count = value->count,
            .texture = std::make_shared<Texture>(value->surface)
        });
    }
    
    // Load TileMaps (Layers)
    for (auto& value : f.tilemaps) {
        if (value.layerIndex >= f.layers.size()) continue;
        auto& layer = f.layers[value.layerIndex];
        TileMapContainer& container = tileMaps.emplace_back(TileMapContainer{
            .layerName = std::move(layer.name),
            .width = value.width,
            .height = value.height,
            .tileSetIndex = layer.tilesetIndex,
            .tiles = std::move(value.tiles)
        });

        // Resolve level references
        if (!container.layerName.starts_with("level_")) {
            continue;
        }
        std::string name = container.layerName.substr(6);

        LevelRef ref = {};
        if (name.ends_with("_collision")) {
            name = name.substr(0, name.size() - 10);
            ref.collisionLayer = &container;
        } else if (name.ends_with("_image")) {
            name = name.substr(0, name.size() - 6);
            ref.imageLayer = &container;
            const std::size_t tileSetID = container.tileSetIndex;

            auto it = tileSets.find(tileSetID);
            if (it != tileSets.end()) {
                ref.imageTileSet = &it->second;
            }
        }

        levelLookup[name] = ref;
    }

	loader::ase::freeAse(f);
}

void TileMap::setLevel(const std::string& levelName) {
    auto it = levelLookup.find(levelName);
    if (it != levelLookup.end()) {
        ref = it->second;
    } else {
        // level not found
    }
}

void TileMap::render(const Point position, const float scale) {
    if (!ref.imageLayer || !ref.imageTileSet) return;

    TileMapContainer& c = *ref.imageLayer;
    TileSet& ts = *ref.imageTileSet;

    const int32_t w = ts.width * scale;
    const int32_t h = ts.height * scale;
    
    SDL_FRect dst = { position.x, position.y, (float)w, (float)h};

    std::size_t i = 0;
    for (std::size_t y = 0; y < c.height; y++) {
        dst.x = position.x;
        for (std::size_t x = 0; x < c.width; x++) {
            uint32_t tileIndex = c.tiles[i++];
            SDL_FRect src = {
                0.0f, (float)tileIndex * ts.height,
                (float)ts.width, (float)ts.height
            };
            ts.texture->render(src, dst);
            dst.x += w;
        }
        dst.y += h;
    }
}
