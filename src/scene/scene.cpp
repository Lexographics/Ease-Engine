#include "scene.hpp"

#include <fstream>

#include "core/debug.hpp"
#include "yaml-cpp/yaml.h"

#include "core/application.hpp"
#include "resource/sprite_sheet_animation.hpp"
#include "scene/node/camera2d.hpp"

Scene::~Scene() {
	Clear();
}

void Scene::Start() {
	for (Node *node : _nodeIter) {
		node->Start();
		node->LoadScripts();
	}
}

void Scene::Update() {
	for (NodeID id : _freeList) {
		freeNode(id);
	}
	_freeList.clear();

	for (Node *node : _nodeIter) {
		node->Update();
	}
}
void Scene::Shutdown() {
	for (Node *node : _nodeIter) {
		node->Exit();
	}
}

Node *Scene::Create(NodeTypeID type, const std::string &name, NodeID id) {
	static UUIDGenerator gen;

	Node *node = _nodeDB->Create(type);
	if (!node)
		return nullptr;
	node->_pScene = this;

	NodeID nodeId = (id != 0 && !HasNode(id)) ? id : gen.Next();
	node->_id = nodeId;
	node->Rename(name);

	_nodes[nodeId] = node;
	_nodeIter.insert(node);
	return node;
}

Node *Scene::Create(const char *typeName, const std::string &name, NodeID id) {
	return Create(_nodeDB->GetNodeTypeID(typeName), name, id);
}

bool Scene::HasNode(NodeID id) {
	return _nodes.find(id) != _nodes.end();
}

Node *Scene::GetNode(NodeID id) {
	if (!HasNode(id))
		return nullptr;
	return _nodes[id];
}

void Scene::SetRoot(Node *node) {
	_root = node;
}

Node *Scene::GetRoot() {
	return _root;
}

void Scene::SetCurrentCamera2D(NodeID id) {
	_currentCamera2D = id;
}

NodeID Scene::GetCurrentCamera2D() {
	return _currentCamera2D;
}

glm::mat4 Scene::GetMatrix2D() {
	if (_currentCamera2D == 0)
		return Camera2D::GetBlankMatrix();

	Camera2D *camera = dynamic_cast<Camera2D *>(GetNode(_currentCamera2D));
	if (!camera)
		return Camera2D::GetBlankMatrix();

	return camera->GetMatrix();
}

void Scene::FreeNode(NodeID id) {
	_freeList.push_back(id);
}

const std::filesystem::path &Scene::GetFilepath() {
	return _scenePath;
}

bool Scene::SaveToFile(const char *path) {
	if (path)
		_scenePath = path;
	else
		path = _scenePath.c_str();

	if (!path) {
		Debug::Error("Failed to save scene. invalid path");
		return false;
	}

	YAML::Node out;

	YAML::Node scene;
	scene["Camera2D"] = GetCurrentCamera2D();
	out["Scene"] = scene;

	YAML::Node resources;
	for (const auto &rid : GetResourceLocker().GetResources()) {
		Resource *res = App().GetResourceRegistry().GetResource(rid);
		if (!res || rid == 0)
			continue;

		Document resNode;
		resNode.Set("Type", App().GetResourceRegistry().GetTypeName(res->ResourceType()));
		res->SaveResource(resNode);

		resources[rid] = resNode.GetYAMLNode();
	}
	out["Resources"] = resources;

	std::function<YAML::Node(Node *)> saveNode;
	saveNode = [&saveNode](Node *node) -> YAML::Node {
		Document nodeDoc;
		node->Serialize(nodeDoc);

		YAML::Node children;
		for (Node *child : node->GetChildren()) {
			children.push_back(saveNode(child));
		}
		if (children.size() > 0)
			nodeDoc.SetDocument("Children", children);

		return nodeDoc.GetYAMLNode();
	};

	out["Root"] = saveNode(GetRoot());

	YAML::Emitter emitter;
	emitter << out;

	SaveableFileServer *fs = dynamic_cast<SaveableFileServer *>(App().FS().GetFileServer("res"));
	if (fs == nullptr) {
		Debug::Error("Failed to save scene: folder fs not found");
		return false;
	}

	std::ofstream fout;
	fs->GetSaveStream(path, fout);

	if (!fout.good()) {
		Debug::Error("Failed to open file on '{}'", path);
		return false;
	}
	fout << emitter.c_str();

	Debug::Info("Scene saved to '{}'", path);
	return true;
}

bool Scene::LoadFromFile(const char *path) {
	Clear();
	_scenePath = path;

	Ref<FileData> data = App().FS().Load(path);
	if (!data) {
		Debug::Error("Failed to open scene file: '{}'", path);
		return false;
	}
	std::string str{reinterpret_cast<char *>(data->Data()), data->Size()};
	YAML::Node doc = YAML::Load(str);

	YAML::Node resources = doc["Resources"];
	for (YAML::const_iterator it = resources.begin(); it != resources.end(); ++it) {
		RID rid = it->first.as<RID>(0);
		YAML::Node resData = it->second;
		std::string resType = resData["Type"].as<std::string>("");

		Resource *res = GetResourceLocker().CreateResource(resType.c_str(), rid);
		if (res) {
			res->LoadResource(resData);
		} else {
			Debug::Error("Failed to load resource: {}", rid);
		}
	}

	std::function<Node *(YAML::Node)> loadNode;

	loadNode = [this, &loadNode](YAML::Node node) {
		std::string type = node["Type"].as<std::string>("Node");
		std::string name = node["Name"].as<std::string>("New Node");
		NodeID nodeId = node["ID"].as<NodeID>(0);

		Node *n = this->Create(type.c_str(), name, nodeId);
		Document doc(node);
		n->Deserialize(doc);

		YAML::Node children = node["Children"];
		for (YAML::const_iterator it = children.begin(); it != children.end(); ++it) {
			n->AddChild(loadNode(*it));
		}

		return n;
	};

	YAML::Node root = doc["Root"];
	SetRoot(loadNode(root));

	YAML::Node scene = doc["Scene"];
	if (scene) {
		SetCurrentCamera2D(scene["Camera2D"].as<NodeID>(GetCurrentCamera2D()));
	}

	return true;
}

void Scene::Clear() {
	if (GetRoot())
		freeNode(GetRoot()->ID());

	_nodes.clear();
	_nodeIter.clear();
	_resourceLocker.Clear();
}

// static
void Scene::Copy(Scene *src, Scene *dst) {
	dst->Clear();

	dst->SetRoot(src->GetRoot()->Duplicate(dst));
	dst->SetCurrentCamera2D(src->GetCurrentCamera2D());
	dst->_scenePath = src->_scenePath;
	dst->_resourceLocker = src->_resourceLocker;
}

void Scene::freeNode(NodeID id) {
	Node *node = GetNode(id);
	if (!node)
		return;

	if (node == _root) {
		_root = nullptr;
	}

	if (Node *parent = node->GetParent(); nullptr != parent) {
		parent->RemoveChild(node);
	}
	for (Node *child : node->GetChildren()) {
		child->_parent = nullptr;
	}

	for (size_t i = 0; i < node->GetChildren().size(); i++) {
		freeNode(node->GetChildren()[i]->ID());
	}

	_nodeDB->Destroy(node);
	_nodes.erase(id);
	_nodeIter.erase(node);
}