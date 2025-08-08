#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "engine/texture.hpp"
#include "tileset.hpp"

#include <engine/sdl_wrapper.hpp>

class TileMap {

public:
    struct LevelRef {
        // should probably be std::shared_ptr,
        // so we don't need to worry about the std::vector<> reallocating
        TileMapContainer* imageLayer;
        TileSet* imageTileSet;
        TileMapContainer* collisionLayer;
    };

public:
    TileMap(const std::string& filepath);

    void setLevel(const std::string& levelName);
	LevelRef getLevel(const std::string &levelName);

    void render(const Point position, const float scale = 1.0f);

private:
    std::unordered_map<uint16_t, TileSet> tileSets;
    std::vector<TileMapContainer> tileMaps;

    // warn(Skulaurun):
    // This expects tileSets and tileMaps to never change
    // (e.g. runtime insertion/deletion)
    LevelRef ref;
    std::unordered_map<std::string, LevelRef> levelLookup;

};
