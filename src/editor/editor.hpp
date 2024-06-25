#ifndef EDITOR_HPP
#define EDITOR_HPP
#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "sowa.hpp"

#include "scene/node.hpp"
#include "scene/node/camera2d.hpp"
#include "scene/scene.hpp"

struct EditorSettings {
	bool coloredLogMessages = true;
};

enum class EditorNodePropType {
	None = 0,
	Int32,
	Float,
	Vec2,
	RID,
	Color,
	String,
	Bool,
};

struct EditorNodeProp {
	std::string propName = "";
	EditorNodePropType propType = EditorNodePropType::None;
	std::function<void *(Node *)> getFunction;

	EditorNodeProp() = default;
	EditorNodeProp(
		const std::string &propName,
		EditorNodePropType propType,
		std::function<void *(Node *)> getFunction)
		: propName(propName),
		  propType(propType),
		  getFunction(getFunction) {}
};

class Editor {
  public:
	void Init();

	void Begin();
	void Update();
	void End();

	inline EditorSettings &GetSettings() { return _settings; }

	inline void RegisterNodeProp(NodeTypeID nodeType, EditorNodeProp prop) { _nodeProps[nodeType].push_back(prop); }

	glm::mat4 GetCamera2DMatrix();

  private:
	EditorSettings _settings;
	NodeID _selectedNodeID = 0;

	std::unordered_map<NodeTypeID, std::vector<EditorNodeProp>> _nodeProps;

	// TODO: Editor cameras should be stored per scene
	Camera2D _camera2d;
	bool _cameraDragging = false;
	float _cameraZoom = 1.f;

	std::vector<Ref<Scene>> _scenes;
	// [extension] = event
	std::unordered_map<std::string, std::function<void(std::filesystem::path)>> _fileClickEvent;

	bool _ignoreOnSceneChanged = false;
};

#endif // EDITOR_HPP