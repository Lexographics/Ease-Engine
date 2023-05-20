#include "camera_2d.hpp"

#include "core/node_db.hpp"

namespace sowa {
Camera2D::Camera2D() {
	m_type = Typename();
	_NodeType = "Camera2D";
}
Camera2D::~Camera2D() {
}

void Camera2D::Bind() {
	NodeFactory factory;
	factory.construct = []() -> Node * {
		Node *node = Allocator<Camera2D>::Get().allocate(1);
		new (node) Camera2D;

		return node;
	};

	factory.destruct = [](Node *node) {
		Allocator<Camera2D>::Get().deallocate(reinterpret_cast<Camera2D *>(node), 1);
	};

	NodeDB::Instance().RegisterNodeType("Camera2D", "Node2D", factory);

	NodeDB::Instance().RegisterAttribute<float>("Camera2D", "zoom.x", [](Node* node) -> float {
		Camera2D* camera = dynamic_cast<Camera2D*>(node);
		if(nullptr != camera) {
			return camera->Zoom().x;
		}
		return 0.f;
	}, [](Node* node, float zoom) {
		Camera2D* camera = dynamic_cast<Camera2D*>(node);
		if(nullptr != camera) {
			camera->Zoom().x = zoom;
		}
	});

	NodeDB::Instance().RegisterAttribute<float>("Camera2D", "zoom.y", [](Node* node) -> float {
		Camera2D* camera = dynamic_cast<Camera2D*>(node);
		if(nullptr != camera) {
			return camera->Zoom().y;
		}
		return 0.f;
	}, [](Node* node, float zoom) {
		Camera2D* camera = dynamic_cast<Camera2D*>(node);
		if(nullptr != camera) {
			camera->Zoom().y = zoom;
		}
	});

	NodeDB::Instance().RegisterAttribute<bool>("Camera2D", "rotatable", [](Node* node) -> bool {
		Camera2D* camera = dynamic_cast<Camera2D*>(node);
		if(nullptr != camera) {
			return camera->Rotatable();
		}
		return 0.f;
	}, [](Node* node, bool rotatable) {
		Camera2D* camera = dynamic_cast<Camera2D*>(node);
		if(nullptr != camera) {
			camera->Rotatable() = rotatable;
		}
	});
}

FileBuffer Camera2D::SaveImpl(object_type *out) {
	Camera2D *o = reinterpret_cast<Camera2D *>(out);

	YAML::Node doc = Serializer::get_singleton().SaveWithType<Node2D>(out).Yaml();

	return FileBuffer(doc);
}

bool Camera2D::LoadImpl(object_type *out, const FileBuffer &buf) {
	Debug::Error("Camera2D::LoadImpl is not implemented");
	return false;
}
} // namespace sowa
