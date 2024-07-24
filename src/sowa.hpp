#ifndef SOWA_HPP
#define SOWA_HPP
#pragma once

#include <cstdint>
#include <memory>

#ifndef SW_EDITOR
#define EDITOR_NODE()
#define EDITOR_NODE_BASE()
#define EDITOR_UPDATE_FUNC(type, body)
#endif

#ifdef SW_EDITOR
#define EDITOR_NODE()             \
  protected:                      \
	friend class Editor;          \
	void UpdateEditor() override; \
                                  \
  private:

#define EDITOR_NODE_BASE()       \
  protected:                     \
	friend class Editor;         \
	virtual void UpdateEditor(); \
                                 \
  private:

#define EDITOR_UPDATE_FUNC(type, body) void type::UpdateEditor() body
#endif

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

using NodeID = u32;
using TypeID = u64;
using NodeTypeID = u64;
using RID = i32;

extern const char *s_gitHash;
extern const char *s_gitBranch;

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T>
Ref<T> MakeRef() {
	return std::make_shared<T>();
}

#endif // SOWA_HPP