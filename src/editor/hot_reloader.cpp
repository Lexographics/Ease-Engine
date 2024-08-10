#include "hot_reloader.hpp"

#include <limits>
#include <sys/stat.h>

#include "core/application.hpp"
#include "core/debug.hpp"
#include "data/id_generator.hpp"

void HotReloader::Init(FolderFileServer *fs) {
	_fs = fs;
}

HotReloader::ID HotReloader::WatchPath(const std::string &path, const Callback &callback) {
	if (App().FS().ResolvePath(path).scheme != "res") {
		return 0;
	}

	static LinearIDGenerator<ID> gen;
	ID id = gen.Next();

	_watchDatas[id].path = path;
	_watchDatas[id].callback = callback;
	_watchDatas[id].lastUpdate = getLastUpdate(path);

	return id;
}

void HotReloader::RemoveWatcher(ID id) {
	_freeList.push_back(id);
}

void HotReloader::Update() {
	for (auto &id : _freeList) {
		auto it = _watchDatas.find(id);
		if (it == _watchDatas.end())
			continue;

		_watchDatas.erase(it);
	}
	_freeList.clear();

	for (auto &[id, data] : _watchDatas) {
		if (data.deleted) {
			continue;
		}

		bool exists = _fs->Exists(data.path);
		if (!data.deleted && !exists) {
			data.callback(data.path, EventType::Deleted);
			data.deleted = true;
		}

		time_t lastUpdate = getLastUpdate(data.path);
		if (lastUpdate > data.lastUpdate) {
			data.callback(data.path, EventType::Updated);
		}
		data.lastUpdate = lastUpdate;
	}
}

time_t HotReloader::getLastUpdate(const std::string &path) {
	if (_fs == nullptr) {
		return std::numeric_limits<time_t>::max();
	}

	auto pathData = App().FS().ResolvePath(path);
	if (pathData.scheme != "res") {
		return std::numeric_limits<time_t>::max();
	}

	auto fullPath = _fs->GetPath(pathData.path);

	struct stat st;
	stat(fullPath.string().c_str(), &st);

	return st.st_mtim.tv_sec;
}