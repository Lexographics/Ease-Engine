#ifndef GUI_HPP
#define GUI_HPP
#pragma once

#include "core/resource.hpp"

namespace Gui {
bool TexturePicker(const char *id, RID &rid);
void AnimationInput(const char *id, RID &rid);
} // namespace Gui

#endif // GUI_HPP