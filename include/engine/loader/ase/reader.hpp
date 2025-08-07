#pragma once

#include "structs.hpp"

#include <cassert>
#include <fstream>
#include <vector>
#include <zlib.h>

namespace loader::ase {
class Reader {
  private:
	std::ifstream m_f;

  public:
    Reader(std::string filepath);
	~Reader();

	size_t tell();

    void rdbuf(char* dst, std::size_t length);

    byte rbyte();
    word rword();
    dword rdword();
    qword rqword();
    void rpadding(size_t n);
    short rshort();
    long rlong();
    fixed rfixed();
    float rfloat();
    long64 rlong64();
    double rdouble();
    std::string rstring();
    Point rpoint();
    Size rsize();
    Rect rrect();
	Pixel rpixel();
    // idk yet
    Tile rtile();
    UUID ruuid();
    // tysm aseprite src code, although i did have to change a shit ton of stuff
    void rcompressed_image_rgba(word width, word height, dword chunkEnd, std::vector<std::vector<uint32_t>> &image);
};
} // namespace loader::ase

