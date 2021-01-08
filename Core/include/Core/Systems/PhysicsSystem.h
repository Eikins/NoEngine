#pragma once

#include "Core/Systems/System.h"
#include "Core/Systems/CollisionSystem.h"

namespace Core
{
	class PhysicsSystem : public System
	{
	private:
		CollisionSystem* _collisionSystem;
	public:
		virtual Signature CreateSignature() override;

		void BindCollisionWorld(CollisionSystem* collisionSystem);

		void Integrate(float dt);
	};
}