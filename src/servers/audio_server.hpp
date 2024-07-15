#ifndef AUDIO_SERVER_HPP
#define AUDIO_SERVER_HPP
#pragma once

class ALCdevice;
class ALCcontext;

class AudioServer {
  public:
  private:
	friend class Application;

	AudioServer();
	~AudioServer();

	ALCdevice *_pDevice = nullptr;
	ALCcontext *_pContext = nullptr;
};

#endif // AUDIO_SERVER_HPP