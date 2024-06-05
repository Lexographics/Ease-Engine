#ifndef DOCUMENT_HPP
#define DOCUMENT_HPP
#pragma once

#include "glm/glm.hpp"
#include "yaml-cpp/yaml.h"

#include "sowa.hpp"

class Document {
  public:
	inline void SetInt(const char *name, i32 value) { _node[name] = value; }
	inline void SetUint(const char *name, u32 value) { _node[name] = value; }
	inline void SetU64(const char *name, u64 value) { _node[name] = value; }
	inline void SetFloat(const char *name, f32 value) { _node[name] = value; }
	inline void SetString(const char *name, const std::string &value) { _node[name] = value; }
	inline void SetVec2(const char *name, const glm::vec2 &value) {
		_node[name]["x"] = value.x;
		_node[name]["y"] = value.y;
	}
	inline void SetDocument(const char *name, const Document &doc) { _node[name] = doc._node; }

	inline i32 GetInt(const char *name, i32 fallback) const { return _node[name].as<i32>(fallback); }
	inline u32 GetUint(const char *name, u32 fallback) const { return _node[name].as<u32>(fallback); }
	inline u64 GetU64(const char *name, u64 fallback) const { return _node[name].as<u64>(fallback); }
	inline i32 GetFloat(const char *name, f32 fallback) const { return _node[name].as<f32>(fallback); }
	inline std::string GetString(const char *name, const std::string &fallback) const { return _node[name].as<std::string>(fallback); }
	inline glm::vec2 GetVec2(const char *name, glm::vec2 callback) const {
		auto vec = _node[name];
		if (vec) {
			callback.x = vec["x"].as<float>(callback.x);
			callback.y = vec["y"].as<float>(callback.y);
		}
		return callback;
	}
	inline Document GetDocument(const char *name) const { return Document(_node[name]); }

	Document() = default;
	Document(YAML::Node _node) : _node(_node) {}
	YAML::Node &GetYAMLNode() { return _node; }
	const YAML::Node &GetYAMLNode() const { return _node; }

  private:
	YAML::Node _node;
};

#endif // DOCUMENT_HPP