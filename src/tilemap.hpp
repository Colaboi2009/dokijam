#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include "engine/texture.hpp"
#include "tileset.hpp"

#include <engine/sdl_wrapper.hpp>


class TileMap {

public:
    TileMap(const std::string& filepath);

    void render(
        const uint16_t layerID,
        const Point position,
        const float scale = 1.0f
    );

private:
    std::unordered_map<uint16_t, TileSet> tileSets;

};
