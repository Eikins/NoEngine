#pragma once

#include <string>

namespace Graphics
{

	enum class ShaderType
	{
		UNSPECIFIED,
		VERTEX,
		PIXEL,
		COMPUTE
	};

	enum class ShaderCompilerType
	{
		DEFAULT
	};


	struct ShaderDescriptor
	{
		ShaderType shaderType;
	};

	struct ShaderCreationDescriptor
	{
		std::string filename;
		ShaderCompilerType compilerType;
		ShaderDescriptor* pDesc;
	};


	class Shader
	{
	public:
		virtual uint32_t GetNativeHandle() = 0;
	};
}