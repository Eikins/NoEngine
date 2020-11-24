#pragma once

#include "SwapChain.h"
#include "Buffer.h"
#include "Window.h"

namespace Graphics
{
	class RenderDevice
	{
	public:
		//virtual SwapChain CreateSwapChain() = 0;
		virtual std::unique_ptr<Window> CreateWindow(const WindowDescriptor& descriptor) = 0;

		virtual std::unique_ptr<Buffer> CreateBuffer(const BufferDescriptor& descriptor, const BufferData& data) = 0;
	};
}