#include "gl/glew.h"

#include "Graphics/OpenGL/GLShader.h"

namespace Graphics
{
	uint32_t GLShader::GetNativeHandle()
	{
		return GLID;
	}
}