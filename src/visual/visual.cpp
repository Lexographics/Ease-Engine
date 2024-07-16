#include "visual.hpp"

#include "visual/gl.hpp"

#include "shader.hpp"
#include "viewport.hpp"
#include "window.hpp"

struct VisualState {
	bool active = true;
	Window *window = nullptr;
};
static VisualState state;

void Visual::InitState(Window *window) {
	state.window = window;
}

void Visual::DestroyState() {
	state.active = false;
	// glfwTerminate();
}

bool Visual::Active() {
	return state.active;
}

void Visual::UseViewport(Viewport *viewport) {
	if (viewport)
		return viewport->Bind();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Vector2 windowSize = state.window->GetWindowSize();

	float windowRatio = (float)windowSize.x / windowSize.y;
	float videoRatio = (float)1920.f / 1080.f;

	if (windowRatio > videoRatio) {
		float width = windowSize.y * videoRatio;
		float height = windowSize.y;
		float gap = windowSize.x - width;

		float x = gap / 2.f;
		glViewport(x, 0.f, width, height);
		// _viewportRect = rect(x, 0.f, width, height);

	} else {
		float width = windowSize.x;
		float height = windowSize.x / videoRatio;
		float gap = windowSize.y - height;

		float y = gap / 2.f;
		glViewport(0.f, y, width, height);
		// _viewportRect = rect(0.f, y, width, height);
	}
}