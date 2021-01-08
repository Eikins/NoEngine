#pragma once

#include "Component.h"
#include "Collider.h"
#include "Math/Math.h"

#ifdef NoEngine_Editor
class Editor::ComponentEditors;
#endif

namespace Core
{
	class AABBCollider : public Collider
	{
#ifdef NoEngine_Editor
		friend class Editor::ComponentEditors;
#endif
	public:
		Math::Bounds bounds;

		AABBCollider(GameObject* gameObject) : Collider(gameObject) {}

		inline Math::Bounds GetWorldBounds() 
		{
			Transform& transform = *GetTransform();
			auto lossyScale = transform.GetLossyScale();
			auto center = transform.GetWorldPosition() + lossyScale * bounds.Center();
			auto size = lossyScale * bounds.Size();
			return Math::Bounds(center, size);
		}
	};
}