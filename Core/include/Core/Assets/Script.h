#pragma once

#include "Asset.h"


namespace Core
{
	class GameObject;
	class Transform;

	class Script : public Asset
	{
	private:
		std::string _path;
	public:
		Script(std::string name, std::string path) : _path(path), Asset(name) {}

		inline std::string GetPath() { return _path; }
	};
}