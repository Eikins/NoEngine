#pragma once

#include "Graphics/RenderDevice.h"

namespace Graphics
{
	class GLRenderDevice : public RenderDevice
	{
	public:
		virtual Window* GetWindow() override;
		virtual Buffer* CreateBuffer(const BufferDescriptor& descriptor, const BufferData &data) override;
		virtual Shader* CreateShader(const ShaderCreationDescriptor& descriptor) override;
		virtual SwapChain* CreateSwapChain() override;
		virtual PipelineState* CreatePipelineState(const PipelineStateCreationDescriptor& descriptor) override;
		virtual CommandBuffer* CreateCommandBuffer() override;

		virtual void Release() override;
		virtual void* GetNativeHandle() override;

		static RenderDevice* CreateDevice(const WindowDescriptor& descriptor);
	};
}