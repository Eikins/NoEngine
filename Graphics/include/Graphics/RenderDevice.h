#pragma once

#include "SwapChain.h"
#include "Buffer.h"
#include "CommandBuffer.h"
#include "DeviceObject.h"
#include "PipelineState.h"
#include "Shader.h"
#include "Window.h"

namespace Graphics
{
	class RenderDevice : public DeviceObject
	{
	protected:
		// Our Engine don't support multiple windows atm
		Window* _window;
	public:

		virtual Window* GetWindow() = 0;
		virtual Buffer* CreateBuffer(const BufferDescriptor& descriptor, const BufferData& data) = 0;
		virtual Shader* CreateShader(const ShaderCreationDescriptor& descriptor) = 0;
		virtual SwapChain* CreateSwapChain() = 0;
		virtual PipelineState* CreatePipelineState(const PipelineStateCreationDescriptor& descriptor) = 0;
		virtual CommandBuffer* CreateCommandBuffer() = 0;
	};
}