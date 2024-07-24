#include "node.hpp"

#include "core/application.hpp"

#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"

void Node::RemoveChild(Node *child) {
	child->_parent = nullptr;
	removeChild(child);
}

bool Node::Serialize(Document &doc) {
	doc.SetString("Type", App().GetNodeDB().GetNodeTypename(TypeID()));
	doc.SetString("Name", Name());
	doc.SetU64("ID", ID());
	doc.Set("Groups", _groups);

	return true;
}

bool Node::Deserialize(const Document &doc) {
	Rename(doc.GetString("Name", Name()));
	_id = doc.GetU64("ID", ID());
	_groups = doc.Get("Groups", _groups);

	return true;
}

bool Node::Copy(Node *dst) {
	dst->Rename(Name());
	dst->_groups = _groups;
	return true;
}

void Node::AddChild(Node *child) {
	if (child->_parent != nullptr) {
		child->_parent->removeChild(child);
	}
	child->_parent = this;
	_children.push_back(child);
}

void Node::Free() {
	App().GetCurrentScene()->FreeNode(ID());
}

size_t Node::GetChildCount() {
	return _children.size();
}

Node *Node::GetChild(size_t index) {
	if (index >= _children.size())
		return nullptr;
	return _children[index];
}

Node *Node::GetNode(const std::string nodePath, bool recursive) {
	std::filesystem::path path(nodePath);

	if (recursive) {
		auto nodePaths = Utils::Split(nodePath, "/");
		Node *node = this; // todo: if string starts with /, start it from root node
		for (auto path : nodePaths) {
			node = node->GetNode(path, false);
			if (nullptr == node)
				break;
		}
		return node;
	}

	for (Node *child : _children) {
		if (child->Name() == nodePath)
			return child;
	}
	return nullptr;
}

Node *Node::Duplicate(Scene *scene /*= nullptr*/) {
	if (!scene)
		scene = App().GetCurrentScene().get();
	if (!scene)
		return nullptr;

	Node *node = scene->Create(TypeID(), Name(), ID());
	Copy(node);

	for (Node *child : GetChildren()) {
		node->AddChild(child->Duplicate(scene));
	}

	return node;
}

bool Node::IsHovered() {
	return ID() == App().GetHoveredID();
}

void Node::removeChild(Node *child) {
	_children.erase(std::remove(_children.begin(), _children.end(), child), _children.end());
}

EDITOR_UPDATE_FUNC(Node, {
	if (ImGui::CollapsingHeader("Node", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Indent();

		if (ImGui::CollapsingHeader("Groups", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::Indent();
			for (size_t i = 0; i < _groups.size();) {
				ImGui::PushID((void *)&_groups[i]);

				ImGui::Text("%s", _groups[i].c_str());
				ImGui::SameLine();
				if (ImGui::Button("x", ImVec2(24, 24))) {
					RemoveGroup(_groups[i]);
				} else {
					i++;
				}

				ImGui::PopID();
			}

			if (ImGui::Button("+", ImVec2(24, 24))) {
				ImGui::OpenPopup("Node_Groups_Add");
			}

			if (ImGui::BeginPopup("Node_Groups_Add")) {
				std::string buf = "";
				ImGui::Text("Group");
				ImGui::SameLine();

				if (ImGui::InputText("##Group", &buf, ImGuiInputTextFlags_EnterReturnsTrue)) {
					AddGroup(buf);
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			ImGui::Unindent();
		}

		ImGui::Unindent();
	}
})