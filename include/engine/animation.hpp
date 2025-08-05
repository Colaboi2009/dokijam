#pragma once

#include "texture.hpp"
#include "point.hpp"
#include <string>
#include <vector>

class Animation {
  private:
    int m_frameCount;
    std::vector<int> m_delays;
    std::vector<Texture> m_textures;

	std::vector<int> m_tags{};
	int m_curTagIndex = 0;

    Uint64 m_frameStart = SDL_GetTicks();
    int m_frame = 0;

	bool m_playing = false;
	bool m_repeat = false;

	void create_gif(std::string filepath);
	void create_aseprite(std::string filepath);

  public:
    Animation(std::string filepath);
    ~Animation();

	//int scale() const { return m_scale; }
	//void setScale(float s) { m_scale = s; }

	bool playing() const { return m_playing; }
	int tagIndex() const { return m_curTagIndex; }
	std::vector<Texture> &frames() { return m_textures; }

	void stop();
	void play();
	void restart();

	void once(int i = 0);
	void repeat(int i = 0);

    void render(SDL_FRect dst);
};
