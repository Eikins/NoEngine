#pragma once

#include <string>

#include "DeviceObject.h"

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


	class Shader : public DeviceObject
	{
	protected:
		ShaderDescriptor _descriptor;
	public:
		ShaderDescriptor GetDescriptor() { return _descriptor; }
	};
}