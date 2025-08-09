#pragma once

#include <SDL3/SDL_render.h>
#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace loader::ase {
using dword = uint32_t;
using word = uint16_t;
using byte = uint8_t;
// idk
using fixed = float;
using qword = uint64_t;
using long64 = int64_t;

struct Point {
    long x, y;
};

struct Size {
    long w, h;
};

struct Rect {
    Point p;
    Size s;
};

// note: only handling RGBA, also before i get confused: no i do NOT use this in normal ase images, only uncompressed ones which i dont even
// support lmao so this is fucking useless
struct Pixel {
    byte r, g, b, a;
};

struct Tile {
    byte b;
    word w;
    dword dw;
};

struct UUID {
    byte b[16];
};

struct Header {
    dword size;
    word magic;
    word frames;
    word width;
    word height;
    word colorDepth;
    dword flags;
    word speed;
    byte transparentIndex;
    word colorCount;
    byte pixelWidth;
    byte pixelHeight;
    short xPos;
    short yPos;
    word gridWidth;
    word gridHeight;
};

struct Chunk {
    dword size;
    word type;
    std::vector<byte> data;
};

struct Frame {
    dword size;
    word magic;
    word oldChunkCount;
    word duration;
    dword chunkCount;
    std::vector<Chunk> chunks;
};

struct TileSet {
    int32_t width;
    int32_t height;
    std::size_t count;
    std::vector<uint8_t> pixels;
    SDL_Surface* surface;
};

struct TileMap {
    int32_t width;
    int32_t height;
    int32_t xOffset;
    int32_t yOffset;
    word layerIndex;
    uint32_t tileIDBitmask;
    std::vector<uint32_t> tiles;
};

struct Layer {
    std::string name;
    word type;

    // is this the same dword as tilesetID
    dword tilesetIndex;
};

struct Asefile {
	// free framePixels after freeing SDL_Surface
	std::vector<uint32_t *> framePixels;
    std::vector<SDL_Surface *> frames;
    std::vector<word> durations;
    std::vector<std::pair<word, word>> tags;
    std::unordered_map<dword, std::shared_ptr<TileSet>> tilesets;
    std::vector<TileMap> tilemaps;
    std::vector<Layer> layers;
};
} // namespace loader::ase
