#include "gl/glew.h"

#include "Graphics/OpenGL/GLRenderDevice.h"
#include "Graphics/OpenGL/GLBuffer.h"

namespace Graphics
{
	Buffer GLRenderDevice::CreateBuffer(BufferDescriptor descriptor, BufferData data)
	{
		GLBuffer buffer;
		glGenBuffers(1, &buffer.GLID);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.GLID);
		glBufferData(GL_ARRAY_BUFFER, data.size, data.data, GL_STATIC_DRAW);
	}
}