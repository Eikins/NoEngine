#include "Core/Systems/CollisionSystem.h"

#include "Core/Modules/GameManager.hpp"

namespace Core
{
	Signature CollisionSystem::CreateSignature()
	{
		Signature signature;
		signature.set(_gameManager->GetComponentType<AABBCollider>());
		return signature;
	}

	bool CollisionSystem::CastAABB(AABBCollider* aabb, Math::Vector3 translation, std::vector<Collision>& result)
	{
		auto sourceGameObject = aabb->GetGameObject();
		auto worldBounds = aabb->GetWorldBounds().Translate(translation);
		result.clear();
		for (auto gameObject : _gameObjects)
		{
			// Do not check collision with itself
			if (gameObject == sourceGameObject || gameObject->IsActive() == false) continue;

			auto aabb2 = _gameManager->GetComponent<AABBCollider>(gameObject);
			if (aabb2->IsEnabled() == false) continue;
			if (worldBounds.Overlaps(aabb2->GetWorldBounds()))
			{
				result.push_back({ aabb2, Math::Vector3::Zero, Math::Vector3::Zero });
			}
		}
		return result.size() > 0;
	}
}