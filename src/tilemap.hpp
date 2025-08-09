#pragma once

#include <string>
#include <memory>
#include <array>
#include <map>
#include <unordered_map>

#include "engine/texture.hpp"
#include "tileset.hpp"

#include <engine/sdl_wrapper.hpp>

class TileMap {

public:
    struct LevelRef {
        std::map<std::size_t, std::pair<TileMapContainer*, TileSet*>> imageLayers;
        //TileMapContainer* imageLayer;
        //TileSet* imageTileSet;
        TileMapContainer* collisionLayer;
    };

public:
    TileMap(const std::string& filepath);

    void setLevel(const std::string& levelName);
	LevelRef getLevel(const std::string &levelName);

    void render(const Point position, const float scale = 1.0f);

private:
    std::unordered_map<uint16_t, TileSet> tileSets;
    std::array<TileMapContainer, 132> tileMaps;

    // warn(Skulaurun):
    // This expects tileSets and tileMaps to never change
    // (e.g. runtime insertion/deletion)
    LevelRef ref;
    std::unordered_map<std::string, LevelRef> levelLookup;

};
