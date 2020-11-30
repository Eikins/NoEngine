#pragma once

#include "CommandBuffer.h"

namespace Graphics
{
	class DeviceContext
	{
		virtual void ExecuteCommandBuffer(const CommandBuffer& commandBuffer) = 0;
	};
}