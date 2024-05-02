#ifndef TEXT2D_HPP
#define TEXT2D_HPP
#pragma once

#include <string>

#include "sowa.hpp"

#include "node2d.hpp"

class Text2D : public Node2D {
  public:
	virtual ~Text2D() = default;

	void Update() override;

	inline RID &GetFont() { return _font; }
	inline std::string &Text() { return _text; }

  private:
	RID _font;
	std::string _text;
};

#endif // TEXT2D_HPP