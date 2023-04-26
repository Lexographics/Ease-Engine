#ifndef _E_SCENE_HPP__
#define _E_SCENE_HPP__

#include "sowa.hpp"
#include "stlpch.hpp"

#include "node.hpp"
#include "utils/memory.hpp"
#include "object_type.hpp"

namespace sowa {

class Scene : public object_type {
  public:
	~Scene();

	static std::string Typename() { return "sowa::Scene"; }

	void Enter();
	void Exit();

	void UpdateLogic();
	void UpdateDraw();

	template <typename T>
	T *Create(const std::string &name) {
		std::allocator<T> allocator = Allocator<T>::Get();
		T *node = allocator.allocate(1);
		new (node) T;
		node->_pScene = _SelfRef;
		node->Name() = name;
		Register(node);

		return node;
	}

	static Reference<Scene> New();

	void Register(Node *node);

	void SetPause(bool v) { _Paused = v; }
	bool IsPaused() { return _Paused; }

	inline void SetRoot(Node *node) {
		_Root = node;
		if (_Root != nullptr)
			_Root->SetParent(nullptr);
	}
	inline Node *GetRoot() { return _Root; }

	// Deallocates parentless entities
	void CollectNodes();

	static FileBuffer SaveImpl(object_type* out);
	static bool LoadImpl(object_type* out, const FileBuffer& buf);

  private:
	Scene();
	bool _Paused{false};

	std::vector<Node *> _RegisteredNodes{};

	std::weak_ptr<Scene> _SelfRef{};
	Node *_Root{nullptr};
};
} // namespace sowa

#endif // _E_SCENE_HPP__