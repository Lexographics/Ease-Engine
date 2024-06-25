#include "viewport.hpp"

#include "visual/gl.hpp"
#include <cstddef>
#include <vector>

Viewport::Viewport() {
	SetTarget(0, ViewportTargetType::Vec4);
	SetTarget(1, ViewportTargetType::Int);
}

Viewport::~Viewport() {
	Delete();
}

void Viewport::Create(int width, int height) {
	Delete();

	_width = width;
	_height = height;

	glGenFramebuffers(1, &_id);
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (auto &[slot, target] : _targets) {
		glGenTextures(1, &target.textureID);
		glBindTexture(GL_TEXTURE_2D, target.textureID);

		if (target.type == ViewportTargetType::Vec4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_FLOAT, NULL);
		} else if (target.type == ViewportTargetType::Int) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, _width, _height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + slot, GL_TEXTURE_2D, target.textureID, 0);
	}

	glGenRenderbuffers(1, &_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _rbo);

	std::vector<GLenum> attachments;
	for (auto &[slot, target] : _targets) {
		attachments.push_back(GL_COLOR_ATTACHMENT0 + slot);
	}
	glDrawBuffers(attachments.size(), attachments.data());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Viewport::Resize(int width, int height) {
	Delete();
	Create(width, height);
}

void Viewport::Delete() {
	if (_id != 0) {
		glDeleteFramebuffers(1, &_id);
		_id = 0;
	}

	for (auto &[slot, target] : _targets) {
		glDeleteTextures(1, &target.textureID);
		target.textureID = 0;
	}
}

void Viewport::SetTarget(uint32_t slot, ViewportTargetType type) {
	_targets[slot].type = type;
	_targets[slot].textureID = 0;
}

void Viewport::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, _id);
	glViewport(0, 0, _width, _height);
}

void Viewport::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Viewport::Clear(float r, float g, float b, float a /*= 1.0*/, bool depth /*= false*/) {
	// TODO: Designed as if all viewports have (1: RGBA, 2: uint) attachments. Needs update after change
	glClearColor(r, g, b, a);

	GLenum attachments[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(1, attachments);

	glClear(
		depth ? GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
			  : GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, _targets[1].textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, _width, _height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);

	glDrawBuffers(2, attachments);
}

int Viewport::GetTargetTextureID(int slot) {
	if (_targets.find(slot) != _targets.end()) {
		return _targets[slot].textureID;
	}
	return 0;
}

int Viewport::ReadAttachmentInt(int slot, int x, int y) {
	int color = 0;
	glReadBuffer(GL_COLOR_ATTACHMENT0 + slot);
	glReadPixels(x, _height - y, 1, 1, GL_RED_INTEGER, GL_INT, &color);
	return color;
}