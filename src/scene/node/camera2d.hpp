#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP
#pragma once

#include "sowa.hpp"

#include "glm/glm.hpp"
#include "node2d.hpp"

class Camera2D : public Node2D {
  public:
	virtual ~Camera2D() = default;

	bool Serialize(Document &doc) override;
	bool Deserialize(const Document &doc) override;

	bool Copy(Node *dst) override;
	void UpdateEditor() override;

	glm::mat4 GetMatrix();
	static glm::mat4 GetBlankMatrix();

	void MakeCurrent();

	inline bool &Rotatable() { return _rotatable; }

  private:
	bool _rotatable = false;
};

#endif // CAMERA2D_HPP