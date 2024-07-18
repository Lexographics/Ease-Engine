#ifndef VISUAL_HPP
#define VISUAL_HPP
#pragma once

class Viewport;
class Window;
class Rect;

namespace Visual {
void InitState(Window *window);
void DestroyState();

bool Active();

void UseViewport(Viewport *viewport, Rect* calculatedRect = nullptr);
} // namespace Visual

#endif // VISUAL_HPP