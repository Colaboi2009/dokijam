#include "audio.hpp"

Audio::Audio() {
	m_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
}

Audio::~Audio() {
	SDL_DestroyAudioStream(m_musicStream);
	for (SDL_AudioStream *stream : m_sfxStreams) {
		if (stream) {
			SDL_DestroyAudioStream(stream);
		}
	}
}

void Audio::clearMusic() {
	SDL_DestroyAudioStream(m_musicStream);
	m_musicStream = NULL;
}

void Audio::queueMusic(std::string filepath) {
	auto [audiospec, buf, len] = openWav(filepath);
	if (!m_musicStream) {
		m_musicStream = SDL_CreateAudioStream(&audiospec, NULL);
		SDL_BindAudioStream(m_device, m_musicStream);
	}
	SDL_PutAudioStreamData(m_musicStream, buf, len);
}

void Audio::playSFX(std::string filepath) {
	for (int i = 0; i < m_sfxStreams.size(); i++) {
		if (m_sfxStreams[i] && SDL_GetAudioStreamAvailable(m_sfxStreams[i]) == 0) {
			SDL_DestroyAudioStream(m_sfxStreams[i]);
			m_sfxStreams[i] = NULL;
		}
	}

	int i;
	for (i = 0; i < m_sfxStreams.size(); i++) {
		if (!m_sfxStreams[i]) {
			break;
		}
	}
	if (i == m_sfxStreams.size()) {
		m_sfxStreams.push_back({});
	} 

	auto [audiospec, buf, len] = openWav(filepath);
	m_sfxStreams[i] = SDL_CreateAudioStream(&audiospec, NULL);
	SDL_BindAudioStream(m_device, m_sfxStreams[i]);
	SDL_PutAudioStreamData(m_sfxStreams[i], buf, len);
}

std::tuple<SDL_AudioSpec, Uint8 *, Uint32> Audio::openWav(std::string filepath) {
    SDL_AudioSpec audiospec;
    Uint8 *buf;
    Uint32 len;
    SDL_LoadWAV(filepath.c_str(), &audiospec, &buf, &len);
	return std::make_tuple(audiospec, buf, len);
}
