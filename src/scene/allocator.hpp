#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP
#pragma once

#include <functional>
#include <memory>

#include "node.hpp"

using NodeCreateFunc = std::function<Node *()>;
using NodeDestroyFunc = std::function<void(Node *)>;

struct NodeAllocator {
  public:
	//
	template <typename T>
	static NodeAllocator Get() {
		std::allocator<T> *alloc = new std::allocator<T>;

		static NodeAllocator allocator(
			[alloc]() -> Node * {
				T *node = alloc->allocate(1);
				new (node) T();
				return node;
			},
			[alloc](Node *node) -> void {
				node->~Node();
				alloc->deallocate(reinterpret_cast<T *>(node), 1);
			});

		return allocator;
	}

  public:
	NodeAllocator() {}
	NodeAllocator(NodeCreateFunc createFunc, NodeDestroyFunc destroyFunc)
		: _createFunc(createFunc), _destroyFunc(destroyFunc) {}

	inline Node *Create() {
		if (!_createFunc)
			return nullptr;

		return _createFunc();
	}

	inline void Destroy(Node *node) {
		if (!_destroyFunc)
			return;

		_destroyFunc(node);
	}

  private:
	NodeCreateFunc _createFunc;
	NodeDestroyFunc _destroyFunc;
};

#endif // ALLOCATOR_HPP