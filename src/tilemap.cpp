#include "tilemap.hpp"
#include "ecs/ecs.hpp"
#include "engine/loader/ase/aseprite.hpp"

#include <print>

#include <optional>

enum class LayerType {
    Unknown, Image, Collision
};

struct LayerInfo {
    std::string levelName;
    LayerType type;
};

static std::optional<LayerInfo> parseName(const std::string_view layerName) {

    constexpr std::string_view IMAGE = "_image";
    constexpr std::string_view COLLISION = "_collision";
    constexpr std::string_view LEVEL = "level_";

    if (!layerName.starts_with(LEVEL)) {
        return {};
    }

    LayerInfo info = {};
    info.levelName = layerName.substr(LEVEL.size());
    if (info.levelName.ends_with(COLLISION)) {
        info.levelName = info.levelName.substr(0, info.levelName.size() - COLLISION.size());
        info.type = LayerType::Collision;
    } else if (info.levelName.ends_with(IMAGE)) {
        info.levelName = info.levelName.substr(0, info.levelName.size() - IMAGE.size());
        info.type = LayerType::Image;
    }

    return info;
}

TileMap::TileMap(const std::string& filepath) {
    loader::ase::Asefile f;
    loader::ase::aseprite("art/" + filepath, f);

    this->ref = {};
    tileSets.reserve(32);

    // Load TileSets
    for (const auto& [key, value] : f.tilesets) {
        tileSets.emplace(key, TileSet{
            .width = value->width,
            .height = value->height,
            .count = value->count,
            .texture = std::make_shared<Texture>(value->surface)
        });
    }
    
    std::size_t lastIndex = 0;

    // Load TileMaps (Layers)
    for (auto& value : f.tilemaps) {
        if (value.layerIndex >= f.layers.size()) continue;
        auto& layer = f.layers[value.layerIndex];
        if (lastIndex >= tileMaps.size()) {
            printf("Error: Bad TileMap file, layer limit of 132 reached.");
            break;
        }

        tileMaps[lastIndex] = TileMapContainer{
            .layerName = std::move(layer.name),
            .width = value.width,
            .height = value.height,
            .xOffset = value.xOffset,
            .yOffset = value.yOffset,
            .tileSetIndex = layer.tilesetIndex,
            .tiles = std::move(value.tiles)
        };
        TileMapContainer& container = tileMaps[lastIndex++];

        // Resolve level references
        std::optional<LayerInfo> infoOpt = parseName(container.layerName);
        if (infoOpt.has_value()) {
            LayerInfo& info = infoOpt.value();
            LevelRef ref = getLevel(info.levelName);

            const std::size_t tileSetID = container.tileSetIndex;
                auto it = tileSets.find(tileSetID);
            if (it == tileSets.end()) {
                continue;
            }

            if (info.type == LayerType::Collision) {
                ref.collisionLayer = { &container, &it->second };
            } else if (info.type == LayerType::Image) {
                    ref.imageLayers[value.layerIndex] = { &container, &it->second };
            } else {
                continue;
                }

                levelLookup[info.levelName] = ref;
            }
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

TileMap::LevelRef TileMap::getLevel(const std::string &levelName) {
    auto it = levelLookup.find(levelName);
    if (it != levelLookup.end()) {
        return it->second;
    } 
	return {};
}

void TileMap::registerEntities(entt::registry& registry, const Point position, const float scale) {
    for (auto& entity : entities) {
        registry.destroy(entity);
    }

    // TODO: Connect Connected-Components to Rectangles

    if (!ref.collisionLayer.first || !ref.collisionLayer.second) return;
    TileMapContainer& c = *ref.collisionLayer.first;
    TileSet& ts = *ref.collisionLayer.second;

    const int32_t w = ts.width * scale;
    const int32_t h = ts.height * scale;
    const int32_t x = position.x + c.xOffset * scale;
    const int32_t y = position.y + c.yOffset * scale;

    SDL_FRect dst = { static_cast<float>(x), static_cast<float>(y), (float)w, (float)h };

    std::size_t i = 0;
    for (std::size_t y = 0; y < c.height; y++) {
        dst.x = x;
        for (std::size_t x = 0; x < c.width; x++) {
            uint32_t tileIndex = c.tiles[i++];

            if (tileIndex != 0) {
                entt::entity e = registry.create();
                registry.emplace<ecs::Position>(e, dst.x, dst.y);
                registry.emplace<ecs::BoxCollider>(e, 0, 0, dst.w, dst.h);
                entities.push_back(e);
            }

            dst.x += w;
        }
        dst.y += h;
    }
}

void TileMap::render(const Point position, const float scale) {
    for (const auto& [_, value] : ref.imageLayers) {
        if (!value.first || !value.second) return;

        TileMapContainer& c = *value.first;
        TileSet& ts = *value.second;

        const int32_t w = ts.width * scale;
        const int32_t h = ts.height * scale;
        const int32_t x = position.x + c.xOffset * scale;
        const int32_t y = position.y + c.yOffset * scale;

        SDL_FRect dst = { static_cast<float>(x), static_cast<float>(y), (float)w, (float)h };

        std::size_t i = 0;
        for (std::size_t y = 0; y < c.height; y++) {
            dst.x = x;
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
}
