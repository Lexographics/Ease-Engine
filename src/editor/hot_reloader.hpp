#ifndef HOT_RELOADER_HPP
#define HOT_RELOADER_HPP
#pragma once

#include <functional>
#include <unordered_map>

#include "core/filesystem/folder_file_server.hpp"
#include "sowa.hpp"

class HotReloader {
  public:
	enum class EventType {
		Updated = 0,
		Deleted,
	};

	using ID = u32;
	using Callback = std::function<void(const std::string &path, EventType type)>;

	void Init(FolderFileServer *fs);

	ID WatchPath(const std::string &path, const Callback &callback);
	void RemoveWatcher(ID id);

	void Update();

  private:
	time_t getLastUpdate(const std::string &path);

	struct WatchData {
		std::string path;
		Callback callback;

		time_t lastUpdate;
		bool deleted = false;
	};

	std::unordered_map<ID, WatchData> _watchDatas;
	std::vector<ID> _freeList;

	FolderFileServer *_fs = nullptr;
};

#endif // HOT_RELOADER_HPP