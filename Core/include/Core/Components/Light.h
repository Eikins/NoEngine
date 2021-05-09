#pragma once

#include "Component.h"

#include "Math/Vector3.h"

namespace Core
{
	enum class LightType
	{
		DIRECTIONAL = 0
	};

	class Light : public Component
	{
	public:
		LightType _type = LightType::DIRECTIONAL;
		Math::Vector3 _color = Math::Vector3::One;
		
		Light(GameObject* gameObject) : Component(gameObject) {}
	};
}