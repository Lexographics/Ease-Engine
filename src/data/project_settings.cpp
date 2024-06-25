#include "project_settings.hpp"

#include <iostream>

#include "yaml-cpp/yaml.h"

#include "core/application.hpp"

void ProjectSettings::Load() {
	Ref<FileData> file = App().FS().Load("res://project.sowa");
	if (!file || file->Size() == 0) {
		return;
	}

	std::string str{reinterpret_cast<char *>(file->Buffer().data()), file->Buffer().size()};
	YAML::Node doc = YAML::Load(str);

	name = doc["Name"].as<std::string>(name);
	author = doc["Author"].as<std::string>(author);

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
}