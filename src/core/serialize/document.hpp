#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP
#pragma once

#include "glm/glm.hpp"
#include "yaml-cpp/yaml.h"

#include "sowa.hpp"

#include "data/color.hpp"
#include "math/vector2.hpp"

namespace YAML {
template <>
struct convert<glm::ivec2> {
	static Node encode(const glm::ivec2 &rhs) {
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.SetStyle(YAML::EmitterStyle::Flow);
		return node;
	}

	static bool decode(const Node &node, glm::ivec2 &rhs) {
		if (!node.IsSequence() || node.size() != 2) {
			return false;
		}

		rhs.x = node[0].as<i32>();
		rhs.y = node[1].as<i32>();
		return true;
	}
};
} // namespace YAML

class Document {
  public:
	inline void SetInt(const char *name, i32 value) { _node[name] = value; }
	inline void SetUint(const char *name, u32 value) { _node[name] = value; }
	inline void SetU64(const char *name, u64 value) { _node[name] = value; }
	inline void SetFloat(const char *name, f32 value) { _node[name] = value; }
	inline void SetString(const char *name, const std::string &value) { _node[name] = value; }
	inline void SetVec2(const char *name, const Vector2 &value) {
		_node[name]["x"] = value.x;
		_node[name]["y"] = value.y;
	}
	inline void SetColor(const char *name, Color color) {
		_node[name]["r"] = color.r;
		_node[name]["g"] = color.g;
		_node[name]["b"] = color.b;
		_node[name]["a"] = color.a;
	}
	inline void SetDocument(const char *name, const Document &doc) { _node[name] = doc._node; }
	template <typename T>
	inline void Set(const char *name, const T &value) { _node[name] = value; }
	template <typename T>
	inline void SetVector(const char *name, const std::vector<T> &value) { _node[name] = value; }

	inline i32 GetInt(const char *name, i32 fallback) const { return _node[name].as<i32>(fallback); }
	inline u32 GetUint(const char *name, u32 fallback) const { return _node[name].as<u32>(fallback); }
	inline u64 GetU64(const char *name, u64 fallback) const { return _node[name].as<u64>(fallback); }
	inline f32 GetFloat(const char *name, f32 fallback) const { return _node[name].as<f32>(fallback); }
	inline std::string GetString(const char *name, const std::string &fallback) const { return _node[name].as<std::string>(fallback); }
	inline Vector2 GetVec2(const char *name, Vector2 callback) const {
		auto vec = _node[name];
		if (vec) {
			callback.x = vec["x"].as<float>(callback.x);
			callback.y = vec["y"].as<float>(callback.y);
		}
		return callback;
	}
	inline Color GetColor(const char *name, Color callback) const {
		auto color = _node[name];
		if (color) {
			callback.r = color["r"].as<float>(callback.r);
			callback.g = color["g"].as<float>(callback.g);
			callback.b = color["b"].as<float>(callback.b);
			callback.a = color["a"].as<float>(callback.a);
		}
		return callback;
	}
	inline Document GetDocument(const char *name) const { return Document(_node[name]); }
	template <typename T>
	inline T Get(const char *name, T fallback) const { return _node[name].as<T>(fallback); }
	template <typename T>
	inline std::vector<T> GetVector(const char *name, std::vector<T> fallback) const { return _node[name].as<std::vector<T>>(fallback); }

	Document() = default;
	Document(YAML::Node _node) : _node(_node) {}
	YAML::Node &GetYAMLNode() { return _node; }
	const YAML::Node &GetYAMLNode() const { return _node; }

  private:
	YAML::Node _node;
};

#endif // DOCUMENT_HPP