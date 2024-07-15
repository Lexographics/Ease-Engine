#ifndef TEXT2D_HPP
#define TEXT2D_HPP
#pragma once

#include <string>

#include "sowa.hpp"

#include "node2d.hpp"

#include "data/color.hpp"

class Text2D : public Node2D {
  public:
	virtual ~Text2D() = default;

	void Update() override;
	bool Serialize(Document &doc) override;
	bool Deserialize(const Document &doc) override;

	bool Copy(Node *dst) override;
	void UpdateEditor() override;

	inline RID &GetFont() { return _font; }
	inline std::string &Text() { return _text; }
	inline Color &Modulate() { return _modulate; }

  public:
	RID _font;
	std::string _text;
	Color _modulate;
};

#endif // TEXT2D_HPP