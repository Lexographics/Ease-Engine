#include "scene.hpp"

#include <fstream>

#include "core/debug.hpp"
#include "yaml-cpp/yaml.h"

void Scene::Start() {
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
}

Node *Scene::Create(NodeTypeID type, const std::string &name, NodeID id) {
	static UUIDGenerator gen;

	Node *node = _nodeDB->Create(type);
	if (!node)
		return nullptr;

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

	YAML::Node resources;
	for (auto &[rid, res] : App().GetResourceRegistry().GetResources()) {
		if (!res || rid == 0)
			continue;

		YAML::Node resNode;

		if (res->ResourceType() == typeid(ImageTexture).hash_code()) {
			ImageTexture *texture = dynamic_cast<ImageTexture *>(res);
			if (!texture) {
				Debug::Error("Invalid resource: {}, expected ImageTexture", res->GetRID());
				continue;
			}

			resNode["Type"] = "ImageTexture";
			resNode["Path"] = texture->Filepath();
		} else if (res->ResourceType() == typeid(Font).hash_code()) {
			Font *font = dynamic_cast<Font *>(res);
			if (!font) {
				Debug::Error("Invalid resource: {}, expected Font", res->GetRID());
				continue;
			}

			resNode["Type"] = "Font";
			resNode["Path"] = font->Filepath();
		} else {
			Debug::Error("Unknown resource type for resource: {}", res->GetRID());
			continue;
		}
		resources[rid] = resNode;
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

	FolderFileSystem *fs = dynamic_cast<FolderFileSystem *>(App().FS());
	if (fs == nullptr) {
		Debug::Error("Failed to save scene: folder fs not found");
		return false;
	}
	std::filesystem::path savePath = fs->GetPath(path);

	std::ofstream fout(savePath);
	if (!fout.good()) {
		Debug::Error("Failed to open file on '{}'", path);
	}
	fout << emitter.c_str();

	Debug::Info("Scene saved to '{}'", path);
	return true;
}

bool Scene::LoadFromFile(const char *path) {
	_scenePath = path;

	FileData data = App().FS()->Load(path);
	if (!data) {
		Debug::Error("Failed to open file: '{}'", path);
		return false;
	}
	std::string str{reinterpret_cast<char *>(data->buffer.data()), data->buffer.size()};
	YAML::Node doc = YAML::Load(str);

	YAML::Node resources = doc["Resources"];
	for (YAML::const_iterator it = resources.begin(); it != resources.end(); ++it) {
		RID rid = it->first.as<RID>(0);
		YAML::Node resData = it->second;
		std::string resType = resData["Type"].as<std::string>("");
		if (resType == "ImageTexture") {
			std::string path = resData["Path"].as<std::string>("");
			if (path == "") {
				Debug::Warn("Blank texture path. skipping");
				continue;
			}

			ImageTexture *tex = new ImageTexture;
			tex->Load(path.c_str());
			App().GetResourceRegistry().AddResource(tex, rid);
			Debug::Info("Loaded texture with id: {}", rid);
		} else if (resType == "Font") {
			std::string path = resData["Path"].as<std::string>("");
			if (path == "") {
				Debug::Warn("Blank font path. skipping");
				continue;
			}

			Font *font = new Font;
			font->LoadTTF(path.c_str());
			App().GetResourceRegistry().AddResource(font, rid);
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

	return true;
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