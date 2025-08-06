#include "animation.hpp"

#include "loader/ase/aseprite.hpp"

#include <SDL3_image/SDL_image.h>
#include <cassert>

#include <stdexcept>

Animation::Animation(std::string filepath) {
    filepath = "art/" + filepath;
    if (filepath.contains(".aseprite")) {
        create_aseprite(filepath);
    } else if (filepath.contains(".gif")) {
        create_gif(filepath);
    } else {
		throw std::runtime_error("Failed to open animation, filetype unsupported!");
	}
}

Animation::~Animation() {}

void Animation::create_gif(std::string filepath) {
    if (filepath.contains(".gif")) {
        IMG_Animation *anim = IMG_LoadAnimation(filepath.c_str());
        assert(anim != NULL && "Could not open animation!");
        m_frameCount = anim->count;
        m_textures.resize(m_frameCount);
        m_delays.resize(m_frameCount);
        for (int i = 0; i < m_frameCount; i++) {
            m_delays[i] = anim->delays[i];
            m_textures[i].create(anim->frames[i]);
        }

        IMG_FreeAnimation(anim);
    } else {
        assert(false && "File type not supported!");
    }
}

void Animation::create_aseprite(std::string filepath) {
    loader::ase::Asefile f;
    loader::ase::aseprite(filepath, f);

    m_frameCount = f.frames.size();
    m_textures.resize(m_frameCount);
    m_delays.resize(m_frameCount);
    m_tags.resize(f.tags.size());
    for (int i = 0; i < m_frameCount; i++) {
        m_textures[i].create(f.frames[i]);
        SDL_DestroySurface(f.frames[i]);
        m_delays[i] = f.durations[i];
    }
    for (int i = 0; i < m_tags.size(); i++) {
        m_tags[i] = f.tags[i].first;
    }
}

void Animation::stop() { m_playing = false; }

void Animation::play() { m_playing = true; }

void Animation::restart() { m_frame = m_tags.size() == 0 ? 0 : m_tags[m_curTagIndex]; }

void Animation::once(int i) {
    if (i != 0 && m_tags.size() == 0) {
		i = 0;
    }
    m_repeat = false;
    m_curTagIndex = i;
    restart();
    play();
}

void Animation::repeat(int i) {
    if (i != 0 && m_tags.size() == 0) {
		i = 0;
    }
    m_repeat = true;
    m_curTagIndex = i;
    restart();
    play();
}

void Animation::render(const float x, const float y, const float scale) {
    if (m_playing && SDL_GetTicks() - m_frameStart > m_delays[m_frame]) {
        m_frame++;
        m_frameStart = SDL_GetTicks();

        int end = (m_tags.size() == 0 || m_curTagIndex + 1 >= m_tags.size()) ? m_frameCount : m_tags[m_curTagIndex + 1];
        if (m_frame >= end) {
            if (!m_repeat) {
                m_playing = false;
            }
            restart();
        }
    }
    m_textures[m_frame].render(Point{x, y}, scale);
}
