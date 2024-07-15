#ifndef AUDIOSTREAMPLAYER_HPP
#define AUDIOSTREAMPLAYER_HPP
#pragma once

#include "resource/audio_stream.hpp"
#include "scene/node.hpp"

class AudioStreamPlayer : public Node {
  public:
	virtual ~AudioStreamPlayer();
	AudioStreamPlayer();

	void Start() override;
	void Update() override;
	void Exit() override;

	bool Serialize(Document &doc) override;
	bool Deserialize(const Document &doc) override;

	bool Copy(Node *dst) override;
	void UpdateEditor() override;

	void Play();
	void Pause();
	void Stop();

	bool IsPlaying();
	bool IsPaused();
	bool HasValidAudio();

  public:
	RID _stream;
	bool _autoplay = false;
	bool _loop = false;
	float _pitch = 1.f; // min 0.5, max 2.0
	float _gain = 1.f;

  private:
	uint32_t _sourceID = 0;
	uint32_t _lastBuffer = 0;

	void updateSource();
};

#endif // AUDIOSTREAMPLAYER_HPP