#pragma once

#include <set>

#include "Core/GameObject.h"

namespace Core
{
	class System
	{
		friend class SystemManager;
		friend class GameManager;
	protected:
		GameManager* _gameManager = nullptr;
		std::set<GameObject*> _gameObjects;
	public:
		virtual Signature CreateSignature() = 0;
	};
}