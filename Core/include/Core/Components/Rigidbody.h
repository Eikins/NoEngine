#pragma once

#include "Component.h"

namespace Core
{
	class Rigidbody : public Component
	{
	public:
		Math::Vector3 acceleration = Math::Vector3::Zero;
		Math::Vector3 velocity = Math::Vector3::Zero;
	public:
		Rigidbody(GameObject* gameObject) : Component(gameObject) {}
	};
}