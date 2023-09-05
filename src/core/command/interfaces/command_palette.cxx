#include "command_palette.hxx"

#include <iostream>

#include "core/app.hxx"

CommandPaletteInterface::CommandPaletteInterface() {
	text_input = true;
	text_placeholder = L"Type a command...";

	for (const auto &command : App::GetInstance().Commands()) {
		options.push_back(command);
	}
}