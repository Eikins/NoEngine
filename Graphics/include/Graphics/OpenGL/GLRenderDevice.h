#pragma once

#include "../RenderDevice.h"

namespace Graphics
{
	class GLRenderDevice : public RenderDevice
	{
		virtual std::unique_ptr<Window> CreateWindow(const WindowDescriptor& descriptor) override;
		virtual std::unique_ptr<Buffer> CreateBuffer(const BufferDescriptor& descriptor, const BufferData &data) override;
		virtual std::unique_ptr<Shader> CreateShader(const ShaderCreationDescriptor& descriptor) override;
	};
}