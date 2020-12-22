#include "gl/glew.h"

#include "Graphics/OpenGL/GLShader.h"

namespace Graphics
{
	void GLShader::Release()
	{
		glDeleteShader(GLID);
	}

	void* GLShader::GetNativeHandle()
	{
		return &GLID;
	}
}