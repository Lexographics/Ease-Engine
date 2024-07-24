#include "application.hpp"

#include "res/shaders/sprite2d.fs.res.inc"
#include "res/shaders/sprite2d.vs.res.inc"

#include "res/shaders/text2d.fs.res.inc"
#include "res/shaders/text2d.vs.res.inc"

#include "res/shaders/fullscreen.fs.res.inc"
#include "res/shaders/fullscreen.vs.res.inc"

#include "res/shaders/default3d.fs.res.inc"
#include "res/shaders/default3d.vs.res.inc"

#include "res/Roboto-Regular.ttf.res.inc"
#include "res/icon.png.res.inc"
#include "res/imgui.ini.res.inc"

void Application::RegisterBuiltinData() {
	DataFileServer *dataFS = _fs.NewDataFileServer();
	_fs.RegisterFileServer("data", dataFS);

	dataFS->AddFile("font.ttf", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_Roboto_Regular_ttf_res_inc_data), src_res_Roboto_Regular_ttf_res_inc_size));
	dataFS->AddFile("imgui.ini", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_imgui_ini_res_inc_data), src_res_imgui_ini_res_inc_size));
	dataFS->AddFile("icon.png", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_icon_png_res_inc_data), src_res_icon_png_res_inc_size));

	dataFS->AddFile("sprite2d.vs", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_shaders_sprite2d_vs_res_inc_data), src_res_shaders_sprite2d_vs_res_inc_size));
	dataFS->AddFile("sprite2d.fs", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_shaders_sprite2d_fs_res_inc_data), src_res_shaders_sprite2d_fs_res_inc_size));

	dataFS->AddFile("text2d.vs", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_shaders_text2d_vs_res_inc_data), src_res_shaders_text2d_vs_res_inc_size));
	dataFS->AddFile("text2d.fs", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_shaders_text2d_fs_res_inc_data), src_res_shaders_text2d_fs_res_inc_size));

	dataFS->AddFile("fullscreen.vs", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_shaders_fullscreen_vs_res_inc_data), src_res_shaders_fullscreen_vs_res_inc_size));
	dataFS->AddFile("fullscreen.fs", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_shaders_fullscreen_fs_res_inc_data), src_res_shaders_fullscreen_fs_res_inc_size));

	dataFS->AddFile("default3d.vs", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_shaders_default3d_vs_res_inc_data), src_res_shaders_default3d_vs_res_inc_size));
	dataFS->AddFile("default3d.fs", FileData::NewStatic(reinterpret_cast<std::byte *>(src_res_shaders_default3d_fs_res_inc_data), src_res_shaders_default3d_fs_res_inc_size));
}