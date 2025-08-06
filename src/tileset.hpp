#pragma once

struct TileSet {
    int32_t width;
    int32_t height;
    std::size_t count;
    std::shared_ptr<Texture> texture;
};
