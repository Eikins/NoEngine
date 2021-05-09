#pragma once

#include "Component.h"

namespace Core
{
	class Collider : public Component
	{
	protected:
		Collider(GameObject* gameObject) : Component(gameObject) {}
	};
}