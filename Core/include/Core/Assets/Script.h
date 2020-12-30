#pragma once

#include "Asset.h"


namespace Core
{
	class GameObject;
	class Transform;

	class Script : public Asset
	{
	public:
		Script(std::string name) : Asset(name) {}

		virtual void Initialize(GameObject* gameObject, Transform* transform) = 0;
		virtual void OnUpdate(GameObject* gameObject, Transform* transform) = 0;

#ifdef NoEngine_Editor
		virtual void DrawExposedProperties() = 0;
#endif
	};
}