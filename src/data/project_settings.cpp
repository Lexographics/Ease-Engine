#include "project_settings.hpp"

#include <fstream>
#include <iostream>

#include "yaml-cpp/yaml.h"

#include "core/application.hpp"
#include "core/debug.hpp"

void ProjectSettings::Load() {
	Ref<FileData> file = App().FS().Load("res://project.sowa");
	if (!file || file->Size() == 0) {
		return;
	}

	std::string str{reinterpret_cast<char *>(file->Buffer().data()), file->Buffer().size()};
	YAML::Node doc = YAML::Load(str);

	if (auto application = doc["Application"]; application) {
		this->application.name = application["Name"].as<std::string>(this->application.name);
		this->application.author = application["Author"].as<std::string>(this->application.author);
		this->application.mainScene = application["MainScene"].as<std::string>(this->application.mainScene);
	}

	if (auto rendering = doc["Rendering"]; rendering) {
		if (auto window = rendering["Window"]; window) {
			this->rendering.window.width = window["Width"].as<int>(this->rendering.window.width);
			this->rendering.window.height = window["Height"].as<int>(this->rendering.window.height);
		}

		if (auto viewport = rendering["Viewport"]; viewport) {
			this->rendering.viewport.width = viewport["Width"].as<int>(this->rendering.viewport.width);
			this->rendering.viewport.height = viewport["Height"].as<int>(this->rendering.viewport.height);
		}
	}

	auto store = doc["GlobalStore"].as<std::unordered_map<std::string, std::string>>(std::unordered_map<std::string, std::string>());
	for (auto &[key, value] : store) {
		App().GetGlobalStore().Set(key, value);
	}
}

bool ProjectSettings::Save() {
	YAML::Node out;

	YAML::Node applicationNode;
	applicationNode["Name"] = application.name;
	applicationNode["Author"] = application.author;
	applicationNode["MainScene"] = application.mainScene;

	YAML::Node windowNode;
	windowNode["Width"] = rendering.window.width;
	windowNode["Height"] = rendering.window.height;

	YAML::Node viewportNode;
	viewportNode["Width"] = rendering.viewport.width;
	viewportNode["Height"] = rendering.viewport.height;

	YAML::Node renderingNode;
	renderingNode["Window"] = windowNode;
	renderingNode["Viewport"] = viewportNode;

	out["Application"] = applicationNode;
	out["Rendering"] = renderingNode;
	out["GlobalStore"] = App().GetGlobalStore().GetStore();

	YAML::Emitter emitter;
	emitter << out;

	SaveableFileServer *fs = dynamic_cast<SaveableFileServer *>(App().FS().GetFileServer("res"));
	if (fs == nullptr) {
		Debug::Error("Failed to save scene: folder fs not found");
		return false;
	}

	std::ofstream fout;
	fs->GetSaveStream("res://project.sowa", fout);

	if (!fout.good()) {
		Debug::Error("Failed to save project settings");
		return false;
	}
	fout << emitter.c_str();

	Debug::Info("Project saved");
	return true;
}