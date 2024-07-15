#include "audiostreamplayer.hpp"

#include "AL/al.h"

#include "core/application.hpp"
#include "core/debug.hpp"

#include "math/math.hpp"

#include "imgui.h"

AudioStreamPlayer::AudioStreamPlayer() {
	alGenSources(1, &_sourceID);
	alSourcef(_sourceID, AL_PITCH, 1.f);
	alSourcef(_sourceID, AL_GAIN, 1.f);
	alSourcei(_sourceID, AL_LOOPING, false);
}

AudioStreamPlayer::~AudioStreamPlayer() {
	if (HasValidAudio()) {
		Stop();
	}
	alDeleteSources(1, &_sourceID);
}

void AudioStreamPlayer::Exit() {
	Stop();
}

void AudioStreamPlayer::Play() {
	AudioStream *stream = dynamic_cast<AudioStream *>(App().GetResourceRegistry().GetResource(_stream));
	if (!stream) {
		Debug::Error("Failed to play audio. Stream not found: {}", _stream);
		return;
	}

	if (stream->ID() == 0) {
		Debug::Error("Failed to play audio. Invalid stream");
		return;
	}

	if (_lastBuffer != stream->ID()) {
		_lastBuffer = stream->ID();
		alSourcei(_sourceID, AL_BUFFER, _lastBuffer);
	}

	updateSource();
	alSourcePlay(_sourceID);
}

void AudioStreamPlayer::Stop() {
	AudioStream *stream = dynamic_cast<AudioStream *>(App().GetResourceRegistry().GetResource(_stream));
	if (!stream) {
		Debug::Error("Failed to stop audio. Stream not found: {}", _stream);
		return;
	}

	if (stream->ID() == 0) {
		Debug::Error("Failed to stop audio. Invalid stream");
		return;
	}

	alSourceStop(_sourceID);
}

void AudioStreamPlayer::Pause() {
	AudioStream *stream = dynamic_cast<AudioStream *>(App().GetResourceRegistry().GetResource(_stream));
	if (!stream) {
		Debug::Error("Failed to pause audio. Stream not found");
		return;
	}

	if (stream->ID() == 0) {
		Debug::Error("Failed to pause audio. Invalid stream");
		return;
	}

	alSourcePause(_sourceID);
}

bool AudioStreamPlayer::IsPlaying() {
	int playing = 0;
	alGetSourcei(_sourceID, AL_SOURCE_STATE, &playing);

	return playing == AL_PLAYING;
}

bool AudioStreamPlayer::IsPaused() {
	int playing = 0;
	alGetSourcei(_sourceID, AL_SOURCE_STATE, &playing);

	return playing == AL_PAUSED;
}

bool AudioStreamPlayer::HasValidAudio() {
	AudioStream *stream = dynamic_cast<AudioStream *>(App().GetResourceRegistry().GetResource(_stream));
	if (!stream) {
		return false;
	}

	if (stream->ID() == 0) {
		return false;
	}

	return true;
}

void AudioStreamPlayer::Start() {
	if (_autoplay) {
		Play();
	}
}

void AudioStreamPlayer::Update() {
	if (IsPlaying())
		updateSource();
}

bool AudioStreamPlayer::Serialize(Document &doc) {
	if (!Node::Serialize(doc))
		return false;

	doc.Set("Stream", _stream);
	doc.Set("Autoplay", _autoplay);
	doc.Set("Pitch", _pitch);
	doc.Set("Gain", _gain);
	doc.Set("Loop", _loop);
	return true;
}

bool AudioStreamPlayer::Deserialize(const Document &doc) {
	if (!Node::Deserialize(doc))
		return false;

	_stream = doc.Get("Stream", _stream);
	_autoplay = doc.Get("Autoplay", _autoplay);
	_loop = doc.Get("Loop", _loop);
	_gain = doc.Get("Gain", _gain);
	_pitch = doc.Get("Pitch", _pitch);

	return true;
}

bool AudioStreamPlayer::Copy(Node *dst) {
	if (!Node::Copy(dst))
		return false;

	AudioStreamPlayer *dstNode = dynamic_cast<AudioStreamPlayer *>(dst);
	dstNode->_stream = _stream;
	dstNode->_autoplay = _autoplay;
	dstNode->_loop = _loop;
	dstNode->_gain = _gain;
	dstNode->_pitch = _pitch;

	return true;
}

void AudioStreamPlayer::UpdateEditor() {
	if (ImGui::CollapsingHeader("AudioStreamPlayer", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		ImGui::Text("Stream");
		ImGui::SameLine();
		ImGui::InputInt("##Stream", &_stream);

		if (IsPlaying()) {
			if (ImGui::Button("Stop")) {
				Stop();
			}
		} else {
			if (ImGui::Button("Play")) {
				Play();
			}
		}

		ImGui::Text("Autoplay");
		ImGui::SameLine();
		ImGui::Checkbox("##Autoplay", &_autoplay);

		ImGui::Text("Loop");
		ImGui::SameLine();
		ImGui::Checkbox("##Loop", &_loop);

		ImGui::Text("Gain");
		ImGui::SameLine();
		ImGui::DragFloat("##Gain", &_gain, 0.001, 0.f, FLT_MAX, "%.3f", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text("Pitch");
		ImGui::SameLine();
		ImGui::SliderFloat("##Pitch", &_pitch, 0.5f, 2.f);

		ImGui::Unindent();
	}

	Node::UpdateEditor();
}

void AudioStreamPlayer::updateSource() {
	alSourcef(_sourceID, AL_PITCH, Math::Clamp(_pitch, 0.5f, 2.f));
	alSourcef(_sourceID, AL_GAIN, _gain);
	alSourcei(_sourceID, AL_LOOPING, _loop);
}