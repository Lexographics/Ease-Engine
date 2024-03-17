#ifndef VISUAL_HPP
#define VISUAL_HPP
#pragma once

class Viewport;
class Window;

namespace Visual {
void InitState(Window *window);
void DestroyState();

bool Active();

void UseViewport(Viewport *viewport);
} // namespace Visual

#endif // VISUAL_HPP