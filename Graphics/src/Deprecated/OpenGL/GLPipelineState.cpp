#include "gl/glew.h"

#include "Graphics/OpenGL/GLPipelineState.h"

namespace Graphics
{
	void GLPipelineState::Bind()
	{
		glUseProgram(GLID);
	}

	void GLPipelineState::Release()
	{
		glDeleteProgram(GLID);
	}

	void* GLPipelineState::GetNativeHandle()
	{
		return &GLID;
	}
}