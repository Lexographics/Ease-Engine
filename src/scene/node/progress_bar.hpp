#ifndef PROGRESS_BAR_HPP
#define PROGRESS_BAR_HPP
#pragma once

#include "sowa.hpp"

#include "data/color.hpp"
#include "node2d.hpp"

class ProgressBar : public Node2D {
	EDITOR_NODE()
  public:
	virtual ~ProgressBar() = default;

	void Update() override;
	bool Serialize(Document &doc) override;
	bool Deserialize(const Document &doc) override;

	bool Copy(Node *dst) override;

  public:
	float _minValue = 0.f;
	float _maxValue = 100.f;
	float _value = 100.f;

	Vector2 _size = Vector2(300.f, 60.f);
	float _padding = 2.f;

	Color _foregroundColor = Color(1.f, 1.f, 1.f, 1.f);
	Color _backgroundColor = Color(0.f, 0.f, 0.f, 1.f);
};

#endif // PROGRESS_BAR_HPP