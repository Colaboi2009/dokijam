#pragma once

#include <string>
#include <vector>
#include <SDL3/SDL.h>
#include <tuple>

class Audio {
  private:
	SDL_AudioDeviceID m_device;

	SDL_AudioStream *m_musicStream;
	std::vector<SDL_AudioStream *> m_sfxStreams;

	std::tuple<SDL_AudioSpec, Uint8 *, Uint32> openWav(std::string filepath);

  public:
	Audio();
	~Audio();

	void clearMusic();
	void queueMusic(std::string filepath);
	void playSFX(std::string filepath);
};
