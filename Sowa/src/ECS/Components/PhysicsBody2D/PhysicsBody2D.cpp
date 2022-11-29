#include "ECS/Components/PhysicsBody2D/PhysicsBody2D.hpp"

namespace Sowa::Component {
PhysicsBody2D::PhysicsBody2D() {
}


b2BodyType PhysicsBody2D::GetInternalBodyType() {
	return m_BodyType == PhysicsBodyType::DYNAMIC	  ? b2_dynamicBody
		   : m_BodyType == PhysicsBodyType::KINEMATIC ? b2_kinematicBody
													  : b2_staticBody;
}
} // namespace Sowa::Component