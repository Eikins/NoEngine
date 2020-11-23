#pragma once

#include "SwapChain.h"
#include "Buffer.h"

namespace Graphics
{
	class RenderDevice
	{
	public:
		//virtual SwapChain CreateSwapChain() = 0;

		virtual Buffer CreateBuffer(BufferDescriptor descriptor, BufferData data) = 0;


	};
}