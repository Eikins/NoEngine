#pragma once

#include <string>

namespace Core
{
	struct RenderStateBlock
	{
		depthTestEnabled = true;
		depthWrite = true;
	};

	class Shader
	{
	private:
		std::string _name;

		uint32_t _codeSize;
		void* _code;

		RenderStateBlock _renderStateBlock;
		std::string _entryPoint;
	public:
		static Shader LoadFromFile(std::string path, std::string entrypoint);
	};
}