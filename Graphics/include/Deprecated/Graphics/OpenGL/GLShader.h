#pragma once

#include "Graphics/Shader.h"

namespace Graphics
{
	class GLShader : public Shader
	{
	public:
		uint32_t GLID;

		virtual void Release() override;
		virtual void* GetNativeHandle() override;
	};
}