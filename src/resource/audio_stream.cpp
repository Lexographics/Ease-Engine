#include "audio_stream.hpp"

#include <vector>

#include "AL/alext.h"
#include "sndfile.h"

#include "core/application.hpp"
#include "core/debug.hpp"

struct sf_func_data {
	std::vector<std::byte> data;
	sf_count_t offset = 0;
};

static sf_count_t sf_func_get_file_len(void *userdata) {
	sf_func_data *func_data = reinterpret_cast<sf_func_data *>(userdata);
	return func_data->data.size();
}

static sf_count_t sf_func_read(void *ptr, sf_count_t count, void *userdata) {
	sf_func_data *func_data = reinterpret_cast<sf_func_data *>(userdata);
	if (static_cast<size_t>(func_data->offset + count) > func_data->data.size()) {
		count = func_data->data.size() - func_data->offset;
	}

	memcpy(ptr, func_data->data.data() + func_data->offset, count);
	func_data->offset += count;
	return count;
}

static sf_count_t sf_func_seek(sf_count_t offset, int whence, void *userdata) {
	sf_func_data *func_data = reinterpret_cast<sf_func_data *>(userdata);

	if (whence == SEEK_SET) {
		func_data->offset = offset;
	} else if (whence == SEEK_CUR) {
		func_data->offset += offset;
	} else if (whence == SEEK_END) {
		func_data->offset = func_data->data.size() - offset;
	}

	return func_data->offset;
}

static sf_count_t sf_func_tell(void *userdata) {
	sf_func_data *func_data = reinterpret_cast<sf_func_data *>(userdata);
	return func_data->offset;
}

static sf_count_t sf_func_write(const void *data, sf_count_t count, void *userdata) {
	sf_func_data *func_data = reinterpret_cast<sf_func_data *>(userdata);
	return 0;
}

AudioStream::AudioStream() {
	_resourceType = typeid(AudioStream).hash_code();
}

AudioStream::~AudioStream() {
	Delete();
}

void AudioStream::Load(const char *path) {
	_filepath = "";

	Delete();
	Ref<FileData> file = App().FS().Load(path);
	if (!file) {
		Debug::Error("Failed to load file: {}", path);
		return;
	}

	ALenum err, format;
	ALuint buffer;
	SNDFILE *sndfile;
	SF_INFO sfinfo;
	short *membuf;
	sf_count_t numFrames;
	ALsizei numBytes;

	std::vector<std::byte> bytes;
	bytes.resize(file->Size());
	memcpy(bytes.data(), file->Data(), file->Size());

	SF_VIRTUAL_IO io;
	io.get_filelen = sf_func_get_file_len;
	io.read = sf_func_read;
	io.seek = sf_func_seek;
	io.tell = sf_func_tell;
	io.write = sf_func_write;

	sf_func_data func_data;
	func_data.data = bytes;
	func_data.offset = 0;

	sndfile = sf_open_virtual(&io, SFM_READ, &sfinfo, &func_data);
	if (!sndfile) {
		Debug::Error("Failed to open audio file {}", path);
		return;
	}

	if (sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT32_MAX / sizeof(short)) / sfinfo.channels) {
		Debug::Error("Bad sample count");
		return;
	}

	format = AL_NONE;
	if (sfinfo.channels == 1) {
		format = AL_FORMAT_MONO16;
	} else if (sfinfo.channels == 2) {
		format = AL_FORMAT_STEREO16;
	} else if (sfinfo.channels == 3) {
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) {
			format = AL_FORMAT_BFORMAT2D_16;
		}
	} else if (sfinfo.channels == 4) {
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT) {
			format = AL_FORMAT_BFORMAT3D_16;
		}
	}

	if (!format) {
		Debug::Error("Unsupported channel count: {}", sfinfo.channels);
		sf_close(sndfile);
		return;
	}

	membuf = static_cast<short *>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
	numFrames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (numFrames < 1) {
		free(membuf);
		sf_close(sndfile);
		Debug::Error("Failed to read samples");
		return;
	}

	numBytes = (ALsizei)(numFrames * sfinfo.channels) * (ALsizei)sizeof(short);

	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, numBytes, sfinfo.samplerate);

	_id = buffer;
	free(membuf);
	sf_close(sndfile);

	err = alGetError();
	if (err != AL_NO_ERROR) {
		Debug::Error("AL ERROR: {}", alGetString(err));
		Delete();
		return;
	}

	_filepath = path;
}

void AudioStream::Delete() {
	if (_id != 0)
		alDeleteBuffers(1, &_id);

	_id = 0;
}