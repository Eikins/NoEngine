#include "Core/Systems/PhysicsSystem.h"

#include "Core/Modules/GameManager.hpp"
#include "Core/Components/AABBCollider.h"
#include "Core/Components/Rigidbody.h"

#include "Math/Math.h"

namespace Core
{
	Signature PhysicsSystem::CreateSignature()
	{
		Signature signature;
		signature.set(_gameManager->GetComponentType<AABBCollider>());
		signature.set(_gameManager->GetComponentType<Rigidbody>());
		return signature;
	}

	void PhysicsSystem::BindCollisionWorld(CollisionSystem* collisionSystem)
	{
		_collisionSystem = collisionSystem;
	}

	void PhysicsSystem::Integrate(float dt)
	{
		static const Math::Vector3 Gravity = Math::Vector3(0, -9.81, 0);
		std::vector<Collision> collisions;

		for (auto const& gameObject : _gameObjects)
		{
			auto aabb = _gameManager->GetComponent<AABBCollider>(gameObject);
			auto rb = _gameManager->GetComponent<Rigidbody>(gameObject);

			if (aabb->IsEnabled() == false || rb->IsEnabled() == false || gameObject->IsActive() == false) continue;
			
			rb->velocity += Gravity * dt;
			auto translation = rb->velocity * dt;
			
			if (_collisionSystem->CastAABB(aabb, translation, collisions) == false)
			{
				gameObject->GetTransform()->Translate(translation);
			}
			else
			{
				rb->velocity = Math::Vector3::Zero;
			}
		}
	}
}