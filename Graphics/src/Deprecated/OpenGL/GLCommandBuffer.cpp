#include "gl/glew.h"

#include "Graphics/OpenGL/GLCommandBuffer.h"

namespace Graphics
{
	void GLCommandBuffer::Clear(bool clearColor, bool clearDepth, float r, float g, float b, float a, float depth)
	{
		glClearColor(r, g, b, a);
		GLbitfield mask = GL_NONE;
		if (clearColor)
		{
			mask |= GL_COLOR_BUFFER_BIT;
		}
		if (clearDepth)
		{
			mask |= GL_DEPTH_BUFFER_BIT;
		}
		glClear(mask);
	}

	uint32_t GLCommandBuffer::GetNativeHandle()
	{
		return 0;
	}
}