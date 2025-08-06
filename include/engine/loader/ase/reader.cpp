#include "reader.hpp"

namespace loader::ase {
Reader::Reader(std::string filepath) : m_f(filepath, std::ios_base::binary) {
    assert(m_f.is_open() && "Couldn't open aseprite file!");
}

Reader::~Reader() { m_f.close(); }

size_t Reader::tell() { return m_f.tellg(); }

void Reader::rdbuf(char* dst, std::size_t length) {
    m_f.read(dst, length);
}

byte Reader::rbyte() {
    byte c;
    m_f.read((char *)&c, 1);
    return c;
}

word Reader::rword() {
    byte b[2];
    m_f.read((char *)b, 2);
    return (int(b[1]) << 8) | int(b[0]);
}

dword Reader::rdword() {
    byte b[4];
    m_f.read((char *)b, 4);
    return (int(b[3]) << 24) | (int(b[2]) << 16) | (int(b[1]) << 8) | int(b[0]);
}

qword Reader::rqword() {
    byte b[8];
    m_f.read((char *)b, 8);
    return (qword(b[7]) << 56) | (qword(b[6]) << 48) | (qword(b[5]) << 40) | (qword(b[4]) << 32) | (qword(b[3]) << 24) |
           (qword(b[2]) << 16) | (qword(b[1]) << 8) | (qword(b[0]));
}

void Reader::rpadding(size_t n) {
    byte *b = new byte[n];
    m_f.read((char *)b, n);
}

short Reader::rshort() { return rword(); }

long Reader::rlong() { return rdword(); }

fixed Reader::rfixed() { return rdword(); }

float Reader::rfloat() { return rdword(); }

long64 Reader::rlong64() { return rqword(); }

double Reader::rdouble() {
    long64 val = rlong64();
    return *reinterpret_cast<double *>(&val);
}

std::string Reader::rstring() {
    std::string s;
    word length = rword();
    char *cs = new char[length];
    m_f.read(cs, length);
    for (int i = 0; i < length; i++) {
        s += cs[i];
    }
    return s;
}

Point Reader::rpoint() { return {rlong(), rlong()}; }

Size Reader::rsize() { return {rlong(), rlong()}; }

Rect Reader::rrect() { return {rpoint(), rsize()}; }

Pixel Reader::rpixel() { return {rbyte()}; }

// idk yet
Tile Reader::rtile() { return {}; }

UUID Reader::ruuid() {
    UUID u;
    m_f.read((char *)u.b, 16);
    return u;
}

// tysm aseprite src code, although i did have to change a shit ton of stuff
void Reader::rcompressed_image_rgba(word width, word height, dword chunkEnd, std::vector<std::vector<uint32_t>> &image) {
    z_stream zstream;
    zstream.zalloc = (alloc_func)0;
    zstream.zfree = (free_func)0;
    zstream.opaque = (voidpf)0;

    int err = inflateInit(&zstream);

    const int widthBytes = width * 4; // RGBA
    std::vector<uint8_t> scanline(widthBytes);
    std::vector<uint8_t> compressed(4096);
    std::vector<uint8_t> uncompressed(4096);
    int scanline_offset = 0;
    int y = 0;

    image.resize(height);
    for (int i = 0; i < image.size(); i++) {
        image[i].resize(width);
    }

    while (true) {
        size_t input_bytes;

        if (size_t(m_f.tellg()) + compressed.size() > chunkEnd) {
            input_bytes = chunkEnd - m_f.tellg(); // Remaining bytes
            assert(input_bytes < compressed.size());

            if (input_bytes <= 0) {
                break; // Done, we consumed all chunk
            }
        } else {
            input_bytes = compressed.size();
        }

        m_f.read((char *)compressed.data(), input_bytes);

        zstream.next_in = (Bytef *)&compressed[0];
        zstream.avail_in = input_bytes;

        do {
            zstream.next_out = (Bytef *)&uncompressed[0];
            zstream.avail_out = uncompressed.size();

            err = inflate(&zstream, Z_NO_FLUSH);

            size_t uncompressed_bytes = uncompressed.size() - zstream.avail_out;
            if (uncompressed_bytes > 0) {
                int i = 0;
                while (y < height) {
                    int n = std::min(uncompressed_bytes, scanline.size() - scanline_offset);
                    if (n > 0) {
                        std::copy(uncompressed.begin() + i, uncompressed.begin() + i + n, scanline.begin() + scanline_offset);
                        uncompressed_bytes -= n;
                        scanline_offset += n;
                        i += n;
                    } else if (scanline_offset < widthBytes) {
                        break;
                    } else {
                        for (int x = 0; x < width; x++) {
                            int i = x * 4;
                            image[y][x] = (scanline[i]) | (scanline[i + 1] << 8) | (scanline[i + 2] << 16) | (scanline[i + 3] << 24);
                        }

                        ++y;
                        scanline_offset = 0;
                        if (uncompressed_bytes == 0)
                            break;
                    }
                }
            }
        } while (zstream.avail_in != 0 && zstream.avail_out == 0);
    }
    err = inflateEnd(&zstream);
}
} // namespace loader::ase
