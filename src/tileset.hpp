#pragma once

#include <memory>
#include <vector>
#include "engine/texture.hpp"

struct TileSet {
    int32_t width;
    int32_t height;
    std::size_t count;
    std::shared_ptr<Texture> texture;
};

struct TileMapContainer {
    std::string layerName;

    int32_t width;
    int32_t height;
    // not needed rn
    //uint32_t tileIDBitmask;
    std::size_t tileSetIndex;
    std::vector<uint32_t> tiles;
};
