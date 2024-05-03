#ifndef DEBUG_FMT_HPP
#define DEBUG_FMT_HPP
#pragma once

#include <fmt/format.h>

#include "core/application.hpp"
#include "data/color.hpp"
#include "resource/image_texture.hpp"
#include "scene/node.hpp"

template <>
struct fmt::formatter<Color> {
  public:
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
		return ctx.begin();
	}

	template <typename Context>
	auto format(const Color &value, Context &ctx) const {
		return format_to(ctx.out(), "Color({}, {}, {}, {})", (u8)(value.r * 255), (u8)(value.g * 255), (u8)(value.b * 255), (u8)(value.a * 255));
	}
};

template <>
struct fmt::formatter<ImageTexture> {
  public:
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
		return ctx.begin();
	}

	template <typename Context>
	auto format(const ImageTexture &value, Context &ctx) const {
		return format_to(ctx.out(), "ImageTexture(ID: {}, RID: {})", value.ID(), value.GetRID());
	}
};

template <>
struct fmt::formatter<Node> {
  public:
	constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
		return ctx.begin();
	}

	template <typename Context>
	auto format(const Node &value, Context &ctx) const {
		return format_to(ctx.out(), "Node(Name: \"{}\", Type: {})", value.Name(), App().GetNodeDB().GetNodeTypename(value.TypeID()));
	}
};

#endif // DEBUG_FMT_HPP