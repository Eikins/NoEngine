#pragma once

#include "SwapChain.h"

namespace Graphics
{
	class RenderDevice
	{
	public:
		virtual SwapChain CreateSwapChain() = 0;
	};
}