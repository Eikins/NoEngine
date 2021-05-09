#pragma once

#include "System.h"

#include "Core/Components/AABBCollider.h"
#include "Math/Math.h"

namespace Core
{
	struct Collision
	{
		Collider* collider;
		Math::Vector3 point;
		Math::Vector3 normal;
	};

	class CollisionSystem : public System
	{
	public:
		virtual Signature CreateSignature() override;

		bool CastAABB(AABBCollider* aabb, Math::Vector3 translation, std::vector<Collision>& result);
	};
}