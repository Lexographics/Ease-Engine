#ifndef EDITOR_HPP
#define EDITOR_HPP
#pragma once

#ifdef SW_EDITOR

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "sowa.hpp"

#include "core/filesystem/filesystem.hpp"
#include "hot_reloader.hpp"
#include "scene/node.hpp"
#include "scene/node/camera2d.hpp"
#include "scene/scene.hpp"

#include "core/filesystem/filesystem.hpp"

struct EditorSettings {
	bool coloredLogMessages = true;
};

class Editor {
  public:
	void Init();

	void Begin();
	void Update();
	void End();

	inline EditorSettings &GetSettings() { return _settings; }

	glm::mat4 GetCamera2DMatrix();

	inline void SetCurrentAnimation(RID animation) {
		_currentAnimation = animation;
		_currentAnimName = "";
	}

	inline const Rect &GetViewportRect() const { return _viewportRect; }
	bool HasFocus();

	inline HotReloader &GetHotReloader() { return _hotReloader; }

  private:
	EditorSettings _settings;
	NodeID _selectedNodeID = 0;

	// TODO: Editor cameras should be stored per scene
	Camera2D _camera2d;
	bool _cameraDragging = false;
	float _cameraZoom = 1.f;

	std::vector<Ref<Scene>> _scenes;
	// [extension] = event
	std::unordered_map<std::string, std::function<void(std::filesystem::path)>> _fileLeftClickEvent;
	std::unordered_map<std::string, std::function<void(std::filesystem::path)>> _fileContextMenu;
	FileEntry _currentRClickPath;

	bool _ignoreOnSceneChanged = false;

	RID _currentAnimation = 0;
	std::string _currentAnimName = "";

	Rect _viewportRect;

	FolderFileServer *_fs;
	HotReloader _hotReloader;
};

#endif // SW_EDITOR
#endif // EDITOR_HPP