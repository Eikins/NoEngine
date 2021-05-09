#pragma once

#include <string>

namespace Core
{
	class Asset
	{
	protected:
		uint32_t guid = 0;
		std::string _name;
	public:
		Asset(std::string name) : _name(name) {}
		std::string GetName() const;
	};
}