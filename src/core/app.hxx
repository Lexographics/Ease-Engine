#ifndef SW_APP_HXX
#define SW_APP_HXX
#pragma once

#include "core/error/error.hxx"
#include "core/graphics.hxx"
#include "data/project_settings.hxx"
#include "ui/ui_tree.hxx"

#include <filesystem>

class App {
  public:
	~App();

	Error Init();
	Error Run();

	inline project_settings &ProjectSettings() { return m_projectSettings; }

	void SetRenderLayer(RenderLayer *renderlayer);

  private:
	void mainLoop();
	static void mainLoopCaller(void *self);

  private:
	Model rectModel;
	Shader mainShader;

	Texture m_testTexture;

	Shader uiShader;

	Model fullscreenModel;
	Shader fullscreenShader;
	RenderLayer m_layer2D;
	RenderLayer m_layerUI;

	project_settings m_projectSettings;

	UITree m_editorTree;

	friend class FileServer;
	std::filesystem::path m_appPath = "";
};

#endif // SW_APP_HXX