#pragma once

#include <string>
#include <vector>

#include "Asset.h"

namespace Core
{
	class Shader : public Asset
	{
	private:
		uint32_t _codeSize = 0;
		std::vector<char> _code;

		std::string _entryPoint = "";

		Shader(std::string name) : Asset(name) {}
	public:
		static Shader LoadFromFile(
			std::string name, 
			std::string path, 
			std::string entrypoint
		);

		std::string GetEntryPoint() const;
		std::vector<char> GetCompiledCode() const;
	};
}