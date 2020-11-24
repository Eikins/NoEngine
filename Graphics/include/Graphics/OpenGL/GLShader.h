#pragma once

#include "../Shader.h"

namespace Graphics
{
	class GLShader : public Shader
	{
	public:
		uint32_t GLID;

		virtual uint32_t GetNativeHandle() override;
	};
}