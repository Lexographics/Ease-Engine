#include "audio_server.hpp"

#include "AL/al.h"
#include "AL/alc.h"

#include "core/debug.hpp"

AudioServer::AudioServer() {
	_pDevice = alcOpenDevice(NULL);
	if (!_pDevice) {
		Debug::Error("Failed to open audio device: {}", alGetError());
		return;
	}

	_pContext = alcCreateContext(_pDevice, NULL);
	if (!alcMakeContextCurrent(_pContext)) {
		Debug::Error("Failed to bind audio context: {}", alGetError());
		return;
	}

	ALfloat listener[] = {0.f, 0.f, 1.f, 0.f, 1.f, 0.f};
	alListener3f(AL_POSITION, 0.f, 0.f, 1.f);
	alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f);
	alListenerfv(AL_ORIENTATION, listener);
}

AudioServer::~AudioServer() {
	alcDestroyContext(_pContext);
	alcCloseDevice(_pDevice);
}