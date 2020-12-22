#include "gl/glew.h"

#include "Graphics/OpenGL/GLBuffer.h"

namespace Graphics
{
	uint32_t GLBuffer::GetNativeHandle()
	{
		return GLID;
	}
}