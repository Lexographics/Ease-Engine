#ifndef _E_SCENE_HPP__
#define _E_SCENE_HPP__

#include "sowa.hpp"
#include "stlpch.hpp"

#include "node.hpp"
#include "object_type.hpp"
#include "utils/memory.hpp"
#include "utils/random.hpp"

namespace sowa {

class Scene : public object_type {
  public:
	~Scene();

	static std::string Typename() { return "sowa::Scene"; }

	void Enter();
	void Exit();

	void PreUpdate();
	void UpdateLogic();
	void UpdateDraw();

	void Load(const std::string& path);

	Node *Create(const std::string &typeName, const std::string &name, uint32_t id = 0);

	Node *GetNodeByID(uint32_t id);

	static Reference<Scene> New();

	void Register(Node *node);

	void SetPause(bool v) { _Paused = v; }
	bool IsPaused() { return _Paused; }

	inline void SetRoot(Node *node) {
		if (_Root != nullptr)
			_Root->SetParent(nullptr);
		
		_Root = node;
		if (_Root != nullptr)
			_Root->SetParent(nullptr);
	}
	inline Node *GetRoot() { return _Root; }

	// Deallocates parentless entities
	void CollectNodes();

	static FileBuffer SaveImpl(object_type *out);
	static bool LoadImpl(object_type *out, const FileBuffer &buf);

	inline const uint32_t GetCurrentCamera2D() const { return m_CurrentCamera2D; }
	void SetCurrentCamera2D(uint32_t id) { m_CurrentCamera2D = id; }

  private:
	Scene();
	bool _Paused{false};
	uint32_t m_CurrentCamera2D = 0;

	std::vector<Node *> _RegisteredNodes{};

	std::weak_ptr<Scene> _SelfRef{};
	Node *_Root{nullptr};
};
} // namespace sowa

#endif // _E_SCENE_HPP__