#ifndef AUDIO_STREAM_HPP
#define AUDIO_STREAM_HPP
#pragma once

#include <cstdint>

#include "core/resource.hpp"

class AudioStream : public Resource {
  public:
	AudioStream();
	virtual ~AudioStream();

	void LoadResource(const Document &doc) override {
		std::string path = doc.Get("Path", std::string(""));
		if (path != "")
			Load(path.c_str());
	}

	void SaveResource(Document &doc) override {
		doc.Set("Path", Filepath());
	}

	void Load(const char *path);

	void Delete();

	inline uint32_t ID() const { return _id; }

  private:
	uint32_t _id = 0;
};

#endif // AUDIO_STREAM_HPP